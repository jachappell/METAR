//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR weather phenomena decoder
//
#include "Phenom.h"

#ifndef NO_STD
#include <cstring>
#include <cctype>
#else
#include <string.h>
#include <ctype.h>
#endif

using namespace std;
using namespace Storage_B::Weather;

namespace
{
  const char *BR = "BR";
  const char *DS = "DS";
  const char *DU = "DU";
  const char *DZ = "DZ";
  const char *FC = "FC";
  const char *FG = "FG";
  const char *FU = "FU";
  const char *GR = "GR";
  const char *GS = "GS";
  const char *HZ = "HZ";
  const char *IC = "IC";
  const char *PE = "PE";
  const char *PL = "PL"; 
  const char *PO = "PO"; 
  const char *PY = "PY";
  const char *RA = "RA";
  const char *SA = "SA";
  const char *SG = "SG";
  const char *SH = "SH";
  const char *SN = "SN";
  const char *SQ = "SQ";
  const char *SS = "SS"; 
  const char *TS = "TS";
  const char *UP = "UP";
  const char *VA = "VA";
}

class PhenomImpl : public Phenom 
{
public:
  PhenomImpl(bool tempo,
#ifndef NO_STD
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
             bool patches = false,
             bool ts = false)
#ifndef NO_STD
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
    , _ts(ts)
    , _tempo(tempo)
  {
#ifdef NO_STD
    for (unsigned int i = 0 ; i < _num_phenom ; i++)
    {
      _phenoms[i] = p[i];
    }
#endif
  }

  PhenomImpl(const PhenomImpl&) = delete;
  PhenomImpl& operator=(const PhenomImpl&) = delete;

  ~PhenomImpl() = default;

  unsigned int NumPhenom() const 
  { 
#ifdef NO_STD
    return _num_phenom;
#else
    return _phenoms.size();
#endif
  }

  virtual phenom
#ifndef NO_STD
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
  virtual bool ThunderStorm() const { return _ts; }
  virtual bool Temporary() const { return _tempo; }

private:
#ifndef NO_STD
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
  bool _ts;
  bool _tempo;
};

#ifndef NO_STD
          std::shared_ptr<Phenom>
#else
          Phenom *
#endif
Phenom::Create(const char *str, bool tempo)
{
#ifndef NO_STD
  vector<Phenom::phenom> p;
#else
  unsigned int idx = 0;
  Phenom::phenom p[4];
#endif
  Phenom::intensity inten = Phenom::intensity::NORMAL;
  bool blowing = false;
  bool freezing = false;
  bool drifting = false;
  bool vicinity = false;
  bool partial = false;
  bool shallow = false;
  bool patches = false;
  bool ts = false;

  if (!isalpha(str[0]))
  {
    switch(str[0])
    {
      case '-':
        inten =  Phenom::intensity::LIGHT;
        break;

      case '+':
        inten =  Phenom::intensity::HEAVY;
        break;

      default:
        return nullptr;
    }
    str++;
  }

  if (!isalpha(str[0]) || !isalpha(str[1]))
  {
    return nullptr;
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
      ts = true;
    }
    else if (!strncmp(str, SH, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SHOWER);
#else
      p[idx++] = Phenom::phenom::SHOWER;
#endif
    }
    else if (!strncmp(str, BR, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::MIST);
#else
      p[idx++] = Phenom::phenom::MIST;
#endif
    }
    else if (!strncmp(str, DS, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::DUST_STORM);
#else
      p[idx++] = Phenom::phenom::DUST_STORM;
#endif
    }
    else if (!strncmp(str, DU, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::DUST);
#else
      p[idx++] = Phenom::phenom::DUST;
#endif
    }
    else if (!strncmp(str, DZ, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::DRIZZLE);
#else
      p[idx++] = Phenom::phenom::DRIZZLE;
#endif
    }
    else if (!strncmp(str, FC, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::FUNNEL_CLOUD);
#else
      p[idx++] = Phenom::phenom::FUNNEL_CLOUD;
#endif
    }
    else if (!strncmp(str, FG, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::FOG);
#else
      p[idx++] = Phenom::phenom::FOG;
#endif
    }
    else if (!strncmp(str, FU, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SMOKE);
#else
      p[idx++] = Phenom::phenom::SMOKE;
#endif
    }
    else if (!strncmp(str, GR, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::HAIL);
#else
      p[idx++] = Phenom::phenom::HAIL;
#endif
    }
    else if (!strncmp(str, GS, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SMALL_HAIL);
#else
      p[idx++] = Phenom::phenom::SMALL_HAIL;
#endif
    }
    else if (!strncmp(str, HZ, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::HAZE);
#else
      p[idx++] = Phenom::phenom::HAZE;
#endif
    }
    else if (!strncmp(str, IC, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::ICE_CRYSTALS);
#else
      p[idx++] = Phenom::phenom::ICE_CRYSTALS;
#endif
    }
    else if (!strncmp(str, PE, 2) || !strncmp(str, PL, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::ICE_PELLETS);
#else
      p[idx++] = Phenom::phenom::ICE_PELLETS;
#endif
    }
    else if (!strncmp(str, PO, 2)) 
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::DUST_SAND_WHORLS);
#else
      p[idx++] = Phenom::phenom::DUST_SAND_WHORLS;
#endif
    }
    else if (!strncmp(str, PY, 2)) 
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SPRAY);
#else
      p[idx++] = Phenom::phenom::SPRAY;
#endif
    }
    else if (!strncmp(str, RA, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::RAIN);
#else
      p[idx++] = Phenom::phenom::RAIN;
#endif
    }
    else if (!strncmp(str, SA, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SAND);
#else
      p[idx++] = Phenom::phenom::SAND;
#endif
    }
    else if (!strncmp(str, SG, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SNOW_GRAINS);
#else
      p[idx++] = Phenom::phenom::SNOW_GRAINS;
#endif
    }
    else if (!strncmp(str, SN, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SNOW);
#else
      p[idx++] = Phenom::phenom::SNOW;
#endif
    }
    else if (!strncmp(str, SQ, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SQUALLS);
#else
      p[idx++] = Phenom::phenom::SQUALLS;
#endif
    }
    else if (!strncmp(str, SS, 2)) 
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::SAND_STORM);
#else
      p[idx++] = Phenom::phenom::SAND_STORM;
#endif
    }
    else if (!strncmp(str, UP, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::UNKNOWN_PRECIP);
#else
      p[idx++] = Phenom::phenom::UNKNOWN_PRECIP;
#endif
    }
    else if (!strncmp(str, VA, 2))
    {
#ifndef NO_STD
      p.push_back(Phenom::phenom::VOLCANIC_ASH);
#else
      p[idx++] = Phenom::phenom::VOLCANIC_ASH;
#endif
    }
    str += 2;
  }

  if (
#ifndef NO_STD
      (p.size() > 0)
#else
      (idx > 0)
#endif
      || blowing
      || freezing
      || drifting
      || vicinity
      || partial
      || shallow
      || patches
      || ts
      )
  {
#ifndef NO_STD
    return make_shared<PhenomImpl>(tempo,
                                   p,
                                   inten,
                                   blowing,
                                   freezing,
                                   drifting,
                                   vicinity,
                                   partial,
                                   shallow,
                                   patches,
                                   ts);
#else
    return new PhenomImpl(tempo,
                          p,
                          idx,
                          inten,
                          blowing,
                          freezing,
                          drifting,
                          vicinity,
                          partial,
                          shallow,
                          patches,
                          ts);
#endif
  }

  return nullptr;
}
