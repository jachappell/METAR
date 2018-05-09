//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder
//

#ifndef __METAR_H__
#define __METAR_H__

#ifdef ARDUINO
#define NO_SHARED_PTR
#endif

#ifndef NO_SHARED_PTR
#include <memory>
#include <vector>
#endif

namespace Storage_B
{
  namespace Weather
  {
    class Metar
    {
    public:
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

      //
      // Constructor
      //    metar_str - METAR to decode
      //
      Metar(const char *metar_str);

      //
      // Constructor
      //    metar_str - METAR to decode
      //
      Metar(char *metar_str);

#ifdef NO_SHARED_PTR
      ~Metar();
#else
      ~Metar() = default;
#endif

      // no copy
      Metar(const Metar&) = delete;
      Metar& operator=(const Metar&) = delete;

      //
      // Message type: METAR or SPECI
      //
      message_type MessageType() const { return _message_type; }
      bool hasMessageType() const
      {
        return _message_type != message_type::undefined;
      }

      //
      // Location identifier
      //
      const char *ICAO() const { return _icao; }
      bool hasICAO() const { return _icao[0] != '\0'; }

      //
      // Observation day of month (UTC)
      //
      int Day() const { return _day; }
      bool hasDay() const { return _day != _INTEGER_UNDEFINED; }

      //
      //  Hour of observation (UTC)
      //
      int Hour() const { return _hour; }
      bool hasHour() const { return _hour != _INTEGER_UNDEFINED; }

      //
      //  Minute of observation (UTC)
      //
      int Minute() const { return _min; }
      bool hasMinute() const { return _min != _INTEGER_UNDEFINED; }

      //
      //  Wind direction
      //
      int WindDirection() const { return _wind_dir; }
      bool hasWindDirection() const { return _wind_dir != _INTEGER_UNDEFINED; }

      //
      // Variable wind direction
      //    If true, wind direction is variable (hasWindDirection() will 
      //    return false in this case)
      bool isVariableWindDirection() const { return _vrb; }

      //
      //  Wind speed
      //
      int WindSpeed() const { return _wind_spd; }
      bool hasWindSpeed() const { return _wind_spd != _INTEGER_UNDEFINED; }

      //
      //  Wind gust
      //
      int WindGust() const { return _gust; }
      bool hasWindGust() const { return _gust != _INTEGER_UNDEFINED; }

      //
      // Minimum wind direction
      //
      int MinWindDirection() const { return _min_wind_dir; }
      int hasMinWindDirection() const
        { return _min_wind_dir != _INTEGER_UNDEFINED ; }

      //
      // Maximum wind direction
      //
      int MaxWindDirection() const { return _max_wind_dir; }
      int hasMaxWindDirection() const
        { return _max_wind_dir != _INTEGER_UNDEFINED; }

      //
      // Wind speed units
      //    KT  - knots
      //    MPS - meters per second
      //    KPH - kilometers per hour
      //
      speed_units WindSpeedUnits() const { return _wind_speed_units; }
      int hasWindSpeedUnits() const
      {
        return _wind_speed_units != speed_units::undefined;
      }

      //
      // Visibility
      //
      double Visibility() const { return _vis; }
      bool hasVisibility() const { return _vis != _DOUBLE_UNDEFINED; }

      //
      // Visibility units
      //    SM - statute miles
      //    M  - meters
      distance_units VisibilityUnits() const { return _vis_units; }
      bool hasVisibilityUnits() const
      {
        return _vis_units != distance_units::undefined;
      }

      //
      // Visibility less than
      //    If true, visibility is less than reported value
      //
      bool isVisibilityLessThan() const { return _vis_lt; }
  
      //
      // CAVOK (Ceiling and Visibility OK)  
      //    If true, Ceiling and Visibility OK
      //
      bool isCAVOK() const { return _cavok; }

      //
      // Number of Cloud Layers
      //
      unsigned int NumCloudLayers() const
      { 
#ifdef NO_SHARED_PTR
        return _num_layers;
#else
        return _layers.size(); 
#endif
      }

      //
      // Vertical visibilty
      //    feet
      int VerticalVisibility() const { return _vert_vis; }
      bool hasVerticalVisibility() const
        { return _vert_vis != _INTEGER_UNDEFINED; }

      //
      // Temperature
      //    Celsius
      //
      int Temperature() const { return _temp; }
      bool hasTemperature() const { return _temp != _INTEGER_UNDEFINED; }

      //
      // Dew point
      //    Celsius
      // 
      int DewPoint() const { return _dew; }
      bool hasDewPoint() const { return _dew != _INTEGER_UNDEFINED; }

      //
      // Altimeter setting
      //    inHg
      double AltimeterA() const { return _altimeterA; }
      bool hasAltimeterA() const { return _altimeterA != _DOUBLE_UNDEFINED; }

      //
      // Current altimeter setting (in QNH)
      //    hPa
      int AltimeterQ() const { return _altimeterQ; }
      bool hasAltimeterQ() const { return _altimeterQ != _INTEGER_UNDEFINED; }

      //
      // Barometric pressure extrapolated to sea level 
      //    hPa
      double SeaLevelPressure() const { return _slp; }
      bool hasSeaLevelPressure() const { return _slp != _DOUBLE_UNDEFINED; }

      //
      // Temperature
      //    Celsius
      //
      double TemperatureNA() const { return _ftemp; }
      bool hasTemperatureNA() const { return _ftemp != _DOUBLE_UNDEFINED; }

      //
      // Dew point
      //    Celsius
      // 
      double DewPointNA() const { return _fdew; }
      bool hasDewPointNA() const { return _fdew != _DOUBLE_UNDEFINED; }

      class SkyCondition
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

        virtual ~SkyCondition() = default;

        virtual cover Cover() const = 0;
        virtual int Altitude() const = 0;
        virtual bool hasAltitude() const = 0;
        virtual type CloudType() const = 0;
        virtual bool hasCloudType() const = 0;
      };

#ifndef NO_SHARED_PTR
      std::shared_ptr<SkyCondition>
#else
      SkyCondition *
#endif
      Layer(unsigned int idx) const
      {
        if (idx < NumCloudLayers())
        {
          return _layers[idx];
        }

        return nullptr;
      }

    private:
      Metar();

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
      std::vector<std::shared_ptr<SkyCondition>>
#else
      SkyCondition **
#endif
      _layers;

#ifdef NO_SHARED_PTR
      unsigned int _num_layers;
#endif

      int _vert_vis;

      int _temp;
      int _dew;

      double _altimeterA;

      int _altimeterQ;

      double _slp;

      double _ftemp;
      double _fdew;

      const char *_previous_element;

      static const int _INTEGER_UNDEFINED;
      static const double _DOUBLE_UNDEFINED;
#ifdef NO_SHARED_PTR
      static const unsigned int _MAX_CLOUD_LAYERS;
#endif
    };
  }
}

#endif
