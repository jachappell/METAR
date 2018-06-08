//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#include "Metar.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <limits.h>
#include <float.h>

using namespace std;
using namespace Storage_B::Weather;
   
const int Metar::_INTEGER_UNDEFINED = INT_MIN;
const double Metar::_DOUBLE_UNDEFINED = DBL_MAX;

#ifdef NO_SHARED_PTR
const unsigned int Metar::_MAX_CLOUD_LAYERS = 6;
const unsigned int Metar::_MAX_PHENOM = 16;
#endif

namespace
{
  const char *WIND_SPEED_KT = "KT";
  const char *WIND_SPEED_MPS = "MPS";
  const char *WIND_SPEED_KPH = "KPH";

  const char *VIS_UNITS_SM = "SM";
    
  bool match(const char *pattern, const char *str,
      bool (*f)(size_t, size_t))
  {
    size_t len = strlen(pattern);
    if (str && f(len, strlen(str)))
    {
      for (size_t i = 0 ; i < len ; i++)
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

  inline bool match(const char *pattern, const char *str)
  {
    return match(pattern, str, [](size_t a, size_t b) { return a == b; });
  }  

  inline bool starts_with(const char *pattern, const char *str)
  {
    return match(pattern, str, [](size_t a, size_t b) { return a <= b; });
  }  

  inline bool is_message_type(const char *str)
  {
    return !strcmp(str, "METAR") || !strcmp(str, "SPECI");
  }

  inline bool is_icao(const char *str)
  {
    return match("$$$$", str);
  }

  inline bool is_ot(const char *str)
  {
    return match("######Z", str);
  }

  inline bool is_wind(const char *str)
  {
    return starts_with("#####", str) 
        || starts_with("#####G##", str) 
        || starts_with("######G###", str)
        || starts_with("VRB", str);
  }

  inline bool is_wind_var(const char *str)
  {
    return match("###V###", str);
  }

  inline bool is_vis(const char *str)
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

  inline bool is_vert_vis(const char *str)
  {
    return match("VV###", str);
  }

  inline bool is_temp(const char *str)
  {
    return match("##/##", str) 
      || match("##/M##", str) 
      || match("M##/M##", str)
      || match("##/", str)
      || match("M##/", str);
  }

  inline bool is_altA(const char *str)
  {
    return match("A####", str);
  }

  inline bool is_altQ(const char *str)
  {
    return match("Q####", str);
  }

  inline bool is_rmk(const char *str)
  {
    return strcmp(str, "RMK") == 0;
  }

  inline bool is_tempo(const char *str)
  {
    return strcmp(str, "TEMPO") == 0;
  }

  inline bool is_slp(const char *str)
  {
    return match("SLP###", str);
  }

  inline bool is_tempNA(const char *str)
  {
    return match("T########", str);
  }
    
  inline int temp(char *val)
  {
    if (val[0] == 'M') val[0] = '-';
    return atoi(val);
  }
    
  inline double tempNA(char *val)
  {
    if (val[0] == '1') val[0] = '-';
    return atof(val) / 10.0;
  }
}

Metar::Metar()
  : _message_type(message_type::undefined)
  , _day(_INTEGER_UNDEFINED)
  , _hour(_INTEGER_UNDEFINED)
  , _min(_INTEGER_UNDEFINED)
  , _wind_dir(_INTEGER_UNDEFINED) 
  , _wind_spd(_INTEGER_UNDEFINED)
  , _gust(_INTEGER_UNDEFINED)
  , _wind_speed_units(speed_units::undefined)
  , _min_wind_dir(_INTEGER_UNDEFINED)
  , _max_wind_dir(_INTEGER_UNDEFINED)
  , _vrb(false)
  , _vis(_DOUBLE_UNDEFINED)
  , _vis_units(distance_units::undefined)
  , _vis_lt(false)
  , _cavok(false)
#ifdef NO_SHARED_PTR
  , _num_layers(0)
  , _num_phenomena(0)
#endif
  , _vert_vis(_INTEGER_UNDEFINED)
  , _temp(_INTEGER_UNDEFINED) 
  , _dew(_INTEGER_UNDEFINED)
  , _altimeterA(_DOUBLE_UNDEFINED)
  , _altimeterQ(_INTEGER_UNDEFINED)
  , _rmk(false)
  , _tempo(false)
  , _slp(_DOUBLE_UNDEFINED)
  , _ftemp(_DOUBLE_UNDEFINED) 
  , _fdew(_DOUBLE_UNDEFINED)
  , _previous_element(nullptr)
{
  _icao[0] = '\0';

#ifdef NO_SHARED_PTR
  _layers = new Clouds*[_MAX_CLOUD_LAYERS];
  _phenomena = new Phenom*[_MAX_PHENOM];
#endif
}

Metar::Metar(const char *metar_str) : Metar()
{
  parse(metar_str);
}

Metar::Metar(char *metar_str) : Metar()
{
  parse(metar_str);
}

#ifdef NO_SHARED_PTR
Metar::~Metar()
{
  for (size_t i = 0 ; i < _num_layers ; i++)
  {
    delete _layers[i];
  }
  delete[] _layers;

  for (size_t i = 0 ; i < _num_phenomena ; i++)
  {
    delete _phenomena[i];
  }

  delete[] _phenomena;
}
#endif

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
    if (!hasMessageType() && is_message_type(el))
    {
      parse_message_type(el);
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
    else if (!_tempo && is_tempo(el))
    {
      _tempo = true;
    }
    else if (!_rmk && is_rmk(el))
    {
      _rmk = true;
    }
    else if (!hasSeaLevelPressure() && is_slp(el))
    {
      parse_slp(el);
    }
    else if (!hasTemperatureNA() && is_tempNA(el))
    {
      parse_tempNA(el);
    }
    else if (!_rmk)
    {
      parse_cloud_layer(el);
      parse_phenom(el);
    }

    _previous_element = el;

    el = strtok(nullptr, " ");
  }
}

void Metar::parse_message_type(const char *str)
{
  if (strcmp(str, "SPECI") == 0)
  {
    _message_type = message_type::SPECI;
  }
  else
  {
    _message_type = message_type::METAR;
  }
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

  _min = atoi(str + 4);
}

void Metar::parse_wind(const char *str)
{
  if (strstr(str, WIND_SPEED_MPS))
  {
    _wind_speed_units = speed_units::MPS;
  }
  else if (strstr(str, WIND_SPEED_KPH))
  {
    _wind_speed_units = speed_units::KPH;
  }
  else if (strstr(str, WIND_SPEED_KT))
  {
    _wind_speed_units = speed_units::KT;
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
    _vis_units = distance_units::M;
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
    _vis_units = distance_units::SM;
  }
}

void Metar::parse_cloud_layer(const char *str)
{
  auto c = Clouds::Create(str, _tempo);

  if (c != nullptr)
  {
#ifndef NO_SHARED_PTR
        _layers.push_back(c);
#else
        _layers[_num_layers++] = c;
#endif
  }
}

void Metar::parse_vert_vis(const char *str)
{
  _vert_vis = atoi(str + 2) * 100;
}

void Metar::parse_temp(const char *str)
{
  char val[4];

  const char *p = strstr(str, "/");
  strncpy(val, str, p - str);

  val[p - str] = '\0';
  _temp = temp(val);

  if (*(p + 1) != '\0')
  {
    strcpy(val, p + 1);
    _dew = temp(val);
  }
}

void Metar::parse_alt(const char *str)
{
  int val = atoi(str + 1);
  if (str[0] == 'Q')
    _altimeterQ = val;
  else
    _altimeterA = static_cast<double>(val) / 100.0;
}

void Metar::parse_phenom(const char *str)
{

  auto p = Phenom::Create(str, _tempo);

  if (p != nullptr)
  {
#ifndef NO_SHARED_PTR
    _phenomena.push_back(p);
#else
    _phenomena[_num_phenomena++] = p;
#endif
  }
}

void Metar::parse_slp(const char *str)
{
  _slp = (atof(str + 3) / 10.0) + 1000.0;
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
