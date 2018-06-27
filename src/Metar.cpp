//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#include "Metar.h"

#include <cstring>
#include <cstdlib>
#include <cctype>

#include <climits>
#include <cfloat>

using namespace std;
using namespace Storage_B::Weather;

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

class PhenomDefault : public Phenom
{
public:
  PhenomDefault() {}
  
  PhenomDefault(const PhenomDefault&) = delete;
  PhenomDefault& operator=(const PhenomDefault&) = delete;

  ~PhenomDefault() = default;

  virtual unsigned int NumPhenom() const { return 0; }

  virtual phenom
#ifndef NO_SHARED_PTR
      operator[](typename std::vector<Phenom>::size_type
#else
      operator[](unsigned int
#endif
                 ) const 
  {
    return phenom::NONE;
  }
  
  virtual intensity Intensity() const { return intensity::NORMAL; } 
  virtual bool Blowing() const { return false; }
  virtual bool Freezing() const { return false; }
  virtual bool Drifting() const { return false; }
  virtual bool Vicinity() const { return false; }
  virtual bool Partial() const { return false; }
  virtual bool Shallow() const { return false; } 
  virtual bool Patches() const { return false; }
  virtual bool Temporary() const { return false; }
};

class MetarImpl : public Metar
{
public:
  MetarImpl(const char *metar_str);
  MetarImpl(char *metar_str);

#ifdef NO_SHARED_PTR
  virtual ~MetarImpl();
#else
  virtual ~MetarImpl() = default;
#endif

  MetarImpl(const MetarImpl&) = delete;
  MetarImpl& operator=(const MetarImpl&) = delete;

  virtual message_type MessageType() const { return _message_type; }
  virtual bool hasMessageType() const
  {
    return _message_type != message_type::undefined;
  }

  virtual const char *ICAO() const { return _icao; }
  virtual bool hasICAO() const { return _icao[0] != '\0'; }
      
  virtual int Day() const { return _day; }
  virtual bool hasDay() const { return _day != _INTEGER_UNDEFINED; }

  virtual int Hour() const { return _hour; }
  virtual bool hasHour() const { return _hour != _INTEGER_UNDEFINED; }

  virtual int Minute() const { return _min; }
  virtual bool hasMinute() const { return _min != _INTEGER_UNDEFINED; }

  virtual int WindDirection() const { return _wind_dir; }
  virtual bool hasWindDirection() const
  { 
    return _wind_dir != _INTEGER_UNDEFINED;
  }

  virtual bool isVariableWindDirection() const { return _vrb; }

  virtual int WindSpeed() const { return _wind_spd; }
  virtual bool hasWindSpeed() const { return _wind_spd != _INTEGER_UNDEFINED; }

  virtual int WindGust() const { return _gust; }
  virtual bool hasWindGust() const { return _gust != _INTEGER_UNDEFINED; }

  virtual int MinWindDirection() const { return _min_wind_dir; }
  virtual int hasMinWindDirection() const
  {
    return _min_wind_dir != _INTEGER_UNDEFINED;
  }

  virtual int MaxWindDirection() const { return _max_wind_dir; }
  virtual int hasMaxWindDirection() const
  {
    return _max_wind_dir != _INTEGER_UNDEFINED;
  }

  virtual speed_units WindSpeedUnits() const { return _wind_speed_units; }
  virtual int hasWindSpeedUnits() const
  {
    return _wind_speed_units != speed_units::undefined;
  }

  virtual double Visibility() const { return _vis; }
  virtual bool hasVisibility() const { return _vis != _DOUBLE_UNDEFINED; }

  virtual distance_units VisibilityUnits() const { return _vis_units; }
  virtual bool hasVisibilityUnits() const
  {
    return _vis_units != distance_units::undefined;
  }

  virtual bool isVisibilityLessThan() const { return _vis_lt; }
  
  virtual bool isCAVOK() const { return _cavok; }
      
  virtual int VerticalVisibility() const { return _vert_vis; }
  virtual bool hasVerticalVisibility() const
  {
    return _vert_vis != _INTEGER_UNDEFINED;
  }
  
  virtual int Temperature() const { return _temp; }
  virtual bool hasTemperature() const { return _temp != _INTEGER_UNDEFINED; }

  virtual int DewPoint() const { return _dew; }
  virtual bool hasDewPoint() const { return _dew != _INTEGER_UNDEFINED; }

  virtual double AltimeterA() const { return _altimeterA; }
  virtual bool hasAltimeterA() const
  {
    return _altimeterA != _DOUBLE_UNDEFINED;
  }

  virtual int AltimeterQ() const { return _altimeterQ; }
  virtual bool hasAltimeterQ() const
  {
    return _altimeterQ != _INTEGER_UNDEFINED;
  }

  virtual double SeaLevelPressure() const { return _slp; }
  virtual bool hasSeaLevelPressure() const { return _slp != _DOUBLE_UNDEFINED; }

  virtual double TemperatureNA() const { return _ftemp; }
  virtual bool hasTemperatureNA() const { return _ftemp != _DOUBLE_UNDEFINED; }

  virtual double DewPointNA() const { return _fdew; }
  virtual bool hasDewPointNA() const { return _fdew != _DOUBLE_UNDEFINED; }

  virtual unsigned int NumCloudLayers() const
  { 
#ifdef NO_SHARED_PTR
    return _num_layers;
#else
    return _layers.size(); 
#endif
  }

#ifndef NO_SHARED_PTR
  std::shared_ptr<Clouds>
#else
  const Clouds *
#endif
  Layer(unsigned int idx) const
  {
    if (idx < NumCloudLayers())
    {
      return _layers[idx];
    }

    return nullptr;
  }

