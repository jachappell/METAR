//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR clouds decoder
//

#ifndef __STORAGE_B_WEATHER_CLOUDS_H__
#define __STORAGE_B_WEATHER_CLOUDS_H__

#include "defines.h"

#ifndef NO_SHARED_PTR
#include <memory>
#endif

namespace Storage_B
{
  namespace Weather
  {
    class Clouds
    {
    public:
      enum class cover
      {
        SKC,
        CLR,
        NSC,
        FEW,
        SCT,
        BKN,
        OVC
      };

      enum class type
      {
        undefined = -1,
        TCU,
        CB,
        ACC 
      };

      static
#ifndef NO_SHARED_PTR
          std::shared_ptr<Clouds>
#else
          Clouds *
#endif
              Create(const char *str, bool temp);

      virtual ~Clouds() = default;

      virtual cover Cover() const = 0;
      virtual int Altitude() const = 0;
      virtual bool hasAltitude() const = 0;
      virtual type CloudType() const = 0;
      virtual bool hasCloudType() const = 0;
      virtual bool Temporary() const = 0;
    };
  }
}

#endif
