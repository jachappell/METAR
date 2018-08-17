#include "Phenom2String.h"

#include <map>

using namespace std;

namespace
{
  using namespace Storage_B::Weather;

  map<Phenom::phenom, string> phenom2String = 
  {
    {Phenom::phenom::NONE,             ""},
    {Phenom::phenom::MIST,             "mist"},
    {Phenom::phenom::DUST_STORM,       "dust storm"},
    {Phenom::phenom::DUST,             "dist"},
    {Phenom::phenom::DRIZZLE,          "drizzle"},
    {Phenom::phenom::FUNNEL_CLOUD,     "funnel cloud"},
    {Phenom::phenom::FOG,              "fog"},
    {Phenom::phenom::SMOKE,            "smoke"},
    {Phenom::phenom::HAIL,             "hail"},
    {Phenom::phenom::SMALL_HAIL,       "small hail"},
    {Phenom::phenom::HAZE,             "haze"},
    {Phenom::phenom::ICE_CRYSTALS,     "ice crystals"},
    {Phenom::phenom::ICE_PELLETS,      "ice pellets"},
    {Phenom::phenom::DUST_SAND_WHORLS, "dust sand whorls"},
    {Phenom::phenom::SPRAY,            "spray"},
    {Phenom::phenom::RAIN,             "rain"},
    {Phenom::phenom::SAND,             "sand"},
    {Phenom::phenom::SNOW_GRAINS,      "snow grains"},
    {Phenom::phenom::SHOWER,           "showers"},
    {Phenom::phenom::SNOW,             "snow"},
    {Phenom::phenom::SQUALLS,          "squalls"},
    {Phenom::phenom::SAND_STORM,       "sand storm"},
    {Phenom::phenom::UNKNOWN_PRECIP,   "unknown precipitation"},
    {Phenom::phenom::VOLCANIC_ASH,     "volcanic ash"}
  };

  string intensity2string(Phenom::intensity intensity)
  {
    string result;

    switch(intensity)
    {
      case Phenom::intensity::LIGHT:
        result = "light ";
        break;
      case Phenom::intensity::HEAVY:
        result = "heavy ";
        break;

      case Phenom::intensity::NORMAL:
      default:
        break;
    }

    return result;
  }
}

string Storage_B::Weather::Phenom2String(const Phenom& phenom)
{
  string result;

  for (unsigned int i = 0 ; i < phenom.NumPhenom() ; i++)
  {
    result += phenom2String[phenom[i]] +  " ";    
  }

  if (phenom.Blowing())
  {
    result = "blowing " + result;
  }

  if (phenom.Freezing())
  {
    result = "freezing " + result;
  }

  if (phenom.Drifting())
  {
    result = "drifting " + result;
  }

  if (phenom.Partial())
  {
    result = "partial " + result;
  }

  if (phenom.Shallow())
  {
    result = "shallow " + result;
  }

  if (phenom.Patches())
  {
    result = "patchy " + result;
  }

  if (phenom.ThunderStorm())
  {
    result += "thunder storm ";
  }

  if (phenom.Vicinity())
  {
    result += " in the vicinity";
  }

  result = intensity2string(phenom.Intensity()) + result;

  result[0] = toupper(result[0]);

  return result;
}
