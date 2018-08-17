//
// Copyright (c) 2018 James A. Chappell (rlrrlrll@gmail.com)
//
// Misc. weather utilities
//
#include "Utils.h"

#ifndef NO_SHARED_PTR
#include <cmath>
#else
#include <math.h>
#endif

#include <Convert.h>

using namespace Storage_B::Weather;

double Utils::Humidity(double t, double td)
{
  return (100.0 * exp((17.625 * td) / (243.04 + td)) / 
                          exp((17.625 * t) / (243.04 + t)));
}

double Utils::WindChill(double temp, double wind_speed)
{
  double twc(temp);
  
  if ((wind_speed > 4.8) && (temp <= 10.0))
  {
    double v16(pow(wind_speed, 0.16));
    twc = 13.12 + (0.6215 * temp) - (11.37 * v16) 
                            + (0.3965 * temp * v16);
  }

  return twc;
}

double Utils::HeatIndex(double temp, double humidity, bool celsius_flg)
{
  double t(celsius_flg ? Convert::c2f(temp) : temp);

  double thi(t);
  if (t >= 80.0)
  {
    double adj(0.0);
    if ((humidity < 13.0) && (t > 80.0) && (t < 112.0))
    {
      adj = -(((13.0 - humidity) / 4.0)
            * sqrt((17.0 -fabs(t - 95.0)) / 17.0));
    }
    else if ((humidity > 85.0) && (t >= 80.0) && (t < 87.0))
    {
      adj = ((humidity - 85.0) / 10.0) * ((87.0 - t) / 5.0);
    }

    thi = -42.379
      + 2.04901523 * t 
      + 10.14333127 * humidity 
      - 0.22475541 * t * humidity 
      - 0.00683783 * t * t 
      - 0.05481717 * humidity * humidity 
      + 0.00122874 * t * t * humidity 
      + 0.00085282 * t * humidity * humidity 
      - 0.00000199 * t * t * humidity * humidity;
    
    thi += adj;
  
    return (celsius_flg ? Convert::f2c(thi) : thi);
  }

  return temp;
}
