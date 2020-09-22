//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR clouds decoder
//

#pragma once

#include <memory>
#include <optional>

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
        TCU,
        CB,
        ACC 
      };

      static std::shared_ptr<Clouds> Create(const char *str, bool temp = false);

      virtual ~Clouds() = default;

      virtual cover Cover() const = 0;

      //
      // Layer altitude in multiples of 100 feet or 30 meters
      //
      virtual std::optional<int> Altitude() const = 0;

      virtual std::optional<type> CloudType() const = 0;

      virtual bool Temporary() const = 0;
    };
  }
}
