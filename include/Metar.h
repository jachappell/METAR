//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR decoder
//

#pragma once

#include <memory>
#include <optional>

namespace Storage_B
{
  namespace Weather
  {
    class Clouds;
    class Phenom;

    /**
     * @class Metar
     * @brief Abstract base class for decoding METAR weather reports.
     *
     * The Metar class serves as a base interface for parsing and
     * retrieving weather-related information from METAR reports.
     */
    class Metar
    {
    public:
      /**
       * @brief Factory method to create a Metar instance from a raw METAR string.
       *
       * This static method parses the provided METAR string and returns
       * a shared pointer to a Metar object with the extracted weather
       * information, if valid. The method ensures that the created
       * object can access relevant decoded attributes like time,
       * temperature, dew point, etc., from the METAR string.
       *
       * @param metar_str A null-terminated character string containing
       * the raw METAR weather report to be parsed.
       * @return A shared pointer to the created Metar instance, or
       * appropriate behavior if the supplied string is invalid.
       */
      static std::shared_ptr<Metar> Create(const char *metar_str);

      /**
       * @brief Factory method to create a Metar instance from a METAR string.
       *
       * This method parses the given METAR weather report string and creates
       * an appropriate Metar instance. The Metar object provides methods
       * to retrieve weather-related data such as temperature and dew point.
       *
       * @param metar_str The raw METAR string to be parsed and decoded.
       * @return A shared pointer to a Metar instance containing the parsed data,
       *         or nullptr if the input string is invalid or cannot be processed.
       */
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

      /**
       * @brief Virtual destructor for the Metar class.
       *
       * Allows proper cleanup of derived classes when deleted
       * through a base class pointer.
       */
      virtual ~Metar() = default;

      // no copy
      Metar(const Metar&) = delete;
      Metar(Metar&&) = delete;
      Metar& operator=(const Metar&) = delete;
      Metar& operator=(Metar&&) = delete;

      /**
       * @brief Retrieves the message type of the METAR report.
       *
       * This method provides the type of message associated with the METAR
       * report, if available.
       *
       * @return An optional containing the message type if it is available,
       *         or an empty optional if the type is not specified.
       */
      virtual std::optional<message_type> MessageType() const = 0;

      /**
       * @brief Retrieves the ICAO (International Civil Aviation Organization) code.
       *
       * This method provides the ICAO code, which is used to identify an airport or
       * aerodrome within the context of aviation operations.
       *
       * @return An optional string containing the ICAO code if available, or
       *         std::nullopt if not set.
       */
      virtual std::optional<std::string> ICAO() const = 0;

      /**
       * @brief Retrieves the day of the month from the decoded data.
       *
       * This method returns the day of the month as an integer if available.
       *
       * @return An optional integer representing the day of the month, or
       *         an empty optional if the day is not available.
       */
      virtual std::optional<int> Day() const = 0;

      /**
       * @brief Retrieves the hour component from the decoded information.
       *
       * This method returns the hour as an optional integer. The returned value
       * will be populated if the hour information is successfully parsed, otherwise
       * it will not contain a value.
       *
       * @return An optional integer representing the hour, or an empty optional if
       * the information is unavailable.
       */
      virtual std::optional<int> Hour() const = 0;

      /**
       * @brief Retrieves the minute component of a time value.
       *
       * This method provides the minute portion, if available, from
       * a time-related data source. It is intended to be overridden by
       * derived classes to return the specific minute value.
       *
       * @return An optional integer containing the minute value if available, or an empty optional if undefined.
       */
      virtual std::optional<int> Minute() const = 0;

      /**
       * @brief Retrieves the wind direction from the weather report.
       *
       * This method returns the wind direction in degrees, if available.
       *
       * @return An optional integer representing the wind direction in degrees
       *         or an empty optional if the information is not available.
       */
      virtual std::optional<int> WindDirection() const = 0;