  unsigned int NumPhenomena() const
  {
#ifdef NO_SHARED_PTR
    return _num_phenomena;
#else
    return _phenomena.size();
#endif
  }

  const Phenom& Phenomenon(unsigned int idx) const
  {
    if (idx < NumPhenomena())
    {
      return *_phenomena[idx];
    }

    return *_default_phenom;
  }

private:
  MetarImpl();

  void parse(char *metar_str);
  void parse(const char *metar_str);

  void parse_message_type(const char *str);

  void parse_icao(const char *str);

  void parse_ot(const char *str);

  void parse_wind(const char *str);

  void parse_wind_var(const char *str);

  void parse_vis(const char *str);

  void parse_cloud_layer(const char *str);
  
  void parse_vert_vis(const char *str);
  
  void parse_temp(const char *str);

  void parse_alt(const char *str);

  void parse_slp(const char *str);

  void parse_tempNA(const char *str);

  void parse_phenom(const char *str);

  message_type _message_type;

  char _icao[5];

  int _day;
  int _hour;
  int _min;

  int _wind_dir;
  int _wind_spd;
  int _gust;
  speed_units _wind_speed_units;

  int _min_wind_dir;
  int _max_wind_dir;
  bool _vrb;

  double _vis;
  distance_units _vis_units;
  bool _vis_lt;
  bool _cavok;

#ifndef NO_SHARED_PTR
  std::vector<std::shared_ptr<Clouds>>
#else
  Clouds **
#endif
    _layers;

#ifdef NO_SHARED_PTR
  unsigned int _num_layers;
#endif

#ifndef NO_SHARED_PTR
  std::vector<std::shared_ptr<Phenom>>
#else
  Phenom **
#endif
    _phenomena;
#ifdef NO_SHARED_PTR
 unsigned int _num_phenomena;
#endif

  int _vert_vis;

  int _temp;
  int _dew;

  double _altimeterA;

  int _altimeterQ;

  bool _rmk;
  bool _tempo;

  double _slp;

  double _ftemp;
  double _fdew;

  const char *_previous_element;

  static const int _INTEGER_UNDEFINED;
  static const double _DOUBLE_UNDEFINED;
#ifdef NO_SHARED_PTR
  static const unsigned int _MAX_CLOUD_LAYERS;
  static const unsigned int _MAX_PHENOM;
#endif

#ifndef NO_SHARED_PTR
  std::shared_ptr<Phenom>
#else
  Phenom *
#endif
  _default_phenom;
};
   
const int MetarImpl::_INTEGER_UNDEFINED = INT_MIN;
const double MetarImpl::_DOUBLE_UNDEFINED = DBL_MAX;

#ifdef NO_SHARED_PTR
const unsigned int MetarImpl::_MAX_CLOUD_LAYERS = 6;
const unsigned int MetarImpl::_MAX_PHENOM = 16;
#endif

#ifndef NO_SHARED_PTR
std::shared_ptr<Metar>
#else
Metar *
#endif
Metar::Create(const char *metar_str)
{
#ifndef NO_SHARED_PTR
  return make_shared<MetarImpl>(metar_str);
#else
  return new MetarImpl(metar_str);
#endif
}

#ifndef NO_SHARED_PTR
std::shared_ptr<Metar>
#else
Metar *
#endif
Metar::Create(char *metar_str)
{
#ifndef NO_SHARED_PTR
  return make_shared<MetarImpl>(metar_str);
#else
  return new MetarImpl(metar_str);
#endif
}

MetarImpl::MetarImpl()
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

#ifdef NO_SHARED_PTR
  _default_phenom = new PhenomDefault();
#else
  _default_phenom = make_shared<PhenomDefault>();
#endif

}

MetarImpl::MetarImpl(const char *metar_str) : MetarImpl()
{
  parse(metar_str);
}

MetarImpl::MetarImpl(char *metar_str) : MetarImpl()
{
  parse(metar_str);
}

#ifdef NO_SHARED_PTR
MetarImpl::~MetarImpl()
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

  delete _default_phenom;
}
#endif

void MetarImpl::parse(const char *metar_str)
{
  char *metar_dup = strdup(metar_str);
  parse(metar_dup);
  free(metar_dup);
}

void MetarImpl::parse(char *metar_str)
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

void MetarImpl::parse_message_type(const char *str)
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

void MetarImpl::parse_icao(const char *str)
{
  strcpy(_icao, str);
}

void MetarImpl::parse_ot(const char *str)
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

void MetarImpl::parse_wind(const char *str)
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

void MetarImpl::parse_wind_var(const char *str)
{
  char val[4];

  strncpy(val, str, 3);
  val[3] = '\0';
  _min_wind_dir = atoi(val);

  strcpy(val, str + 4);
  _max_wind_dir = atoi(val);
}

void MetarImpl::parse_vis(const char *str)
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

void MetarImpl::parse_cloud_layer(const char *str)
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

void MetarImpl::parse_vert_vis(const char *str)
{
  _vert_vis = atoi(str + 2) * 100;
}

void MetarImpl::parse_temp(const char *str)
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

void MetarImpl::parse_alt(const char *str)
{
  int val = atoi(str + 1);
  if (str[0] == 'Q')
    _altimeterQ = val;
  else
    _altimeterA = static_cast<double>(val) / 100.0;
}

void MetarImpl::parse_phenom(const char *str)
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

void MetarImpl::parse_slp(const char *str)
{
  _slp = (atof(str + 3) / 10.0) + 1000.0;
}

void MetarImpl::parse_tempNA(const char *str)
{
  char val[5];

  strncpy(val, str + 1, 4);
  val[4] = '\0';
  _ftemp = tempNA(val);

  strcpy(val, str + 5);
  _fdew = tempNA(val);
}
