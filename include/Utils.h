//
// Copyright (c) 2018 James A. Chappell
//
// Weather utilities
//

#ifndef __UTILS_H__
#define __UTILS_H__

namespace Storage_B 
{
  namespace Weather
  {
    class Utils
    {
    public:
      static double Humidity(double t, double td);
      static double WindChill(double temp, double wind_speed);

      Utils() = delete;
      Utils(const Utils&) = delete;
      Utils& operator=(const Utils&) = delete;
      ~Utils() = default;
    };
  }
}

#endif
