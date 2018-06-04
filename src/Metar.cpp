//
// Copyright (c) 2018 James A. Chappell
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

static const char *WIND_SPEED_KT = "KT";
static const char *WIND_SPEED_MPS = "MPS";
static const char *WIND_SPEED_KPH = "KPH";

static const char *VIS_UNITS_SM = "SM";

static const char *BR = "BR";
static const char *DS = "DS";
static const char *DU = "DU";
static const char *DZ = "DZ";
static const char *FC = "FC";
static const char *FG = "FG";
static const char *FU = "FU";
static const char *GR = "GR";
static const char *GS = "GS";
static const char *HZ = "HZ";
static const char *IC = "IC";
static const char *PE = "PE";
static const char *PL = "PL"; 
static const char *PO = "PO"; 
static const char *PY = "PY";
static const char *RA = "RA";
static const char *SA = "SA";
static const char *SG = "SG";
static const char *SH = "SH";
static const char *SN = "SN";
static const char *SQ = "SQ";
static const char *SS = "SS"; 
static const char *TS = "TS";
static const char *UP = "UP";
static const char *VA = "VA";

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
static const auto NUM_LAYERS =
  sizeof(sky_conditions) / sizeof(sky_conditions[0]);

static const char *cloud_types[] =
{
  "TCU",
  "CB",
  "ACC" 
};
static const auto NUM_CLOUDS =
  sizeof(cloud_types) / sizeof(cloud_types[0]);

class SkyConditionImpl : public Metar::SkyCondition
{
public:
  SkyConditionImpl(bool temp, cover c, int a = INT_MIN,
                   type t = type::undefined)
    : _cover(c)
    , _alt(a)
    , _tempo(temp)
    , _type(t)
  {
  }

  virtual ~SkyConditionImpl() = default;

  SkyConditionImpl() = delete;

  SkyConditionImpl(const SkyConditionImpl&) = delete;
  SkyConditionImpl& operator=(const SkyConditionImpl&) = delete;

  virtual cover Cover() const { return _cover; } 
  virtual int Altitude() const { return _alt; }
  virtual bool hasAltitude() const { return _alt != INT_MIN; }
  virtual type CloudType() const { return _type; }
  virtual bool hasCloudType() const { return _type != type::undefined; }
  virtual bool Temporary() const { return _tempo; }

private:
    cover _cover;
    int _alt;
    bool _tempo;
    type _type;
};

class PhenomImpl : public Metar::Phenom 
{
public:
  PhenomImpl(bool tempo,
#ifndef NO_SHARED_PTR
             vector<phenom>& p,
#else
             const phenom *p,
             unsigned int num_phenom,
#endif
             intensity i = intensity::NORMAL,
             bool blowing = false,
             bool freezing = false,
             bool drifting = false,
             bool vicinity = false,
             bool partial = false,
             bool shallow = false,
             bool patches = false)
#ifndef NO_SHARED_PTR
    : _phenoms(p)
#else
    : _num_phenom(num_phenom)
#endif
    , _intensity(i)
    , _blowing(blowing)
    , _freezing(freezing)
    , _drifting(drifting)
    , _vicinity(vicinity)
    , _partial(partial)
    , _shallow(shallow)
    , _patches(patches)
    , _tempo(tempo)
  {
#ifdef NO_SHARED_PTR
    for (unsigned int i = 0 ; i < _num_phenom ; i++)
    {
      _phenoms[i] = p[i];
    }
#endif
  }

  PhenomImpl(const Phenom&) = delete;
  PhenomImpl& operator=(const Phenom&) = delete;

  ~PhenomImpl() = default;

  size_t NumPhenom() const 
  { 
#ifdef NO_SHARED_PTR
    return _num_phenom;
#else
    return _phenoms.size();
#endif
  }

