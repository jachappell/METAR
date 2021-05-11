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
    class Utils
    {
    public:
      static double Humidity(double t, double td);

      static double WindChill(double temp, double wind_speed);

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
