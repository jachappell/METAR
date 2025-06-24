//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// Units conversion
//

#pragma once

namespace Storage_B 
{
  namespace Weather
  {
	  /**
	   * @class Convert
	   * @brief A utility class for performing various unit conversions.
	   *
	   * The Convert class provides static methods for converting between commonly used units
	   * of measurement, including temperature, distance, pressure, and speed. The class is
	   * non-instantiable and contains only static utility methods.
	   *
	   * The class includes predefined conversion factors as constexpr private members,
	   * ensuring high precision and performance in all conversions. All methods
	   * take a specific input value in one unit and return the equivalent value in another unit.
	   */
  	class Convert
    {
    private:
		static constexpr double feetPerMeter = 3.28084;
		static constexpr double metersPerFoot = 1.0 / feetPerMeter;
		static constexpr double inchesPerCentimeter = 2.54;

		static constexpr double inHgPerMb = 0.02953;
		static constexpr double mbPerInHg = 1.0 / 0.02953;

		static constexpr double mphPerKnot = 1.15078;
		static constexpr double mpsPerKnot = 0.514444;
		static constexpr double kphPerKnot = 1.852;

		static constexpr double miles2Nm = 0.868976;
		static constexpr double miles2Km = 1.60934;

    public:
		/**
		 * @brief Converts a temperature from Celsius to Fahrenheit.
		 *
		 * This function takes a temperature in Celsius and converts it to the equivalent
		 * temperature in Fahrenheit using the formula: (temp * 1.8) + 32.0.
		 *
		 * @param temp The temperature in Celsius.
		 * @return The equivalent temperature in Fahrenheit.
		 */
  		static double c2f(double temp)
	  	{
      		return (temp * 1.8) + 32.0;
		}

		/**
		 * @brief Converts a temperature from Fahrenheit to Celsius.
		 *
		 * This method performs the conversion of a given temperature in degrees Fahrenheit
		 * to its equivalent in degrees Celsius using the standard formula.
		 *
		 * @param temp The temperature in degrees Fahrenheit to be converted.
		 * @return The equivalent temperature in degrees Celsius.
		 */
  		static double f2c(double temp)
		{
			return (temp - 32.0) / 1.8;
		}

		/**
		 * @brief Converts a distance from feet to meters.
		 *
		 * This method takes a distance measurement in feet and converts it to meters
		 * using a standard conversion factor.
		 *
		 * @param f A double value representing the distance in feet.
		 * @return A double value representing the equivalent distance in meters.
		 */
  		static double f2m(double f)
        {
	  		return f * metersPerFoot;
        }

		/**
		 * @brief Converts a distance from meters to feet.
		 *
		 * This function takes a distance measurement in meters and converts it to feet
		 * using a predefined conversion factor.
		 *
		 * @param m The distance in meters to be converted.
		 * @return The equivalent distance in feet.
		 */
  		static double m2f(double m)
        {
	  		return m * feetPerMeter ;
        }

		/**
		 * @brief Converts a length from inches to centimeters.
		 *
		 * This function takes a length value in inches and converts it to the equivalent
		 * value in centimeters using a predefined conversion factor.
		 *
		 * @param inch The length in inches to be converted.
		 * @return The equivalent length in centimeters.
		 */
  		static double inch2cm(double inch)
        {
	  		return inch * inchesPerCentimeter;
        }

		/**
		 * @brief Converts pressure from millibars to inches of mercury.
		 *
		 * This method takes a pressure value in millibars and converts it
		 * to inches of mercury, using a predefined conversion factor.
		 *
		 * @param mb The pressure value in millibars to be converted.
		 * @return The equivalent pressure value in inches of mercury.
		 */
  		static double mb2InMg(double mb)
        {
	  		return mb * inHgPerMb;
	    }

		/**
		 * @brief Converts pressure from inches of mercury (inHg) to millibars (mb).
		 *
		 * This method takes a pressure value in inches of mercury and converts it
		 * to its equivalent in millibars using a predefined conversion factor.
		 *
		 * @param ih The pressure value in inches of mercury (inHg) to be converted.
		 * @return The equivalent pressure value in millibars (mb).
		 */
  		static double inHg2Mb(double ih)
        {
	  		return ih * mbPerInHg;
        }

		/**
		 * @brief Converts speed from knots to miles per hour (mph).
		 *
		 * This method takes a speed in knots and converts it to miles per hour using
		 * a predefined conversion factor.
		 *
		 * @param s The speed in knots to be converted.
		 * @return The equivalent speed in miles per hour (mph).
		 */
  		static double Kts2Mph(double s)
	  	{
		    return s * mphPerKnot;
	    }

		/**
		 * @brief Converts speed from knots to meters per second.
		 *
		 * This method takes a speed in knots and converts it to the equivalent
		 * speed in meters per second using a predefined conversion factor.
		 *
		 * @param s The speed in knots to be converted.
		 * @return The equivalent speed in meters per second.
		 */
  		static double Kts2Mps(double s)
	  	{
	  		return s * mpsPerKnot;
	    }

		/**
		 * @brief Converts speed from knots to kilometers per hour.
		 *
		 * This method takes a speed value in knots and converts it to kilometers per hour
		 * using a predefined conversion factor.
		 *
		 * @param s The speed value in knots to convert.
		 * @return The equivalent speed in kilometers per hour.
		 */
  		static double Kts2Kph(double s)
	  	{
	  		return s * kphPerKnot;
	    }

		/**
		 * @brief Converts a distance from miles to nautical miles.
		 *
		 * This static method takes a distance in miles as input and converts it
		 * to the equivalent distance in nautical miles using a predefined conversion factor.
		 *
		 * @param v The distance in miles to be converted.
		 * @return The converted distance in nautical miles.
		 */
  		static double Miles2Nm(double v)
	  	{
	  		return v * miles2Nm;
	    }

		/**
		 * @brief Converts a value from miles to kilometers.
		 *
		 * This method takes a distance value in miles and converts it to its equivalent in kilometers
		 * using a predefined conversion factor.
		 *
		 * @param v The distance value in miles to be converted.
		 * @return The equivalent distance in kilometers.
		 */
  		static double Miles2Km(double v)
	  	{
	  		return v * miles2Km;
	    }

	  	Convert() = delete;
        Convert(const Convert&) = delete;
	  	Convert(Convert&&) = delete;
        Convert& operator=(const Convert&) = delete;
	  	Convert& operator=(Convert&&) = delete;
        ~Convert() = default;
    };
  }
}