  virtual phenom
#ifndef NO_SHARED_PTR
  operator[](typename vector<Phenom>::size_type
#else
  operator[](unsigned int
#endif
                        idx) const
  {
    if (idx < NumPhenom())
    {
      return _phenoms[idx];
    }

    return phenom::NONE;
  }

  virtual intensity Intensity() const { return _intensity; }
  virtual bool Blowing() const { return _blowing; }
  virtual bool Freezing() const { return _freezing; }
  virtual bool Drifting() const { return _drifting; }
  virtual bool Vicinity() const { return _vicinity; }
  virtual bool Partial() const { return _partial; }
  virtual bool Shallow() const { return _shallow; }
  virtual bool Patches() const { return _patches; }
  virtual bool Temporary() const { return _tempo; }

private:
#ifndef NO_SHARED_PTR
  vector<phenom> _phenoms;
#else
  phenom _phenoms[4];
  size_t _num_phenom;
#endif
  intensity _intensity;
  bool _blowing;
  bool _freezing;
  bool _drifting;
  bool _vicinity;
  bool _shower;
  bool _partial;
  bool _shallow;
  bool _patches;
  bool _tempo;
};

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
  _layers = new SkyCondition*[_MAX_CLOUD_LAYERS];
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

static bool match(const char *pattern, const char *str,
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

static inline bool match(const char *pattern, const char *str)
{
  return match(pattern, str, [](size_t a, size_t b) { return a == b; });
}  

static inline bool starts_with(const char *pattern, const char *str)
{
  return match(pattern, str, [](size_t a, size_t b) { return a <= b; });
}  

static inline bool is_message_type(const char *str)
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
  return starts_with("#####", str) 
      || starts_with("#####G##", str) 
      || starts_with("######G###", str)
      || starts_with("VRB", str);
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

static inline bool is_cloud_layer(const char *str)
{
  for (size_t i = 0 ; i < NUM_LAYERS ; i++)
  {
    if (starts_with(sky_conditions[i], str))
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
    || match("M##/M##", str)
    || match("##/", str)
    || match("M##/", str);
}

static inline bool is_altA(const char *str)
{
  return match("A####", str);
}

static inline bool is_altQ(const char *str)
{
  return match("Q####", str);
}

static inline bool is_rmk(const char *str)
{
  return strcmp(str, "RMK") == 0;
}

static inline bool is_tempo(const char *str)
{
  return strcmp(str, "TEMPO") == 0;
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
    else if (is_cloud_layer(el))
    {
      parse_cloud_layer(el);
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
  for (size_t i = 0 ; i < NUM_LAYERS ; i++)
  {
    if (starts_with(sky_conditions[i], str))
    {
      if (str[3] == '\0')
      {
#ifndef NO_SHARED_PTR
        _layers.push_back(
            make_shared<SkyConditionImpl>(_tempo,
                static_cast<SkyCondition::cover>(i)));
#else
        _layers[_num_layers++] =
            new SkyConditionImpl(_tempo,
                static_cast<SkyCondition::cover>(i)); 
#endif
      }
      else if (str[6] == '\0')
      {
#ifndef NO_SHARED_PTR
        _layers.push_back(
            make_shared<SkyConditionImpl>(_tempo,
                static_cast<SkyCondition::cover>(i), atoi(str + 3) * 100));
#else
        _layers[_num_layers++] =
            new SkyConditionImpl(_tempo, static_cast<SkyCondition::cover>(i), 
                                 atoi(str + 3) * 100);
#endif
      }
      else
      {
        SkyCondition::type t = SkyCondition::type::undefined;
        for (size_t j = 0 ; j < NUM_CLOUDS ; j++)
        {
          if (!strcmp(str + 6, cloud_types[j]))
          {
            t = static_cast<SkyCondition::type>(j);
            break;
          }
        } 
#ifndef NO_SHARED_PTR
        _layers.push_back(
            make_shared<SkyConditionImpl>(_tempo,
                static_cast<SkyCondition::cover>(i), atoi(str + 3) * 100, t));
#else
        _layers[_num_layers++] =
            new SkyConditionImpl(_temp, static_cast<SkyCondition::cover>(i), 
                                 atoi(str + 3) * 100, t);
#endif
      }
    }
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
#ifndef NO_SHARED_PTR
  vector<Metar::Phenom::phenom> p;
#else
  unsigned int idx = 0;
  Metar::Phenom::phenom p[4];
#endif
  Metar::Phenom::intensity inten = Metar::Phenom::intensity::NORMAL;
  bool blowing = false;
  bool freezing = false;
  bool drifting = false;
  bool vicinity = false;
  bool partial = false;
  bool shallow = false;
  bool patches = false;

  if (!isalpha(str[0]))
  {
    switch(str[0])
    {
      case '-':
        inten =  Metar::Phenom::intensity::LIGHT;
        break;

      case '+':
        inten =  Metar::Phenom::intensity::HEAVY;
        break;

      default:
        return;
    }
    str++;
  }

  if (!starts_with("$$", str))
  {
    return;
  }
  
  while (strlen(str) > 1)
  {
    if (!strncmp(str, "VC", 2))
    {
      vicinity = true;
    }
    else if (!strncmp(str, "BL", 2))
    {
      blowing = true;
    }
    else if (!strncmp(str, "DR", 2))
    {
      drifting = true;
    }
    else if (!strncmp(str, "FZ", 2))
    {
      freezing = true;
    }
    else if (!strncmp(str, "PR", 2))
    {
      partial = true;
    }
    else if (!strncmp(str, "MI", 2))
    {
      shallow = true;
    }
    else if (!strncmp(str, "BC", 2))
    {
      patches = true;
    }
    else if (!strncmp(str, TS, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::THUNDER_STORM);
#else
      p[idx++] = Metar::Phenom::phenom::THUNDER_STORM;
#endif
    }
    else if (!strncmp(str, SH, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SHOWER);
#else
      p[idx++] = Metar::Phenom::phenom::SHOWER;
#endif
    }
    else if (!strncmp(str, BR, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::MIST);
#else
      p[idx++] = Metar::Phenom::phenom::MIST;
#endif
    }
    else if (!strncmp(str, DS, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::DUST_STORM);
#else
      p[idx++] = Metar::Phenom::phenom::DUST_STORM;
#endif
    }
    else if (!strncmp(str, DU, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::DUST);
#else
      p[idx++] = Metar::Phenom::phenom::DUST;
#endif
    }
    else if (!strncmp(str, DZ, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::DRIZZLE);
#else
      p[idx++] = Metar::Phenom::phenom::DRIZZLE;
#endif
    }
    else if (!strncmp(str, FC, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::FUNNEL_CLOUD);
#else
      p[idx++] = Metar::Phenom::phenom::FUNNEL_CLOUD;
#endif
    }
    else if (!strncmp(str, FG, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::FOG);
#else
      p[idx++] = Metar::Phenom::phenom::FOG;
#endif
    }
    else if (!strncmp(str, FU, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SMOKE);
#else
      p[idx++] = Metar::Phenom::phenom::SMOKE;
#endif
    }
    else if (!strncmp(str, GR, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::HAIL);
#else
      p[idx++] = Metar::Phenom::phenom::HAIL;
#endif
    }
    else if (!strncmp(str, GS, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SMALL_HAIL);
#else
      p[idx++] = Metar::Phenom::phenom::SMALL_HAIL;
#endif
    }
    else if (!strncmp(str, HZ, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::HAZE);
#else
      p[idx++] = Metar::Phenom::phenom::HAZE;
#endif
    }
    else if (!strncmp(str, IC, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::ICE_CRYSTALS);
#else
      p[idx++] = Metar::Phenom::phenom::ICE_CRYSTALS;
#endif
    }
    else if (!strncmp(str, PE, 2) || !strncmp(str, PL, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::ICE_PELLETS);
#else
      p[idx++] = Metar::Phenom::phenom::ICE_PELLETS;
#endif
    }
    else if (!strncmp(str, PO, 2)) 
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::DUST_SAND_WHORLS);
#else
      p[idx++] = Metar::Phenom::phenom::DUST_SAND_WHORLS;
#endif
    }
    else if (!strncmp(str, PY, 2)) 
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SPRAY);
#else
      p[idx++] = Metar::Phenom::phenom::SPRAY;
#endif
    }
    else if (!strncmp(str, RA, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::RAIN);
#else
      p[idx++] = Metar::Phenom::phenom::RAIN;
#endif
    }
    else if (!strncmp(str, SA, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SAND);
#else
      p[idx++] = Metar::Phenom::phenom::SAND;
#endif
    }
    else if (!strncmp(str, SG, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SNOW_GRAINS);
#else
      p[idx++] = Metar::Phenom::phenom::SNOW_GRAINS;
#endif
    }
    else if (!strncmp(str, SN, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SNOW);
#else
      p[idx++] = Metar::Phenom::phenom::SNOW;
#endif
    }
    else if (!strncmp(str, SQ, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SQUALLS);
#else
      p[idx++] = Metar::Phenom::phenom::SQUALLS;
#endif
    }
    else if (!strncmp(str, SS, 2)) 
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::SAND_STORM);
#else
      p[idx++] = Metar::Phenom::phenom::SAND_STORM;
#endif
    }
    else if (!strncmp(str, UP, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::UNKNOWN_PRECIP);
#else
      p[idx++] = Metar::Phenom::phenom::UNKNOWN_PRECIP;
#endif
    }
    else if (!strncmp(str, VA, 2))
    {
#ifndef NO_SHARED_PTR
      p.push_back(Metar::Phenom::phenom::VOLCANIC_ASH);
#else
      p[idx++] = Metar::Phenom::phenom::VOLCANIC_ASH;
#endif
    }
    str += 2;
  }

#ifndef NO_SHARED_PTR
  if (p.size() > 0)
  {
    _phenomena.push_back(make_shared<PhenomImpl>(_tempo,
                                       p,
                                       inten,
                                       blowing,
                                       freezing,
                                       drifting,
                                       vicinity,
                                       partial,
                                       shallow,
                                       patches));
#else
  if (idx > 0)
  {
    _phenomena[_num_phenomena++] = new PhenomImpl(_temp,
                                                 p,
                                                 idx,
                                                 inten,
                                                 blowing,
                                                 freezing,
                                                 drifting,
                                                 vicinity,
                                                 partial,
                                                 shallow,
                                                 patches);
#endif
  }
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
