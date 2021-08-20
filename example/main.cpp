//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder example
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include <cmath>
#include <getopt.h>

#include <boost/algorithm/string.hpp>

#include "Fetch.h"

#include "Metar.h"
#include "Convert.h"
#include "Utils.h"

#include "Phenom2String.h"

using namespace std;
using namespace Storage_B::Weather;
using namespace Storage_B::Curlpp;

static const string URL = 
  "https://tgftp.nws.noaa.gov/data/observations/metar/stations/";

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

static optional<double> Temp(const optional<double>& td,
                             const optional<int>& ti)
{
  if (td.has_value())
    return *td;
  else if (ti.has_value())
    return static_cast<double>(*ti);
  else
    return {};
}
                             

static void print_temp(const optional<double>& temp, bool fahrenheit_flag)
{
  if (temp.has_value())
  {
    cout << (fahrenheit_flag ? Convert::c2f(*temp) : *temp);
  }
  else
  {
    cout << "???";
  }
  cout << DEG_SIM << (fahrenheit_flag ? 'F' : 'C');
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
  bool dflag(false);

  string metar_str;
  
  string command = basename(argv[0]);

  opterr = 0;
  while((c = getopt_long(argc, argv, "hfd:", long_options,
             &option_index)) != -1)
  {
    switch(c)
    {
      case 'h':
        usage(command);
        return 1;

      case 'f':
        fahrenheit_flag = true;
        break;

      case 'd':
        dflag = true;
        metar_str = optarg;
        break;

      default:
        usage(command);
        return 1;
    }
  }

  if (argv[optind] && !dflag)
  {
    string url(URL + argv[optind] + ".TXT");
    
    Fetch fetch(url.c_str());
    string data;
  
    auto result = fetch(data);

    if (!Curl::httpStatusOK(result))
    {
      cerr << "http_status = " << result << endl;
      return 1;
    }

    //
    // This METAR source returns 2 lines of text.
    //   * The first line is the observation time and date (UTC)
    //   * The second line is the METAR string
    //
    vector<string> list;
    boost::split(list, data, boost::is_any_of("\n"));

    metar_str = list[1];
  }

  if (!metar_str.empty())
  {
    cout << metar_str << endl;

    auto metar = Metar::Create(metar_str.c_str());

    auto temp =
        Temp(metar->TemperatureNA(), metar->Temperature());  
    auto dew =
        Temp(metar->DewPointNA(), metar->DewPoint());
    
    cout << metar->ICAO().value() << endl;
    cout << setprecision(1) << fixed;

    cout <<   "Temperature: ";
    print_temp(temp, fahrenheit_flag);  

    optional<double> feels_like;
    if (metar->WindSpeed().has_value() && temp.has_value())
    {
      double wind_kph;
      switch(metar->WindSpeedUnits().value())
      {
        case Metar::speed_units::KT:
          wind_kph = Convert::Kts2Kph(metar->WindSpeed().value());
          break;

        case Metar::speed_units::MPS:
          wind_kph = metar->WindSpeed().value() / 1000.0;
          break;

        default:
          wind_kph = metar->WindSpeed().value();
          break;
      }
     
      feels_like = Utils::WindChill(*temp, wind_kph);
      if (feels_like == temp) feels_like.reset();
    }

    optional<double> humidity;
    if (temp.has_value() && dew.has_value())
    {
      humidity = Utils::Humidity(*temp, *dew);
      if (!feels_like.has_value())
      {
        feels_like = Utils::HeatIndex(*temp, *humidity, true);
        if (feels_like == temp) feels_like.reset();
      }
    }
      
    if (feels_like.has_value())
    {
      cout <<   "\nFeels Like:  ";
      print_temp(feels_like, fahrenheit_flag);
    }

    cout << "\nDew Point:   ";
    print_temp(dew, fahrenheit_flag);

    if (humidity.has_value())
    {
      cout << "\nHumidity:    " << *humidity << "%";
    }

    cout << "\nPressure:    ";
    if (metar->AltimeterA().has_value())
      cout << metar->AltimeterA().value() << " inHg" << endl;
    else if (metar->AltimeterQ().has_value())
      cout << metar->AltimeterQ().value() << " hPa" << endl;

    if (metar->WindSpeed().has_value())
    {
      cout << "\nWind:        ";
      if (!metar->isVariableWindDirection())
      { 
        cout << metar->WindDirection().value() << DEG_SIM;
      }
      else
      {
        cout << "VRB";
      }
      cout << " / " << metar->WindSpeed().value();
      if (metar->WindGust().has_value())
      {
        cout << " (" << metar->WindGust().value() << ")";
      }
      cout << " "
           << speed_units[
              static_cast<int>(metar->WindSpeedUnits().value())] << endl;
    }

    cout << setprecision(2) << fixed;
    if (metar->Visibility().has_value())
    {
      cout << "\nVisibility:  " << metar->Visibility().value() << " ";
      if (metar->VisibilityUnits() == Metar::distance_units::M)
      {
        cout << "meters" << endl;
      }
      else
      {
        cout << "miles" << endl;
      }
    }
    
    cout << endl;
    for (unsigned int i = 0 ; i < metar->NumCloudLayers() ; i++)
    {
      auto layer = metar->Layer(i);
      if (!layer->Temporary())
      {
        cout << sky_conditions[static_cast<int>(layer->Cover())];
        if (layer->Altitude().has_value())
        {
          cout << ": " << layer->Altitude().value() * 100  << " feet";
          if (layer->CloudType().has_value())
          {
            cout << " ("
                 << cloud_types[static_cast<int>(layer->CloudType().value())] 
                 << ")";
          }
        }
        cout << endl;
      }
    }

    for (unsigned int i = 0 ; i < metar->NumPhenomena() ; i++)
    {
      const auto& p = metar->Phenomenon(i);
      cout << Phenom2String(p) << endl;
    }

    return 0;
  }

  usage(command);

  return 1;
}
