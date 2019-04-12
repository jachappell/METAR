//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder Arduino example
//

#include <Ethernet.h>
#include <Metar.h>
#include <Convert.h>
#include <Utils.h>

// See - https://en.wikipedia.org/wiki/List_of_airports_by_ICAO_code:_A
// to find a station near your location
static const char *STATION = "KSTL";

static const char *USERNAME = "anonymous";
static const char *PASSWORD = USERNAME;

// TODO - add ability to specify more units
static const bool FAHRENHEIT = true;

static const size_t BUF_SIZE = 192;

void setup()
{
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

  IPAddress ip(192, 168, 10, 177);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0)
  {
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.begin (9600);
}

void loop()
{
  displayPage(Serial, STATION, FAHRENHEIT);

  delay(3600000);
}

using namespace Storage_B::Weather;

static const char *speed_units[]
{
  "KT",
  "MPS",
  "KPH"
};

#ifndef NO_CLOUDS
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
#endif

static void printDeg(Stream& stream)
{
  stream.write(0xC2);
  stream.write(0xB0);
}

static void printTemp(Stream& stream, double temp, bool fahr_flag, char *disp)
{
  const char deg = 176;
  dtostrf(fahr_flag ? Convert::c2f(temp) : temp, 4, 1, disp);
  sprintf(disp, "%s", disp);
  stream.print(disp);
  printDeg(stream);
  stream.println(fahr_flag ? 'F' : 'C');
}

static void receiveFail(Client& client)
{
  client.println("QUIT");

  while (!client.available()) delay(1);

  while (client.available())
  {
    auto c = client.read();
    Serial.write(c);
  }

  client.stop();
}

static bool receive(Client& client, char *outBuf, bool data_flg = false)
{
  while (!client.available()) delay(1);

  byte respCode = client.peek();

  if (respCode >= '4')
  {
    receiveFail(client);
    return false;
  }

  int idx = 0;

  int lines = 0;

  delay(10);
  while (client.available())
  {
    auto c = client.read();

    if (idx < BUF_SIZE)
    {
      if (data_flg)
      {
        if (c == '\n') lines++;
      }

      if (!data_flg)
      {
        outBuf[idx++] = c;
      }
      else if (lines)
      {
        if (c != '\n')
          outBuf[idx++] = c;
      }
    }
  }
  outBuf[idx < BUF_SIZE ? idx : BUF_SIZE - 1] = '\0';

  return true;
}

static int getData(const char *station, char *buffer)
{
  EthernetClient client;
  const char *address = "tgftp.nws.noaa.gov";

  int status = client.connect(address, 21);
  if (status > 0) {
    if (!receive(client, buffer)) return 0;
    client.print("USER ");
    client.println(USERNAME);

    if (!receive(client, buffer)) return 0;
    client.print("PASS ");
    client.println(PASSWORD);

    if (!receive(client, buffer)) return 0;
    client.println("PASV");

    if (!receive(client, buffer)) return 0;

    char *tStr = strtok(buffer, "(,");
    int array_pasv[6];
    for ( int i = 0; i < 6; i++) {
      tStr = strtok(nullptr, "(,");
      array_pasv[i] = atoi(tStr);
      if (tStr == nullptr)
      {
        Serial.println("Bad PASV Answer");
        receiveFail(client);
        return 0;
      }
    }
    unsigned int hiPort, loPort;

    hiPort = array_pasv[4] << 8;
    loPort = array_pasv[5] & 255;

    hiPort = hiPort | loPort;

    EthernetClient dclient;
    if (dclient.connect(address, hiPort)) {
      client.print("CWD ");
      client.println("data/observations/metar/stations");
      if (!receive(client, buffer)) return 0;
      client.print("RETR ");
      client.print(station);
      client.println(".TXT");
      if (!receive(client, buffer)) return 0;
      if (receive(dclient, buffer, true))
      {
        dclient.stop();
      }
      else
      {
        status = 0;
      }
      client.println("QUIT");
      client.stop();
    }
  }

  return status;
}

