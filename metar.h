//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder
//

#ifndef __METAR_H__
#define __METAR_H__

class Metar
{
public:
  //
  // Constructor
  //    metar_str - METAR to decode
  //
  Metar(const char *metar_str)
    : Metar()
  {
    parse(metar_str);
  }

  //
  // Constructor
  //    metar_str - METAR to decode
  //
  Metar(char *metar_str)
    : Metar()
  {
    parse(metar_str);
  }

  ~Metar() = default;

  // no copy
  Metar(const Metar&) = delete;
  Metar& operator=(const Metar&) = delete;

  //
  // Message type: METAR or SPECI
  //
  const char *METAR() const { return _metar; }
  bool hasMETAR() const { return _metar[0] != '\0'; }

  //
  // Location identifier
  //
  const char *ICAO() const { return _icao; }
  bool hasICAO() const { return _icao[0] != '\0'; }

  //
  // Observation day of month (UTC)
  //
  int Day() const { return _day; }
  bool hasDay() const { return _day != _INTEGER_UNDEFINED; }

  //
  //  Hour of observation (UTC)
  //
  int Hour() const { return _hour; }
  bool hasHour() const { return _hour != _INTEGER_UNDEFINED; }

  //
  //  Minute of observation (UTC)
  //
  int Minute() const { return _min; }
  bool hasMinute() const { return _min != _INTEGER_UNDEFINED; }

  //
  //  Wind direction
  //
  int WindDirection() const { return _wind_dir; }
  bool hasWindDirection() const { return _wind_dir != _INTEGER_UNDEFINED; }

  //
  //  Wind speed
  //
  int WindSpeed() const { return _wind_spd; }
  bool hasWindSpeed() const { return _wind_spd != _INTEGER_UNDEFINED; }

  //
  //  Wind gust
  //
  int WindGust() const { return _gust; }
  bool hasWindGust() const { return _gust != _INTEGER_UNDEFINED; }

  //
  // Minimum wind direction
  //
  int MinWindDirection() const { return _min_wind_dir; }
  int hasMinWindDirection() const
    { return _min_wind_dir != _INTEGER_UNDEFINED ; }

  //
  // Maximum wind direction
  //
  int MaxWindDirection() const { return _max_wind_dir; }
  int hasMaxWindDirection() const
    { return _max_wind_dir != _INTEGER_UNDEFINED ; }

  //
  // Wind speed units
  //    KT  - knots
  //    MPS - meters per second
  //    KPH - kilometers per hour
  //
  const char *WindSpeedUnits() const { return _wind_speed_units; }
  int hasWindSpeedUnits() const { return _wind_speed_units != nullptr;  }

  //
  // Temperature
  //    Celsius
  //
  int Temperature() const { return _temp; }
  bool hasTemperature() const { return _temp != _INTEGER_UNDEFINED; }

  //
  // Dew point
  //    Celsius
  // 
  int DewPoint() const { return _dew; }
  bool hasDewPoint() const { return _dew != _INTEGER_UNDEFINED; }

  //
  // Altimeter setting
  //    inHg
  double AltimeterA() const { return _altimeterA; }
  bool hasAltimeterA() const { return _altimeterA != _DOUBLE_UNDEFINED; }

  //
  // Current altimeter setting (in QNH)
  //    hPa
  int AltimeterQ() const { return _altimeterQ; }
  bool hasAltimeterQ() const { return _altimeterQ != _INTEGER_UNDEFINED; }

  //
  // Barometric pressure extrapolated to sea level 
  //    hPa
  double SeaLevelPressure() const { return _slp; }
  bool hasSeaLevelPressure() const { return _slp != _DOUBLE_UNDEFINED; }

  //
  // Temperature
  //    Celsius
  //
  double TemperatureNA() const { return _ftemp; }
  bool hasTemperatureNA() const { return _ftemp != _DOUBLE_UNDEFINED; }

  //
  // Dew point
  //    Celsius
  // 
  double DewPointNA() const { return _fdew; }
  bool hasDewPointNA() const { return _fdew != _DOUBLE_UNDEFINED; }

private:
  Metar();

  void parse(char *metar_str);
  void parse(const char *metar_str);

  void parse_metar(const char *str);

  void parse_icao(const char *str);

  void parse_ot(const char *str);

  void parse_wind(const char *str);

  void parse_wind_var(const char *str);
  
  void parse_temp(const char *str);

  void parse_alt(const char *str);

  void parse_slp(const char *str);

  void parse_tempNA(const char *str);

  char _metar[6];

  char _icao[5];

  int _day;
  int _hour;
  int _min;

  int _wind_dir;
  int _wind_spd;
  int _gust;
  const char *_wind_speed_units;

  int _min_wind_dir;
  int _max_wind_dir;

  int _temp;
  int _dew;

  double _altimeterA;

  int _altimeterQ;

  double _slp;

  double _ftemp;
  double _fdew;

  static int _INTEGER_UNDEFINED;
  static double _DOUBLE_UNDEFINED;
};

#endif
