//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR weather phenomena decoder
//
#pragma once

#include <memory>
#include <vector>

namespace Storage_B
{
  namespace Weather
  {
    class Phenom
    {
    public:
      enum class phenom
      {
        NONE,
        MIST,                   // BR
        DUST_STORM,             // DS
        DUST,                   // DU
        DRIZZLE,                // DZ
        FUNNEL_CLOUD,           // FC
        FOG,                    // FG 
        SMOKE,                  // FU
        HAIL,                   // GR
        SMALL_HAIL,             // GS
        HAZE,                   // HZ
        ICE_CRYSTALS,           // IC
        ICE_PELLETS,            // PE
        DUST_SAND_WHORLS,       // PO
        SPRAY,                  // PY
        RAIN,                   // RA
        SAND,                   // SA
        SNOW_GRAINS,            // SG
        SHOWER,                 // SH
        SNOW,                   // SN
        SQUALLS,                // SQ
        SAND_STORM,             // SS
        UNKNOWN_PRECIP,         // UP
        VOLCANIC_ASH,           // VA
      };

      enum class intensity
      {
        LIGHT = -1,
        NORMAL,
        HEAVY
      };

      static std::shared_ptr<Phenom> Create(const char *str, bool temp = false);

      virtual ~Phenom() = default;

      virtual unsigned int NumPhenom() const = 0;

      virtual phenom

      operator[](typename std::vector<Phenom>::size_type idx) const = 0;
        
      virtual intensity Intensity() const = 0;
      virtual bool Blowing() const = 0;
      virtual bool Freezing() const = 0;
      virtual bool Drifting() const = 0;
      virtual bool Vicinity() const = 0;
      virtual bool Partial() const = 0;
      virtual bool Shallow() const = 0;
      virtual bool Patches() const = 0;
      virtual bool ThunderStorm() const = 0;
      virtual bool Temporary() const = 0;
    };
  }
}