      /**
       * @brief Checks if the wind direction is variable.
       *
       * This method determines whether the wind direction is classified
       * as variable based on the METAR weather report data.
       *
       * @return True if the wind direction is variable, false otherwise.
       */
      virtual bool isVariableWindDirection() const = 0;

      /**
       * @brief Retrieves the wind speed from the METAR report.
       *
       * This function provides the wind speed information if available in the METAR data.
       *
       * @return An optional integer representing the wind speed in knots. If the wind speed is not available, returns an empty optional.
       */
      virtual std::optional<int> WindSpeed() const = 0;

      /**
       * @brief Retrieves the wind gust value from the METAR report, if available.
       *
       * This method provides access to the wind gust information described in the METAR
       * report. If the wind gust value is not present in the report, the result will be
       * an empty optional.
       *
       * @return A std::optional containing the wind gust value as an integer if available, or an empty optional if the information is not present.
       */
      virtual std::optional<int> WindGust() const = 0;

      /**
       * @brief Retrieves the minimum wind direction from the METAR report.
       *
       * This method obtains the minimum wind direction value, if available,
       * from the METAR weather report. The value represents the lower bound
       * of the wind direction range when variations are present.
       *
       * @return An optional integer representing the minimum wind direction
       *         in degrees, or an empty optional if unavailable.
       */
      virtual std::optional<int> MinWindDirection() const = 0;

      /**
       * @brief Retrieves the maximum wind direction from the METAR report.
       *
       * Provides the maximum wind direction, if available, as an optional value.
       * The value represents the wind direction in degrees.
       *
       * @return An optional integer representing the maximum wind direction
       *         in degrees, or std::nullopt if not available.
       */
      virtual std::optional<int> MaxWindDirection() const = 0;

      /**
       * @brief Retrieves the units of measurement for wind speed.
       *
       * This method provides the wind speed units as an optional value,
       * which specifies the unit type used in the METAR report.
       *
       * @return An optional containing the wind speed units, or an empty
       * optional if the units are not specified.
       */
      virtual std::optional<speed_units> WindSpeedUnits() const = 0;

      /**
       * @brief Retrieves the visibility value from a METAR report.
       *
       * This method is responsible for returning the visibility value if it is
       * available in the METAR report. The value is expressed as a double and
       * represents the visibility in units defined by the reporting system.
       *
       * @return An optional containing the visibility value if available, or
       *         an empty optional if the visibility information is not provided.
       */
      virtual std::optional<double> Visibility() const = 0;

      /**
       * @brief Retrieves the visibility units from the METAR report.
       *
       * This method provides the units used for representing visibility
       * measurements in the METAR report, if available.
       *
       * @return An optional containing the visibility units, or an empty optional
       *         if the visibility units are not specified.
       */
      virtual std::optional<distance_units> VisibilityUnits() const = 0;

      /**
       * @brief Pure virtual function to determine if visibility is below a specific threshold.
       *
       * This function provides  logic for evaluating whether the visibility condition meets
       * specificcriteria based on the implementation.
       *
       * @return True if visibility is below the threshold; otherwise, false.
       */
      virtual bool isVisibilityLessThan() const = 0;

      /**
       * @brief Checks if the METAR report indicates CAVOK (Ceiling and Visibility OK).
       *
       * This function determines whether the weather conditions in the METAR report
       * satisfy the CAVOK criteria, which typically include clear skies and good visibility.
       *
       * @return True if the METAR report indicates CAVOK conditions, false otherwise.
       */
      virtual bool isCAVOK() const = 0;

      /**
       * @brief Retrieves the vertical visibility value if available.
       *
       * This pure virtual function provides the vertical visibility
       * measurement, typically expressed in feet, if such data is present.
       *
       * @return An optional containing the vertical visibility value
       *         or an empty optional if the value is not available.
       */
      virtual std::optional<int> VerticalVisibility() const = 0;

      /**
       * @brief Retrieves the temperature value from the implemented METAR report.
       *
       * This method provides the temperature information parsed from the METAR
       * weather report, if available.
       *
       * @return An optional integer representing the temperature in the METAR report.
       *         If the temperature is unavailable, the optional will contain no value.
       */
      virtual std::optional<int> Temperature() const = 0;

