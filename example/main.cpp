//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder example
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <math.h>
#include <getopt.h>

#include <boost/algorithm/string.hpp>

#include "Fetch.h"

#include "Metar.h"
#include "Convert.h"
#include "Utils.h"

using namespace std;
using namespace Storage_B::Weather;

static const string URL = 
  "http://tgftp.nws.noaa.gov//data/observations/metar/stations/";

static void usage(const string& command)
{
  cerr << "usage: " << command << " [options..] <stationString>\n"; 
  cerr << " -f, --fahrenheit Print temperature in Fahrenheit\n";
}

static const char *DEG_SIM = "\u00B0";

static const char *speed_units[]
{
  "KT",
  "MPS",
  "KPH"
};

static const char *sky_conditions[] =
{
  "SKC",
  "CLR",
  "NSC",
  "FEW",
  "SCT",
  "BKN",
  "OVC"
};

static const char *cloud_types[] =
{
  "TCU",
  "CB",
  "ACC" 
};

static void print_temp(double temp, bool fahrenheit_flag)
{
  cout << (fahrenheit_flag ? Convert::c2f(temp) : temp) 
         << DEG_SIM << (fahrenheit_flag ? 'F' : 'C');
}

int main(int argc, char **argv)
{
  static struct option long_options[] =
  {
    {"help", no_argument, NULL, 'h'},
    {"fahrenheit", no_argument, NULL, 'f'},
    {0, 0, 0, 0}
  };

  bool fahrenheit_flag(false);
  int option_index(0);
  int c;
  
  string command = basename(argv[0]);

  opterr = 0;
  while((c = getopt_long(argc, argv, "hf", long_options, &option_index)) != -1)
  {
    switch(c)
    {
      case 'h':
        usage(command);
        return 1;

      case 'f':
        fahrenheit_flag = true;
        break;

      default:
        usage(command);
        return 1;
    }
  }

  if (argv[optind])
  {
    string url(URL + argv[optind] + ".TXT");
    
    Fetch fetch(url.c_str());
    string data;
  
    long result = fetch(data);

    if (!Curl::httpStatusOK(result))
    {
      cerr << "http_status = " << result << endl;
      return 1;
    }

    cout << data << endl;
    
    //
    // This METAR source returns 2 lines of text.
    //   * The first line is the observation time and date (UTC)
    //   * The second line is the METAR string
    //
    vector<string> list;
    boost::split(list, data, boost::is_any_of("\n"));

    Metar metar(list[1].c_str());

    double temp, dew;
    if (metar.hasTemperatureNA())
    {
      temp = metar.TemperatureNA();
      dew = metar.DewPointNA();
    }
    else
    {
      temp = metar.Temperature();
      dew = metar.DewPoint();
    }
    
    cout << metar.ICAO() << endl;
    cout << setprecision(1) << fixed;

    cout <<   "Temperature: ";
    print_temp(temp, fahrenheit_flag);  

    double feels_like(temp);
    if (metar.hasWindSpeed())
    {
      double wind_kph;
      switch(metar.WindSpeedUnits())
      {
        case Metar::speed_units::KT:
          wind_kph = Convert::Kts2Kph(metar.WindSpeed());
          break;

        case Metar::speed_units::MPS:
          wind_kph = metar.WindSpeed() / 1000.0;
          break;

        default:
          wind_kph = metar.WindSpeed();
          break;
      }
      
      feels_like = Utils::WindChill(temp, wind_kph);
    }

    double humidity =  Utils::Humidity(temp, dew);
    if (feels_like == temp)
    {
      feels_like = Utils::HeatIndex(temp, humidity, true);
    }
      
    if (feels_like != temp)
    {
      cout <<   "\nFeels Like:  ";
      print_temp(feels_like, fahrenheit_flag);
    }

    cout << "\nDew Point:   ";
    print_temp(dew, fahrenheit_flag);

    cout << "\nHumidity:    " << humidity << "%" << endl;

    cout << "Pressure:    ";
    if (metar.hasAltimeterA())
      cout << metar.AltimeterA() << " inHg" << endl;
    else if (metar.hasAltimeterQ())
      cout << metar.AltimeterQ() << " hPa" << endl;

    if (metar.hasWindSpeed())
    {
      cout << "\nWind:        ";
      if (!metar.isVariableWindDirection())
      { 
        cout << metar.WindDirection() << DEG_SIM;
      }
      else
      {
        cout << "VRB";
      }
      cout << " / " << metar.WindSpeed();
      if (metar.hasWindGust())
      {
        cout << " (" << metar.WindGust() << ")";
      }
      cout << " "
           << speed_units[static_cast<int>(metar.WindSpeedUnits())] << endl;
    }

    cout << setprecision(2) << fixed;
    if (metar.hasVisibility())
    {
      cout << "\nVisibility:  " << metar.Visibility() << " ";
      if (metar.VisibilityUnits() == Metar::distance_units::SM)
      {
        cout << "meters" << endl;
      }
      else
      {
        cout << "miles" << endl;
      }
    }
    
    cout << endl;
    for (unsigned int i = 0 ; i < metar.NumCloudLayers() ; i++)
    {
      auto layer = metar.Layer(i);
      cout << sky_conditions[static_cast<int>(layer->Cover())];
      if (layer->hasAltitude())
      {
        cout << ": " << layer->Altitude() << " feet";
        if (layer->hasCloudType())
        {
          cout << " (" << cloud_types[static_cast<int>(layer->CloudType())] 
               << ")";
        }
      }
      cout << endl;
    }

    return 0;
  }

  usage(command);

  return 1;
}
