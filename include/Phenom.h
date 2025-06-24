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
    /**
     * @class Phenom
     * @brief An abstract representation of various meteorological phenomena and their properties.
     *
     * This class provides an interface for managing and analyzing meteorological phenomena.
     * Various phenomena types and their properties such as intensity, visibility, and additional
     * conditions like blowing, freezing, and drifting are encapsulated within this structure.
     */
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

      /**
       * @brief Creates a Phenom instance based on input string and tempo flag.
       *
       * This method parses the input string to identify meteorological phenomena,
       * including their intensity and associated attributes such as blowing, freezing,
       * or drifting. It returns a shared pointer to a Phenom instance if the input
       * represents valid phenomena data or null if the input is invalid.
       *
       * @param str A character pointer representing the meteorological phenomena string.
       *            It is expected to follow a specific format for identifying phenomena.
       * @param tempo A boolean flag indicating whether the phenomena are temporary in nature.
       * @return A shared pointer to a Phenom instance if successfully created, or nullptr if
       *         the input string is invalid or does not represent any phenomena.
       */
      static std::shared_ptr<Phenom> Create(const char *str, bool tempo = false);

      /**
       * @brief Virtual destructor for the Phenom class.
       *
       * Ensures proper cleanup of derived class resources when the object is deleted
       * through a pointer to the base class. This destructor is declared as `default`
       * for compiler-provided implementation.
       */
      virtual ~Phenom() = default;

      /**
       * @brief Retrieves the number of phenomena represented by the current instance.
       *
       * This method returns the count of meteorological phenomena encapsulated
       * in the current object. Each phenomenon is represented as a distinct entity.
       *
       * @return The number of meteorological phenomena.
       */
      virtual unsigned int NumPhenom() const = 0;

      /**
       * @brief Accesses the meteorological phenomenon at the specified index in a collection.
       *
       * This operator provides read-only access to a specific phenomenon within a collection
       * of meteorological phenomena based on the given index.
       *
       * @param idx The index of the phenomenon within the collection.
       * @return The meteorological phenomenon at the specified index.
       */
      virtual phenom operator[](typename std::vector<Phenom>::size_type idx) const = 0;

      /**
       * @brief Retrieves the intensity of the meteorological phenomenon.
       *
       * This function provides access to the intensity level of the current
       * phenomenon, which could represent various degrees such as light, normal, or severe.
       *
       * @return The intensity of the meteorological phenomenon as a value from the `intensity` enumeration.
       */
      virtual intensity Intensity() const = 0;

      /**
       * @brief Determines if the phenomenon involves blowing conditions.
       *
       * This method checks whether the phenomenon is associated with blowing conditions,
       * such as blowing snow or sand, typically indicative of reduced visibility
       * due to particles lifted into the air by wind.
       *
       * @return True if the phenomenon involves blowing conditions, false otherwise.
       */
      virtual bool Blowing() const = 0;

      /**
       * @brief Determines whether the phenomenon involves freezing activity.
       *
       * This method evaluates if the meteorological phenomenon has a freezing component,
       * such as freezing rain or freezing drizzle.
       *
       * @return True if the phenomenon involves freezing, otherwise false.
       */
      virtual bool Freezing() const = 0;

      /**
       * @brief Indicates whether the phenomenon involves drifting conditions.
       *
       * This method checks if the current meteorological phenomenon includes
       * any drifting-related aspects. Drifting conditions could refer to
       * movements caused by the wind involving loose particles like snow, sand, or dirt.
       *
       * @return true if drifting is involved, false otherwise.
       */
      virtual bool Drifting() const = 0;

      /**
       * @brief Pure virtual function to determine if the phenomenon occurs in the vicinity.
       *
       * This method provides an interface for determining whether a meteorological phenomenon
       * is present within the vicinity of a specific location. The exact definition of "vicinity"
       * depends on the implementation within derived classes.
       *
       * @return True if the phenomenon is in the vicinity, otherwise false.
       */
      virtual bool Vicinity() const = 0;

      /**
       * @brief Determines if the phenomenon is partial in nature.
       *
       * This method checks whether the current phenomenon has a partial characteristic,
       * indicating it does not encompass the entire extent of its potential scope.
       *
       * @return True if the phenomenon is partial, otherwise false.
       */
      virtual bool Partial() const = 0;

      /**
       * @brief Determines if the phenomenon has shallow characteristics.
       *
       * This method specifies whether the phenomenon exhibits shallow properties.
       *
       * @return True if the phenomenon is shallow, otherwise false.
       */
      virtual bool Shallow() const = 0;

      /**
       * @brief Abstract method to determine the presence of patches.
       *
       * his method specifies whether patches are present.
       *
       * @return A boolean value indicating the presence of patches.
       */
      virtual bool Patches() const = 0;

      /**
       * @brief Determines the presence of a thunderstorm.
       *
       * This method is used to identify whether a thunderstorm condition is occurring.
       *
       * @return A boolean indicating the presence of a thunderstorm.
       *         Returns true if a thunderstorm is present, otherwise false.
       */
      virtual bool ThunderStorm() const = 0;

      /**
       * @brief Determines if a phenomenon is temporary.
       *
       * This method is used to check whether a specific phenomenon
       * or condition is considered temporary in its nature or existence.
       *
       * @return A boolean value indicating if the phenomenon is temporary.
       */
      virtual bool Temporary() const = 0;
    };
  }
}
