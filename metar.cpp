//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder
//

#include "metar.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <limits.h>
#include <float.h>

int Metar::_INTEGER_UNDEFINED = INT_MIN;
double Metar::_DOUBLE_UNDEFINED = DBL_MAX;

static const char *WIND_SPEED_KT = "KT";
static const char *WIND_SPEED_MPS = "MPS";
static const char *WIND_SPEED_KPH = "KPH";

static const char *VIS_UNITS_M = "M";
static const char *VIS_UNITS_SM = "SM";

Metar::Metar()
  : _day(_INTEGER_UNDEFINED)
  , _hour(_INTEGER_UNDEFINED)
  , _min(_INTEGER_UNDEFINED)
  , _wind_dir(_INTEGER_UNDEFINED) 
  , _wind_spd(_INTEGER_UNDEFINED)
  , _gust(_INTEGER_UNDEFINED)
  , _wind_speed_units(nullptr)
  , _min_wind_dir(_INTEGER_UNDEFINED)
  , _max_wind_dir(_INTEGER_UNDEFINED)
  , _vrb(false)
  , _vis(_DOUBLE_UNDEFINED)
  , _vis_units(nullptr)
  , _vis_lt(false)
  , _cavok(false)
  , _vert_vis(_INTEGER_UNDEFINED)
  , _temp(_INTEGER_UNDEFINED) 
  , _dew(_INTEGER_UNDEFINED)
  , _altimeterA(_DOUBLE_UNDEFINED)
  , _altimeterQ(_INTEGER_UNDEFINED)
  , _slp(_DOUBLE_UNDEFINED)
  , _ftemp(_DOUBLE_UNDEFINED) 
  , _fdew(_DOUBLE_UNDEFINED)
  , _previous_element(nullptr)
{
  _metar[0] = '\0';
  _icao[0] = '\0';
}

Metar::Metar(const char *metar_str) : Metar()
{
  parse(metar_str);
}

Metar::Metar(char *metar_str) : Metar()
{
  parse(metar_str);
}

static bool match(const char *pattern, const char *str)
{
  size_t pattern_len = strlen(pattern);
  if (str && (pattern_len == strlen(str)))
  {
    for (size_t i = 0 ; i < pattern_len ; i++)
    {
      switch(pattern[i])
      {
        case '#':
          if (!isdigit(str[i])) return false;
          break;

        case '$':
          if (!isalpha(str[i])) return false;
          break;

        default:
          if (pattern[i] != str[i]) return false;
          break;
      }
    }

    return true;
  }

  return false;
}

static inline bool is_metar(const char *str)
{
  return !strcmp(str, "METAR") || !strcmp(str, "SPECI");
}

static inline bool is_icao(const char *str)
{
  return match("$$$$", str);
}

static inline bool is_ot(const char *str)
{
  return match("######Z", str);
}

static inline bool is_wind(const char *str)
{
  return match("#####KT", str) 
    || match("#####G##KT", str) 
    || match("VRB##KT", str)
    || match("VRB##G##KT", str)
    || match("VRB##G###KT", str)
    || match("VRB###G###KT", str)
    || match("######KT", str)
    || match("#####G###KT", str)
    || match("######G###KT", str)
    || match("#####MPS", str)
    || match("#####G##MPS", str)
    || match("VRB##MPS", str)
    || match("VRB##G##MPS", str)
    || match("VRB##G###MPS", str)
    || match("VRB###G###MPS", str)
    || match("######MPS", str)
    || match("#####G###MPS", str)
    || match("######G###MPS", str)
    || match("#####KPH", str)
    || match("#####G##KPH", str)
    || match("VRB##KPH", str)
    || match("VRB##G##KPH", str)
    || match("VRB##G###KPH", str)
    || match("VRB###G###KPH", str)
    || match("######KPH", str)
    || match("#####G###KPH", str)
    || match("######G###KPH", str);
}

static inline bool is_wind_var(const char *str)
{
  return match("###V###", str);
}

static inline bool is_vis(const char *str)
{
  if (!strcmp(str, "CAVOK"))
    return true;

  const char *p = strstr(str, VIS_UNITS_SM);
  if (!p)
  {  
    return match("####", str);
  }
 
  auto len = strlen(str); 
  if ((str + len - p) == 2)
  {
    if (!isdigit(str[0]) && (str[0] != 'M')) return false;
    for (size_t i = 1 ; i < len - 2 ; i++)
    {
      if (!isdigit(str[i]) && str[i] != '/') return false;
    }

    return true;
  }

  return false;
}

static inline bool is_vert_vis(const char *str)
{
  return match("VV###", str);
}

static inline bool is_temp(const char *str)
{
  return match("##/##", str) 
    || match("##/M##", str) 
    || match("M##/M##", str);
}

static inline bool is_altA(const char *str)
{
  return match("A####", str);
}

static inline bool is_altQ(const char *str)
{
  return match("Q####", str);
}

static inline bool is_slp(const char *str)
{
  return match("SLP###", str);
}

static inline bool is_tempNA(const char *str)
{
  return match("T########", str);
}

void Metar::parse(const char *metar_str)
{
  char *metar_dup = strdup(metar_str);
  parse(metar_dup);
  free(metar_dup);
}

