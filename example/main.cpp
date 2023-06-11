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
#include <libgen.h>

#include "Fetch.h"

#include "Metar.h"
#include "Clouds.h"
#include "Convert.h"
#include "Utils.h"

#include "Phenom2String.h"

using namespace Storage_B::Weather;
using namespace Storage_B::Curlpp;

static const std::string URL = 
  "https://tgftp.nws.noaa.gov/data/observations/metar/stations/";

static void usage(const std::string& command)
{
  std::cerr << "usage: " << command << " [options..] <stationString>\n"; 
  std::cerr << " -f, --fahrenheit Print temperature in Fahrenheit\n";
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

static std::optional<double> Temp(const std::optional<double>& td,
                             const std::optional<int>& ti)
{
  if (td.has_value())
    return *td;
  else if (ti.has_value())
    return static_cast<double>(*ti);
  else
    return {};
}
                             

static void print_temp(const std::optional<double>& temp,
                       bool fahrenheit_flag)
{
  if (temp.has_value())
  {
    std::cout << (fahrenheit_flag ? Convert::c2f(*temp) : *temp);
  }
  else
  {
    std::cout << "???";
  }
  std::cout << DEG_SIM << (fahrenheit_flag ? 'F' : 'C');
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

  std::string metar_str;
  
  std::string command = basename(argv[0]);

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
    std::string url(URL + argv[optind] + ".TXT");
    
    Fetch fetch(url.c_str());
    std::string data;
  
    auto result = fetch(data);

    if (!Curl::httpStatusOK(result))
    {
      std::cerr << "http_status = " << result << std::endl;
      return 1;
    }

    //
    // This METAR source returns 2 lines of text.
    //   * The first line is the observation time and date (UTC)
    //   * The second line is the METAR string
    //
    auto pos = data.find('\n');
    metar_str = data.substr(pos);

    const std::string ws(" \t\v\r\n");
    pos = metar_str.find_first_not_of(ws);
    auto end = metar_str.find_last_not_of(ws);
    metar_str = metar_str.substr(pos, end);
  }

  if (!metar_str.empty())
  {
    std::cout << metar_str << '\n';

    auto metar = Metar::Create(metar_str.c_str());

    auto temp =
        Temp(metar->TemperatureNA(), metar->Temperature());  
    auto dew =
        Temp(metar->DewPointNA(), metar->DewPoint());
    
    std::cout << metar->ICAO().value() << '\n';
    std::cout << std::setprecision(1) << std::fixed;

    std::cout <<   "Temperature: ";
    print_temp(temp, fahrenheit_flag);  

    std::optional<double> feels_like;
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

    std::optional<double> humidity;
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
      std::cout <<   "\nFeels Like:  ";
      print_temp(feels_like, fahrenheit_flag);
    }

    std::cout << "\nDew Point:   ";
    print_temp(dew, fahrenheit_flag);

    if (humidity.has_value())
    {
      std::cout << "\nHumidity:    " << *humidity << "%";
    }

    std::cout << "\nPressure:    ";
    if (metar->AltimeterA().has_value())
      std::cout << metar->AltimeterA().value() << " inHg" << '\n';
    else if (metar->AltimeterQ().has_value())
      std::cout << metar->AltimeterQ().value() << " hPa" << '\n';

    if (metar->WindSpeed().has_value())
    {
      std::cout << "\nWind:        ";
      if (!metar->isVariableWindDirection())
      { 
        std::cout << metar->WindDirection().value() << DEG_SIM;
      }
      else
      {
        std::cout << "VRB";
      }
      std::cout << " / " << metar->WindSpeed().value();
      if (metar->WindGust().has_value())
      {
        std::cout << " (" << metar->WindGust().value() << ")";
      }
      std::cout << " "
                << speed_units[
                     static_cast<int>(metar->WindSpeedUnits().value())] 
                << '\n';
    }

    std::cout << std::setprecision(2) << std::fixed;
    if (metar->Visibility().has_value())
    {
      std::cout << "\nVisibility:  " << metar->Visibility().value() << " ";
      if (metar->VisibilityUnits() == Metar::distance_units::M)
      {
        std::cout << "meters" << '\n';
      }
      else
      {
        std::cout << "miles" << '\n';
      }
    }
    
    std::cout << '\n';
    for (unsigned int i = 0 ; i < metar->NumCloudLayers() ; i++)
    {
      auto layer = metar->Layer(i);
      if (!layer->Temporary())
      {
        std::cout << sky_conditions[static_cast<int>(layer->Cover())];
        if (layer->Altitude().has_value())
        {
          std::cout << ": " << layer->Altitude().value() * 100  << " feet";
          if (layer->CloudType().has_value())
          {
            std::cout << " ("
                      << cloud_types[
                           static_cast<int>(layer->CloudType().value())] 
                      << ")";
          }
        }
        std::cout << '\n';
      }
    }

    for (unsigned int i = 0 ; i < metar->NumPhenomena() ; i++)
    {
      const auto& p = metar->Phenomenon(i);
      std::cout << Phenom2String(p) << '\n';
    }

    return 0;
  }

  usage(command);

  return 1;
}
