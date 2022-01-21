//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#include "Metar.h"

#include <cstring>
#include <cstdlib>
#include <cctype>

#include <climits>
#include <cfloat>

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
    return starts_with("T####", str);
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

  virtual phenom operator[](typename std::vector<Phenom>::size_type) const 
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
  virtual bool ThunderStorm() const { return false; }
  virtual bool Temporary() const { return false; }
};

class MetarImpl : public Metar
{
public:
  MetarImpl(const char *metar_str);
  MetarImpl(char *metar_str);

  virtual ~MetarImpl() = default;

  MetarImpl(const MetarImpl&) = delete;
  MetarImpl& operator=(const MetarImpl&) = delete;

  virtual std::optional<message_type> MessageType() const { return _message_type; }

  virtual std::optional<std::string> ICAO() const { return _icao; }
      
  virtual std::optional<int> Day() const { return _day; }

  virtual std::optional<int> Hour() const { return _hour; }

  virtual std::optional<int> Minute() const { return _min; }

  virtual std::optional<int> WindDirection() const { return _wind_dir; }

  virtual bool isVariableWindDirection() const { return _vrb; }

  virtual std::optional<int> WindSpeed() const { return _wind_spd; }

  virtual std::optional<int> WindGust() const { return _gust; }

  virtual std::optional<int> MinWindDirection() const { return _min_wind_dir; }

  virtual std::optional<int> MaxWindDirection() const { return _max_wind_dir; }

  virtual std::optional<speed_units> WindSpeedUnits() const
  {
    return _wind_speed_units;
  }

  virtual std::optional<double> Visibility() const { return _vis; }

  virtual std::optional<distance_units> VisibilityUnits() const
  {
    return _vis_units;
  }

  virtual bool isVisibilityLessThan() const { return _vis_lt; }
  
  virtual bool isCAVOK() const { return _cavok; }
      
  virtual std::optional<int> VerticalVisibility() const { return _vert_vis; }
  
  virtual std::optional<int> Temperature() const { return _temp; }

  virtual std::optional<int> DewPoint() const { return _dew; }

  virtual std::optional<double> AltimeterA() const { return _altimeterA; }

  virtual std::optional<int> AltimeterQ() const { return _altimeterQ; }

  virtual std::optional<double> SeaLevelPressure() const { return _slp; }

  virtual std::optional<double> TemperatureNA() const { return _ftemp; }

  virtual std::optional<double> DewPointNA() const { return _fdew; }

  virtual unsigned int NumCloudLayers() const
  { 
    return _layers.size(); 
  }

  std::shared_ptr<Clouds> Layer(unsigned int idx) const
  {
    if (idx < NumCloudLayers())
    {
      return _layers[idx];
    }

    return nullptr;
  }

  unsigned int NumPhenomena() const
  {
    return _phenomena.size();
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

  std::optional<message_type> _message_type;

  std::optional<std::string> _icao;

  std::optional<int> _day;
  std::optional<int> _hour;
  std::optional<int> _min;

  std::optional<int> _wind_dir;
  std::optional<int> _wind_spd;
  std::optional<int> _gust;
  std::optional<speed_units> _wind_speed_units;

  std::optional<int> _min_wind_dir;
  std::optional<int> _max_wind_dir;
  bool _vrb;

  std::optional<double> _vis;
  std::optional<distance_units> _vis_units;
  bool _vis_lt;
  bool _cavok;

  std::vector<std::shared_ptr<Clouds>> _layers;

  std::vector<std::shared_ptr<Phenom>> _phenomena;

  std::optional<int> _vert_vis;

  std::optional<int> _temp;
  std::optional<int> _dew;

  std::optional<double> _altimeterA;

  std::optional<int> _altimeterQ;

  bool _rmk;
  bool _tempo;

  std::optional<double> _slp;

  std::optional<double> _ftemp;
  std::optional<double> _fdew;

  const char *_previous_element;

  std::shared_ptr<Phenom> _default_phenom;
};

std::shared_ptr<Metar> Metar::Create(const char *metar_str)
{
  return std::make_shared<MetarImpl>(metar_str);
}

std::shared_ptr<Metar> Metar::Create(char *metar_str)
{
  return std::make_shared<MetarImpl>(metar_str);
}

MetarImpl::MetarImpl()
  : _vrb(false)
  , _vis_lt(false)
  , _cavok(false)
  , _rmk(false)
  , _tempo(false)
  , _previous_element(nullptr)
{
  _default_phenom = std::make_shared<PhenomDefault>();
}

MetarImpl::MetarImpl(const char *metar_str) : MetarImpl()
{
  parse(metar_str);
}

MetarImpl::MetarImpl(char *metar_str) : MetarImpl()
{
  parse(metar_str);
}

void MetarImpl::parse(const char *metar_str)
{
  char *metar_dup = strdup(metar_str);
  parse(metar_dup);
  free(metar_dup);
}

void MetarImpl::parse(char *metar_str)
{
  char *sp;
  char *el = strtok_r(metar_str, " ", &sp);
  while (el)
  {
    if (!_message_type.has_value() && is_message_type(el))
    {
      parse_message_type(el);
    }
    else if (!_icao.has_value() && is_icao(el))
    {
      parse_icao(el);
    }
    else if (!_min.has_value() && is_ot(el))
    {
      parse_ot(el);
    }
    else if (!_wind_spd.has_value() && is_wind(el))
    {
      parse_wind(el);
    }
    else if (!_min_wind_dir.has_value() && is_wind_var(el))
    {
      parse_wind_var(el);
    }
    else if (!_vis.has_value() && !_cavok && is_vis(el))
    {
      parse_vis(el);
    }
    else if (!_vert_vis.has_value() && is_vert_vis(el))
    {
      parse_vert_vis(el);
    }
    else if (!_temp.has_value() && is_temp(el))
    {
      parse_temp(el);
    } 
    else if (!_altimeterA.has_value() && is_altA(el))
    {
      parse_alt(el);
    }
    else if (!_altimeterQ.has_value() && is_altQ(el))
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
    else if (!_slp.has_value() && is_slp(el))
    {
      parse_slp(el);
    }
    else if (!_ftemp.has_value() && is_tempNA(el))
    {
      parse_tempNA(el);
    }
    else if (!_rmk)
    {
      parse_cloud_layer(el);
      parse_phenom(el);
    }

    _previous_element = el;

    el = strtok_r(nullptr, " ", &sp);
  }
}

void MetarImpl::parse_message_type(const char *str)
{
  _message_type = str[0] == 'S' ? message_type::SPECI : message_type::METAR;
}

void MetarImpl::parse_icao(const char *str)
{
  _icao = str;
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

      double vis = numerator / denominator;
      if (match("#", _previous_element))
      {
        vis += atof(_previous_element);
      }
      _vis = vis;
    }
    _vis_units = distance_units::SM;
  }
}

void MetarImpl::parse_cloud_layer(const char *str)
{
  auto c = Clouds::Create(str, _tempo);

  if (c != nullptr)
  {
    _layers.push_back(c);
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
    _phenomena.push_back(p);
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

  if (strlen(str) > 5)
  {
    strcpy(val, str + 5);
    _fdew = tempNA(val);
  }
}