      /**
       * @brief Retrieves the dew point temperature.
       *
       * This method provides the dew point temperature, if available, as an optional integer.
       * The dew point is a measure of atmospheric moisture.
       *
       * @return An optional containing the dew point temperature in integer form if available, or an empty optional otherwise.
       */
      virtual std::optional<int> DewPoint() const = 0;

      /**
       * @brief Retrieves the altimeter setting in inches of mercury (A-prefix format).
       *
       * This pure virtual function provides the altimeter value, if available,
       * using the A-prefix format commonly used in aviation (e.g., A2992). The
       * returned value is represented as a double in inches of mercury.
       *
       * @return An optional double containing the altimeter value if present, or
       *         std::nullopt if unavailable.
       */
      virtual std::optional<double> AltimeterA() const = 0;

      /**
       * @brief Retrieves the altimeter setting in QNH (hectopascals).
       *
       * This method provides the altimeter value extracted from a METAR report,
       * represented in terms of QNH (hectopascals) if available.
       *
       * @return An optional integer representing the altimeter setting in QNH, or
       *         std::nullopt if the value is not available or not present in the data.
       */
      virtual std::optional<int> AltimeterQ() const = 0;

      /**
       * @brief Retrieves the sea-level atmospheric pressure.
       *
       * This method provides the sea-level pressure from the weather report
       * if the information is available. If the sea-level pressure is not
       * specified, it returns an empty optional value.
       *
       * @return An optional containing the sea-level pressure in hectopascals (hPa),
       * or an empty optional if the information is unavailable.
       */
      virtual std::optional<double> SeaLevelPressure() const = 0;

      /**
       * @brief Retrieves the temperature in North America format, if available.
       *
       * Provides an optional value representing the temperature specific to
       * the North American region. The implementation may return an empty
       * optional if the temperature information is not available or applicable.
       *
       * @return An optional containing the temperature in North America format, or empty if unavailable.
       */
      virtual std::optional<double> TemperatureNA() const = 0;

      /**
       * @brief Retrieves the dew point temperature in degrees, if available.
       *
       * This method provides an optional dew point temperature value measured
       * in degrees. It returns the correct value or an empty optional if the
       * dew point is not available.
       *
       * @return An optional containing the dew point temperature or an empty optional
       *         if the value is not available.
       */
      virtual std::optional<double> DewPointNA() const = 0;

      /**
       * @brief Retrieves the number of cloud layers reported.
       *
       * This method provides the total count of distinct cloud layers
       * identified in the context of the implemented weather report.
       *
       * @return The number of cloud layers as an unsigned integer.
       */
      virtual unsigned int NumCloudLayers() const = 0;

      /**
       * @brief Retrieves cloud layer information at the specified index.
       *
       * This method provides access to a specific cloud layer by its index
       * in the sequence of reported cloud layers. The returned object
       * contains details about the cloud layer.
       *
       * @param idx The zero-based index of the desired cloud layer.
       * @return A shared pointer to a Clouds object representing the
       *         specified cloud layer.
       * @throw Implementation-defined exceptions may occur if the index
       *        is out of bounds or invalid.
       */
      virtual std::shared_ptr<Clouds> Layer(unsigned int idx) const = 0;

      /**
       * @brief Retrieves the number of weather phenomena in the report.
       *
       * This method provides the count of distinct weather phenomena
       * present in the report or data source.
       *
       * @return The number of weather phenomena as an unsigned integer.
       */
      virtual unsigned int NumPhenomena() const = 0;

      /**
       * @brief Retrieves a specific weather phenomenon by index.
       *
       * This method returns a reference to a weather phenomenon object
       * at the specified index within the collection.
       *
       * @param idx The zero-based index of the phenomenon to retrieve.
       * @return A constant reference to the Phenom object at the specified index.
       */
      virtual const Phenom& Phenomenon(unsigned int idx) const = 0;
    };
  }
}