void Metar::parse(char *metar_str)
{
  char *el = strtok(metar_str, " ");
  while (el)
  {
    if (!hasMETAR() && is_metar(el))
    {
      parse_metar(el);
    }
    else if (!hasICAO() && is_icao(el))
    {
      parse_icao(el);
    }
    else if (!hasMinute() && is_ot(el))
    {
      parse_ot(el);
    }
    else if (!hasWindSpeed() && is_wind(el))
    {
      parse_wind(el);
    }
    else if (!hasMinWindDirection() && is_wind_var(el))
    {
      parse_wind_var(el);
    }
    else if (!hasVisibility() && !_cavok && is_vis(el))
    {
      parse_vis(el);
    }
    else if (!hasVerticalVisibility() && is_vert_vis(el))
    {
      parse_vert_vis(el);
    }
    else if (!hasTemperature() && is_temp(el))
    {
      parse_temp(el);
    } 
    else if (!hasAltimeterA() && is_altA(el))
    {
      parse_alt(el);
    }
    else if (!hasAltimeterQ() && is_altQ(el))
    {
      parse_alt(el);
    }
    else if (!hasSeaLevelPressure() && is_slp(el))
    {
      parse_slp(el);
    }
    else if (!hasTemperatureNA() && is_tempNA(el))
    {
      parse_tempNA(el);
    }

    _previous_element = el;

    el = strtok(nullptr, " ");
  }
}

void Metar::parse_metar(const char *str)
{
  strcpy(_metar, str);
}

void Metar::parse_icao(const char *str)
{
  strcpy(_icao, str);
}

void Metar::parse_ot(const char *str)
{
  char val[3];

  strncpy(val, str, 2);
  val[2] = '\0';
  _day = atoi(val);

  strncpy(val, str + 2, 2);
  val[2] = '\0';
  _hour = atoi(val);

  strcpy(val, str + 4);
  _min = atoi(val);
}

void Metar::parse_wind(const char *str)
{
  if (strstr(str, WIND_SPEED_MPS))
  {
    _wind_speed_units = WIND_SPEED_MPS;
  }
  else if (strstr(str, WIND_SPEED_KPH))
  {
    _wind_speed_units = WIND_SPEED_KPH;
  }
  else
  {
    _wind_speed_units = WIND_SPEED_KT;
  }

  char val[4];

  if (!strstr(str, "VRB"))
  {
    strncpy(val, str, 3);
    val[3] = '\0';
    _wind_dir = atoi(val);
  }
  else
  {
    _vrb = true;
  }
 
  strncpy(val, str + 3, 3);
  val[3] = '\0';
  
  _wind_spd = atoi(val);

  const char *g = strstr(str, "G");
  if (g)
  {
    strncpy(val, g + 1, 3);
    val[3] = '\0';
    _gust = atoi(val);
  } 
}

void Metar::parse_wind_var(const char *str)
{
  char val[4];

  strncpy(val, str, 3);
  val[3] = '\0';
  _min_wind_dir = atoi(val);

  strcpy(val, str + 4);
  _max_wind_dir = atoi(val);
}

void Metar::parse_vis(const char *str)
{
  if (!strcmp(str, "CAVOK"))
  {
    _cavok = true;
    return;
  }

  const char *u = strstr(str, VIS_UNITS_SM);
  if (!u)
  {
    _vis = atof(str);
    _vis_units = VIS_UNITS_M;
  }
  else
  {
    const char *p = strstr(str, "/");
    if (!p)
    {
      _vis = atof(str);
    }
    else
    {
      char val[4];
      auto len = p - str;
      
      if (str[0] == 'M')
      {
        strncpy(val, str + 1, len);
        _vis_lt = true;
      }
      else
      {
        strncpy(val, str, len);
      }
      val[len] = '\0';
      double numerator = atof(val);

      len = u - p;
      strncpy(val, p + 1, len);
      val[len] = '\0';
      double denominator = atof(val);

      _vis = numerator / denominator;
      if (match("#", _previous_element))
      {
        _vis += atof(_previous_element);
      }
    }
    _vis_units = VIS_UNITS_SM;
  }
}

void Metar::parse_vert_vis(const char *str)
{
  _vert_vis = atoi(str + 2) * 100;
}

static inline int temp(char *val)
{
  if (val[0] == 'M') val[0] = '-';
  return atoi(val);
}

void Metar::parse_temp(const char *str)
{
  char val[4];

  const char *p = strstr(str, "/");
  strncpy(val, str, p - str);
  val[p - str] = '\0';
  _temp = temp(val);

  strcpy(val, p + 1);
  _dew = temp(val);
}

void Metar::parse_alt(const char *str)
{
  int val = atoi(str + 1);
  if (str[0] == 'Q')
    _altimeterQ = val;
  else
    _altimeterA = static_cast<double>(val) / 100.0;
}

void Metar::parse_slp(const char *str)
{
  _slp = (atof(str + 3) / 10.0) + 1000.0;
}

static inline double tempNA(char *val)
{
  if (val[0] == '1') val[0] = '-';
  return atof(val) / 10.0;
}

void Metar::parse_tempNA(const char *str)
{
  char val[5];

  strncpy(val, str + 1, 4);
  val[4] = '\0';
  _ftemp = tempNA(val);

  strcpy(val, str + 5);
  _fdew = tempNA(val);
}
