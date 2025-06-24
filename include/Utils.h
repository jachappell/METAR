//
// Copyright (c) 2020 James A. Chappell
//
// Weather utilities
//

#pragma once

namespace Storage_B 
{
  namespace Weather
  {
    /**
     * @class Utils
     * @brief A utility class providing static methods for weather-related calculations.
     *
     * This class provides static methods to calculate humidity, wind chill, and heat index
     * based on provided temperature, dew point, wind speed, and humidity levels.
     * The Utils class cannot be instantiated or copied.
     */
    class Utils
    {
    public:
      /**
       * @brief Calculates the relative humidity based on temperature and dew point.
       *
       * This method computes the relative humidity percentage by using the provided
       * temperature and dew point values. It utilizes the formula derived from
       * the Clausius-Clapeyron equation for approximating saturation vapor pressure.
       *
       * @param t The air temperature in Celsius.
       * @param td The dew point temperature in Celsius.
       * @return The relative humidity as a percentage.
       */
      static double Humidity(double t, double td);

      /**
       * @brief Calculates the wind chill temperature based on ambient temperature and wind speed.
       *
       * This method computes the wind chill effect, which represents the perceived decrease in
       * temperature felt by a person due to the combined effect of low temperature and wind.
       * The calculation is applied only if the wind speed is above 4.8 km/h and the temperature
       * is 10°C or below. If these conditions are not met, the ambient temperature is returned.
       *
       * @param temp The ambient temperature in degrees Celsius.
       * @param wind_speed The wind speed in kilometers per hour.
       * @return The effective wind chill temperature in degrees Celsius, calculated based on
       *         the input parameters.
       */
      static double WindChill(double temp, double wind_speed);

      /**
       * @brief Calculates the heat index based on temperature and relative humidity.
       *
       * The heat index, also known as the "apparent temperature," is a measure of how hot it feels
       * when accounted for both the temperature and the relative humidity. The calculation uses the
       * heat index formula, applying additional adjustments when the input conditions are within specific ranges.
       * If the temperature is provided in Celsius, it will be automatically converted to Fahrenheit
       * for computation and then converted back to Celsius if the `celsius_flg` flag is set.
       *
       * @param temp The ambient temperature. Can be in Celsius or Fahrenheit based on `celsius_flg`.
       * @param humidity The relative humidity as a percentage (0-100).
       * @param celsius_flg A boolean flag indicating whether the input temperature is in Celsius (true) or Fahrenheit (false).
       * @return The computed heat index in the same unit (Celsius or Fahrenheit) as specified by `celsius_flg`.
       */
      static double HeatIndex(
          double temp, 
          double humidity,
          bool celsius_flg = true);

      Utils() = delete;
      Utils(const Utils&) = delete;
      Utils& operator=(const Utils&) = delete;
      ~Utils() = default;
    };
  }
}