static void displayPage(Stream& stream, const char *station, bool fahr_flag)
{
  char buffer[BUF_SIZE];
  int status = getData(station, buffer);
  if (status > 0)
  {
    stream.println();
    stream.println(buffer);
    stream.println();

    auto metar = Metar::Create(buffer);

    sprintf(buffer, "%02d:%02dZ", metar->Hour(), metar->Minute());

    stream.println(metar->ICAO());
    stream.print("Observation time: ");
    stream.println(buffer);
    stream.println();

    double temp = metar->hasTemperatureNA() ? metar->TemperatureNA() : static_cast<double>(metar->Temperature());
    stream.print("Temperature: ");
    printTemp(stream, temp, fahr_flag, buffer);

    double feels_like(temp);
    if (metar->hasWindSpeed())
    {
      double wind_kph;
      switch (metar->WindSpeedUnits())
      {
        case Metar::speed_units::KT:
          wind_kph = Convert::Kts2Kph(metar->WindSpeed());
          break;

        case Metar::speed_units::MPS:
          wind_kph = metar->WindSpeed() / 1000.0;
          break;

        default:
          wind_kph = metar->WindSpeed();
          break;
      }

      feels_like = Utils::WindChill(temp, wind_kph);
    }

    double humidity;
    if (metar->hasDewPointNA() ||  metar->DewPointNA())
    {
      double dew = metar->hasDewPointNA() ? metar->DewPointNA() : static_cast<double>(metar->DewPoint());

      double humidity =  Utils::Humidity(temp, dew);
   

      if (feels_like == temp)
      {
        feels_like = Utils::HeatIndex(temp, humidity, true);
      }
    

      if (feels_like != temp)
      {
        stream.print("Feels Like:  ");
        printTemp(stream, feels_like, fahr_flag, buffer);
      }
  
      stream.print("Dew Point:   ");
      printTemp(stream, dew, fahr_flag, buffer);
  
      dtostrf(humidity, 4, 1, buffer);
  
      sprintf(buffer, "%s%%", buffer);
  
      stream.print("Humidity:    ");
      stream.println(buffer);
    }

    stream.println();

    stream.print("Pressure:    ");

    if (metar->hasAltimeterA())
    {
      stream.print(metar->AltimeterA());
      stream.println(" inHg");
    }
    else if (metar->hasAltimeterQ())
    {
      stream.print(metar->AltimeterQ());
      stream.println(" hPa");
    }
    stream.println();

    if (metar->hasWindSpeed())
    {
      stream.print("Wind:        ");
      if (!metar->isVariableWindDirection())
      {
        stream.print(metar->WindDirection());
        printDeg(stream);
        stream.print(" / ");
      }
      else
      {
        stream.print("VRB / ");
      }
      stream.print(metar->WindSpeed());
      if (metar->hasWindGust())
      {
        stream.print(" (");
        stream.print(metar->WindGust());
        stream.print(")");
      }
      stream.print(" ");
      stream.println(speed_units[static_cast<int>(metar->WindSpeedUnits())]);
      stream.println();
    }

    if (metar->hasVisibility())
    {
      stream.print("Visibility:  ");
      stream.print(metar->Visibility());
      if (metar->VisibilityUnits() == Metar::distance_units::M)
      {
        stream.println(" meters");
      }
      else
      {
        stream.println(" miles");
      }
      stream.println();
    }
#ifndef NO_CLOUDS
    for (unsigned int i = 0 ; i < metar->NumCloudLayers() ; i++)
    {
      auto layer = metar->Layer(i);
      if (!layer->Temporary())
      {
        stream.print(sky_conditions[static_cast<int>(layer->Cover())]);
        if (layer->hasAltitude())
        {
          stream.print(": ");
          stream.print(layer->Altitude() * 100);
          stream.print(" feet");
          if (layer->hasCloudType())
          {
            stream.print(" (");
            stream.print(cloud_types[static_cast<int>(layer->CloudType())]);
            stream.print(")");
          }
        }
        stream.println();
      }
    }
#endif
    delete metar;
  }
  else
  {
    stream.print(status);
    stream.print(": connection failed");
  }
}
