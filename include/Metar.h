//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#ifndef STORAGE_B_WEATHER_METAR_H_
#define STORAGE_B_WEATHER_METAR_H_

#include "defines.h"

#ifndef NO_STD
#include <memory>
#include <vector>
#endif

#ifndef NO_PHENOM
#include "Phenom.h"
#endif

#ifndef NO_CLOUDS
#include "Clouds.h"
#endif

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
      static
#ifndef NO_STD
        std::shared_ptr<Metar>
#else
        Metar * // caller is responsible for deleting
#endif
          Create(const char *metar_str);

      //
      // Static Creator
      //    metar_str - METAR to decode
      //
      static
#ifndef NO_STD
        std::shared_ptr<Metar>
#else
        Metar *  // caller is responsible for deleting
#endif
          Create(char *metar_str);
      
      enum class message_type
      {
        undefined = -1,
        METAR,
        SPECI
      };

      enum class speed_units
      {
        undefined = -1,
        KT,  // knots
        MPS, // meters per second
        KPH  // kilometer per hour
      };

      enum class distance_units
      {
        undefined = -1,
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
      virtual message_type MessageType() const = 0;
      virtual bool hasMessageType() const = 0;

      //
      // Location identifier
      //
      virtual const char *ICAO() const = 0;
      virtual bool hasICAO() const = 0;

      //
      // Observation day of month (UTC)
      //
      virtual int Day() const = 0;
      virtual bool hasDay() const = 0;

      //
      //  Hour of observation (UTC)
      //
      virtual int Hour() const = 0;
      virtual bool hasHour() const = 0;

      //
      //  Minute of observation (UTC)
      //
      virtual int Minute() const = 0;
      virtual bool hasMinute() const = 0;

      //
      //  Wind direction
      //
      virtual int WindDirection() const = 0;
      virtual bool hasWindDirection() const = 0;

      //
      // Variable wind direction
      //    If true, wind direction is variable (hasWindDirection() will 
      //    return false in this case)
      virtual bool isVariableWindDirection() const = 0;

      //
      //  Wind speed
      //
      virtual int WindSpeed() const = 0;
      virtual bool hasWindSpeed() const = 0;

      //
      //  Wind gust
      //
      virtual int WindGust() const = 0;
      virtual bool hasWindGust() const = 0;

      //
      // Minimum wind direction
      //
      virtual int MinWindDirection() const = 0;
      virtual int hasMinWindDirection() const = 0;

      //
      // Maximum wind direction
      //
      virtual int MaxWindDirection() const = 0;
      virtual int hasMaxWindDirection() const = 0;

      //
      // Wind speed units
      //    KT  - knots
      //    MPS - meters per second
      //    KPH - kilometers per hour
      //
      virtual speed_units WindSpeedUnits() const = 0;
      virtual int hasWindSpeedUnits() const = 0;

      //
      // Visibility
      //
      virtual double Visibility() const = 0;
      virtual bool hasVisibility() const = 0;

      //
      // Visibility units
      //    SM - statute miles
      //    M  - meters
      virtual distance_units VisibilityUnits() const = 0;
      virtual bool hasVisibilityUnits() const = 0;

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
      virtual int VerticalVisibility() const = 0;
      virtual bool hasVerticalVisibility() const = 0;

      //
      // Temperature
      //    Celsius
      //
      virtual int Temperature() const = 0;
      virtual bool hasTemperature() const = 0;

      //
      // Dew point
      //    Celsius
      // 
      virtual int DewPoint() const = 0;
      virtual bool hasDewPoint() const = 0;

      //
      // Altimeter setting
      //    inHg
      virtual double AltimeterA() const = 0;
      virtual bool hasAltimeterA() const = 0;

      //
      // Current altimeter setting (in QNH)
      //    hPa
      virtual int AltimeterQ() const = 0;
      virtual bool hasAltimeterQ() const = 0;

      //
      // Barometric pressure extrapolated to sea level 
      //    hPa
      virtual double SeaLevelPressure() const = 0;
      virtual bool hasSeaLevelPressure() const = 0;

      //
      // Temperature
      //    Celsius
      //
      virtual double TemperatureNA() const = 0;
      virtual bool hasTemperatureNA() const = 0;

      //
      // Dew point
      //    Celsius
      // 
      virtual double DewPointNA() const = 0;
      virtual bool hasDewPointNA() const = 0;

#ifndef NO_CLOUDS
      //
      // Number of Cloud Layers
      //
      virtual unsigned int NumCloudLayers() const = 0;

      virtual
#ifndef NO_STD
        std::shared_ptr<Clouds>
#else
        const Clouds *
#endif
          Layer(unsigned int idx) const = 0;
#endif

#ifndef NO_PHENOM
      virtual unsigned int NumPhenomena() const = 0;

      virtual const Phenom& Phenomenon(unsigned int idx) const = 0;
#endif
    };
  }
}

#endif
