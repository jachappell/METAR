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
    /**
     * @class Clouds
     * @brief Represents a cloud layer with attributes such as coverage,
     *        altitude, and cloud type, along with an optional temporary status.
     *
     * The Clouds class is used to model individual cloud layers. It includes
     * information about the cloud coverage type, its altitude, and an optional
     * cloud type. The class also supports determining whether the cloud
     * observation is temporary.
     */
    class Clouds
    {
    public:
      /**
       * @enum cover
       * @brief Enumerates various types of cloud coverage conditions.
       *
       * The cover enumeration represents different levels or types of cloud coverage
       * conditions as observed in meteorological data. Each value provides a distinct
       * categorization of sky coverage, ranging from clear skies to overcast.
       */
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

      /**
       * @enum type
       * @brief Defines a set of cloud types commonly observed in meteorology.
       *
       * The type enumeration specifies distinct cloud formations, which include:
       * - TCU: Towering Cumulus
       * - CB: Cumulonimbus
       * - ACC: Altocumulus Castellanus
       */
      enum class type
      {
        TCU,
        CB,
        ACC 
      };

      /**
       * @brief Creates a Clouds object based on the provided string representation
       *        of cloud layer information.
       *
       * This method parses a string to determine the cloud coverage type,
       * altitude, and optional cloud type. It also specifies whether the cloud
       * observation is temporary. The information is used to create and return
       * an appropriate `Clouds` object.
       *
       * @param str A null-terminated string representing the cloud layer,
       *            formatted to indicate cloud coverage (e.g., "CLR", "FEW105",
       *            "OVC050TCU"). Depending on the format, it may include
       *            additional details such as altitude or a specific cloud type.
       * @param tempo A boolean flag indicating whether the cloud observation
       *              is considered temporary.
       * @return A shared pointer to a `Clouds` object that represents the parsed
       *         cloud layer, or `nullptr` if the input string cannot be parsed.
       */
      static std::shared_ptr<Clouds> Create(const char *str, bool tempo = false);

      /**
       * @brief Virtual destructor for the Clouds class.
       *
       * Ensures proper cleanup of resources when an instance of a derived
       * class is deleted through a pointer to the Clouds base class.
       */
      virtual ~Clouds() = default;

      /**
       * @brief Retrieves the cloud coverage type for the current cloud layer.
       *
       * This method returns the specific type of cloud coverage, such as CLR, SKC,
       * NSC, FEW, SCT, BKN, OVC, or other defined types. The cover value represents
       * the general cloud coverage description associated with the layer.
       *
       * @return The cloud coverage type as a member of the `cover` enumeration.
       */
      virtual cover Cover() const = 0;

      /**
       * @brief Retrieves the altitude of the cloud layer if available.
       *
       * This method provides the altitude information for a cloud layer.
       * If the altitude is not applicable or unknown, the method returns an empty optional.
       *
       * @return An optional integer representing the altitude in feet above ground level
       *         if available; otherwise, an empty optional.
       */
      virtual std::optional<int> Altitude() const = 0;

      /**
       * @brief Retrieves the cloud type if available.
       *
       * This method provides the cloud type information associated with a cloud
       * layer object. If no specific cloud type is defined, the method returns
       * an empty optional.
       *
       * @return An std::optional containing the cloud type if available, or an
       *         empty optional if there is no cloud type defined.
       */
      virtual std::optional<type> CloudType() const = 0;

      /**
       * @brief Indicates whether the cloud observation is temporary.
       *
       * This method determines if the cloud observation is of a temporary nature.
       * Temporary cloud observations might be used in scenarios where conditions
       * are expected to change within a short time frame.
       *
       * @return true if the cloud observation is temporary, false otherwise.
       */
      virtual bool Temporary() const = 0;
    };
  }
}
