#include "Utils.h"

#include <math.h>

using namespace Storage_B::Weather;

double Utils::Humidity(double t, double td)
{
  return (100.0 * exp((17.625 * td) / (243.04 + td)) / 
                          exp((17.625 * t) / (243.04 + t)));
}

double Utils::WindChill(double temp, double wind_speed)
{
  double twc(temp);
  
  if ((wind_speed > 4.8) && (temp < 10.0))
  {
    double v16(pow(wind_speed, 0.16));
    twc = 13.12 + (0.6215 * temp) - (11.37 * v16) 
                            + (0.3965 * temp * v16);
  }

  return twc;
}
