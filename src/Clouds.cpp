//
// Copyright (c) 2020 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR clouds decoder
//

#include "Clouds.h"

#include <cstdlib>
#include <cstring>
#include <cstring>

using namespace std;
using namespace Storage_B::Weather;

namespace
{
  const char *sky_conditions[] =
  {
    "SKC",
    "CLR",
    "NSC",
    "FEW",
    "SCT",
    "BKN",
    "OVC"
  };
  const auto NUM_LAYERS =
      sizeof(sky_conditions) / sizeof(sky_conditions[0]);

  const char *cloud_types[] =
  {
    "TCU",
    "CB",
    "ACC" 
  };
  const auto NUM_CLOUDS =
      sizeof(cloud_types) / sizeof(cloud_types[0]);
}

class CloudsImpl : public Clouds
{
public:
  CloudsImpl(bool temp, cover c, int alt, type t)
    : _tempo(temp)
    , _cover(c)
    , _alt(alt)
    , _type(t)
  {
  }
  
  CloudsImpl(bool temp, cover c, int alt)
    : _tempo(temp)
    , _cover(c)
    , _alt(alt)
  {
  }
  
  CloudsImpl(bool temp, cover c)
    : _tempo(temp)
    , _cover(c)
  {
  }

  virtual ~CloudsImpl() = default;

  CloudsImpl() = delete;

  CloudsImpl(const CloudsImpl&) = delete;
  CloudsImpl& operator=(const CloudsImpl&) = delete;

  virtual cover Cover() const { return _cover; } 
  virtual optional<int> Altitude() const { return _alt; }
  virtual optional<type> CloudType() const { return _type; }
  virtual bool Temporary() const { return _tempo; }

private:
  bool _tempo;
  cover _cover;
  optional<int> _alt;
  optional<type> _type;
};

std::shared_ptr<Clouds> Clouds::Create(const char *str, bool tempo)
{
  bool cloud_flg = false;

  unsigned int idx = 0;

  for (unsigned int i = 0 ; i < NUM_LAYERS ; i++)
  {
    if (!strncmp(sky_conditions[i], str, strlen(sky_conditions[i])))
    {
      cloud_flg = true;
      idx = i;
      break;
    }
  }

  if (cloud_flg)
  {
    if (str[3] == '\0')
    {
      return make_shared<CloudsImpl>(tempo,
                static_cast<Clouds::cover>(idx));
    }
    else if (str[6] == '\0')
    {
      return make_shared<CloudsImpl>(tempo,
              static_cast<Clouds::cover>(idx), atoi(str + 3));
    }
    else
    {
      Clouds::type t;
      for (size_t j = 0 ; j < NUM_CLOUDS ; j++)
      {
        if (!strcmp(str + 6, cloud_types[j]))
        {
          t = static_cast<Clouds::type>(j);
          break;
        }
      } 
      return make_shared<CloudsImpl>(tempo,
              static_cast<Clouds::cover>(idx), atoi(str + 3), t);
    }
  }

  return nullptr;
}
