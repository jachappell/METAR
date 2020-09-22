//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#pragma once

#include <memory>
#include <vector>
#include <optional>

#include "Phenom.h"
#include "Clouds.h"

namespace Storage_B
{
  namespace Weather
  {
    class Metar
    {
    public:
      //
      // Static Creator
      //    metar_str - METAR to decode
      //
      static std::shared_ptr<Metar> Create(const char *metar_str);

      //
      // Static Creator
      //    metar_str - METAR to decode
      //
      static std::shared_ptr<Metar> Create(char *metar_str);
      
      enum class message_type
      {
        METAR,
        SPECI
      };

      enum class speed_units
      {
        KT,  // knots
        MPS, // meters per second
        KPH  // kilometer per hour
      };

      enum class distance_units
      {
        M,  // meters
        SM  // statute miles
      };

      Metar() = default;

      virtual ~Metar() = default;

      // no copy
      Metar(const Metar&) = delete;
      Metar& operator=(const Metar&) = delete;

      //
      // Message type: METAR or SPECI
      //
      virtual std::optional<message_type> MessageType() const = 0;

      //
      // Location identifier
      //
      virtual std::optional<std::string> ICAO() const = 0;

      //
      // Observation day of month (UTC)
      //
      virtual std::optional<int> Day() const = 0;

      //
      //  Hour of observation (UTC)
      //
      virtual std::optional<int> Hour() const = 0;

      //
      //  Minute of observation (UTC)
      //
      virtual std::optional<int> Minute() const = 0;

      //
      //  Wind direction
      //
      virtual std::optional<int> WindDirection() const = 0;

      //
      // Variable wind direction
      //    If true, wind direction is variable (hasWindDirection() will 
      //    return false in this case)
      virtual bool isVariableWindDirection() const = 0;

      //
      //  Wind speed
      //
      virtual std::optional<int> WindSpeed() const = 0;

      //
      //  Wind gust
      //
      virtual std::optional<int> WindGust() const = 0;

      //
      // Minimum wind direction
      //
      virtual std::optional<int> MinWindDirection() const = 0;

      //
      // Maximum wind direction
      //
      virtual std::optional<int> MaxWindDirection() const = 0;

      //
      // Wind speed units
      //    KT  - knots
      //    MPS - meters per second
      //    KPH - kilometers per hour
      //
      virtual std::optional<speed_units> WindSpeedUnits() const = 0;

      //
      // Visibility
      //
      virtual std::optional<double> Visibility() const = 0;

      //
      // Visibility units
      //    SM - statute miles
      //    M  - meters
      virtual std::optional<distance_units> VisibilityUnits() const = 0;

      //
      // Visibility less than
      //    If true, visibility is less than reported value
      //
      virtual bool isVisibilityLessThan() const = 0;
  
      //
      // CAVOK (Ceiling and Visibility OK)  
      //    If true, Ceiling and Visibility OK
      //
      virtual bool isCAVOK() const = 0;

      //
      // Vertical visibilty
      //    feet
      virtual std::optional<int> VerticalVisibility() const = 0;

      //
      // Temperature
      //    Celsius
      //
      virtual std::optional<int> Temperature() const = 0;

      //
      // Dew point
      //    Celsius
      // 
      virtual std::optional<int> DewPoint() const = 0;

      //
      // Altimeter setting
      //    inHg
      virtual std::optional<double> AltimeterA() const = 0;

      //
      // Current altimeter setting (in QNH)
      //    hPa
      virtual std::optional<int> AltimeterQ() const = 0;

      //
      // Barometric pressure extrapolated to sea level 
      //    hPa
      virtual std::optional<double> SeaLevelPressure() const = 0;

      //
      // Temperature
      //    Celsius
      //
      virtual std::optional<double> TemperatureNA() const = 0;

      //
      // Dew point
      //    Celsius
      // 
      virtual std::optional<double> DewPointNA() const = 0;

      //
      // Number of Cloud Layers
      //
      virtual unsigned int NumCloudLayers() const = 0;

      virtual std::shared_ptr<Clouds> Layer(unsigned int idx) const = 0;

      virtual unsigned int NumPhenomena() const = 0;

      virtual const Phenom& Phenomenon(unsigned int idx) const = 0;
    };
  }
}
