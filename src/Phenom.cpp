//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR weather phenomena decoder
//
#include "Phenom.h"

#include <cstring>
#include <cctype>

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
             std::vector<phenom>& p,
             intensity i = intensity::NORMAL,
             bool blowing = false,
             bool freezing = false,
             bool drifting = false,
             bool vicinity = false,
             bool partial = false,
             bool shallow = false,
             bool patches = false,
             bool ts = false)
    : _phenoms(p)
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
  }

  PhenomImpl(const PhenomImpl&) = delete;
  PhenomImpl& operator=(const PhenomImpl&) = delete;

  ~PhenomImpl() = default;

  unsigned int NumPhenom() const 
  { 
    return _phenoms.size();
  }

  virtual phenom operator[](typename std::vector<Phenom>::size_type
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
  std::vector<phenom> _phenoms;
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

std::shared_ptr<Phenom> Phenom::Create(const char *str, bool tempo)
{
  std::vector<Phenom::phenom> p;
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
      p.push_back(Phenom::phenom::SHOWER);
    }
    else if (!strncmp(str, BR, 2))
    {
      p.push_back(Phenom::phenom::MIST);
    }
    else if (!strncmp(str, DS, 2))
    {
      p.push_back(Phenom::phenom::DUST_STORM);
    }
    else if (!strncmp(str, DU, 2))
    {
      p.push_back(Phenom::phenom::DUST);
    }
    else if (!strncmp(str, DZ, 2))
    {
      p.push_back(Phenom::phenom::DRIZZLE);
    }
    else if (!strncmp(str, FC, 2))
    {
      p.push_back(Phenom::phenom::FUNNEL_CLOUD);
    }
    else if (!strncmp(str, FG, 2))
    {
      p.push_back(Phenom::phenom::FOG);
    }
    else if (!strncmp(str, FU, 2))
    {
      p.push_back(Phenom::phenom::SMOKE);
    }
    else if (!strncmp(str, GR, 2))
    {
      p.push_back(Phenom::phenom::HAIL);
    }
    else if (!strncmp(str, GS, 2))
    {
      p.push_back(Phenom::phenom::SMALL_HAIL);
    }
    else if (!strncmp(str, HZ, 2))
    {
      p.push_back(Phenom::phenom::HAZE);
    }
    else if (!strncmp(str, IC, 2))
    {
      p.push_back(Phenom::phenom::ICE_CRYSTALS);
    }
    else if (!strncmp(str, PE, 2) || !strncmp(str, PL, 2))
    {
      p.push_back(Phenom::phenom::ICE_PELLETS);
    }
    else if (!strncmp(str, PO, 2)) 
    {
      p.push_back(Phenom::phenom::DUST_SAND_WHORLS);
    }
    else if (!strncmp(str, PY, 2)) 
    {
      p.push_back(Phenom::phenom::SPRAY);
    }
    else if (!strncmp(str, RA, 2))
    {
      p.push_back(Phenom::phenom::RAIN);
    }
    else if (!strncmp(str, SA, 2))
    {
      p.push_back(Phenom::phenom::SAND);
    }
    else if (!strncmp(str, SG, 2))
    {
      p.push_back(Phenom::phenom::SNOW_GRAINS);
    }
    else if (!strncmp(str, SN, 2))
    {
      p.push_back(Phenom::phenom::SNOW);
    }
    else if (!strncmp(str, SQ, 2))
    {
      p.push_back(Phenom::phenom::SQUALLS);
    }
    else if (!strncmp(str, SS, 2)) 
    {
      p.push_back(Phenom::phenom::SAND_STORM);
    }
    else if (!strncmp(str, UP, 2))
    {
      p.push_back(Phenom::phenom::UNKNOWN_PRECIP);
    }
    else if (!strncmp(str, VA, 2))
    {
      p.push_back(Phenom::phenom::VOLCANIC_ASH);
    }
    str += 2;
  }

  if (
      (p.size() > 0)
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
    return std::make_shared<PhenomImpl>(tempo,
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
  }

  return nullptr;
}
