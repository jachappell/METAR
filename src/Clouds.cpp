//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// METAR clouds decoder
//

#include "Clouds.h"

#include <stdlib.h>
#include <string.h>
#include <limits.h>

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
  CloudsImpl(bool temp, cover c, int a = INT_MIN,
                   type t = type::undefined)
    : _cover(c)
    , _alt(a)
    , _tempo(temp)
    , _type(t)
  {
  }

  virtual ~CloudsImpl() = default;

  CloudsImpl() = delete;

  CloudsImpl(const CloudsImpl&) = delete;
  CloudsImpl& operator=(const CloudsImpl&) = delete;

  virtual cover Cover() const { return _cover; } 
  virtual int Altitude() const { return _alt; }
  virtual bool hasAltitude() const { return _alt != INT_MIN; }
  virtual type CloudType() const { return _type; }
  virtual bool hasCloudType() const { return _type != type::undefined; }
  virtual bool Temporary() const { return _tempo; }

private:
    cover _cover;
    int _alt;
    bool _tempo;
    type _type;
};

#ifndef NO_SHARED_PTR
          std::shared_ptr<Clouds>
#else
          Clouds *
#endif
Clouds::Create(const char *str, bool tempo)
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
#ifndef NO_SHARED_PTR
      return make_shared<CloudsImpl>(tempo,
                static_cast<Clouds::cover>(idx));
#else
      return new CloudsImpl(tempo,
                static_cast<Clouds::cover>(idx)); 
#endif
    }
    else if (str[6] == '\0')
    {
#ifndef NO_SHARED_PTR
      return make_shared<CloudsImpl>(tempo,
              static_cast<Clouds::cover>(idx), atoi(str + 3) * 100);
#else
      return
          new CloudsImpl(tempo, static_cast<Clouds::cover>(idx), 
                               atoi(str + 3) * 100);
#endif
    }
    else
    {
      Clouds::type t = Clouds::type::undefined;
      for (size_t j = 0 ; j < NUM_CLOUDS ; j++)
      {
        if (!strcmp(str + 6, cloud_types[j]))
        {
          t = static_cast<Clouds::type>(j);
          break;
        }
      } 
#ifndef NO_SHARED_PTR
      return make_shared<CloudsImpl>(tempo,
              static_cast<Clouds::cover>(idx), atoi(str + 3) * 100, t);
#else
      return
          new CloudsImpl(tempo, static_cast<Clouds::cover>(idx), 
                               atoi(str + 3) * 100, t);
#endif
    }
  }

  return nullptr;
}

