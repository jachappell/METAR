//
// Copyright (c) 2018 James A. Chappell
//
// Units conversion
//

#ifndef __CONVERT_H__
#define __CONVERT_H__

namespace Storage_B 
{
  namespace Weather
  {
    class Convert
    {
    private:
	    static constexpr double feetPerMeter = 3.28084;
	    static constexpr double metersPerFoot = 1.0 / feetPerMeter;
	    static constexpr double inchesPerCentimeter = 2.54;
	
	    static constexpr double inHgPerMb = 0.02953;
	    static constexpr double mbPerInHg = 1.0 / 0.02953;
	
	    static constexpr double mphPerKnot = 1.15078;
	    static constexpr double mpsPerKnot = 0.514444;
      static constexpr double kphPerKnot = 1.852;
  
	    static constexpr double miles2Nm = 0.868976;
      static constexpr double miles2Km = 1.60934;

    public:
	    static double c2f(double temp) 
      {
		    return (temp * 1.8) + 32.0;
	    }

      static double f2c(double temp)
      {
        return (temp - 32.0) / 1.8;
      }

	    static double f2m(double f)
      {
		    return f * metersPerFoot;
	    }
	
	    static double m2f(double m) 
      {
		    return m * feetPerMeter ;
	    }

	    static double inch2cm(double inch) 
      {
		    return inch * inchesPerCentimeter;
	    }
	
	    static double mb2InMg(double mb) 
      {
		    return mb * inHgPerMb;
	    }
	
	    static double inHg2Mb(double ih) 
      {
		    return ih * mbPerInHg;
	    }

	    static double Kts2Mph(double s) 
      {
		    return s * mphPerKnot;
	    }

	    static double Kts2Mps(double s) 
      {
		    return s * mpsPerKnot;
	    }

	    static double Kts2Kph(double s) 
      {
		    return s * kphPerKnot;
	    }

	    static double Miles2Nm(double v) 
      {
		    return v * miles2Nm;
	    }

	    static double Miles2Km(double v) 
      {
		    return v * miles2Km;
	    }

      Convert() = delete;
      Convert(const Convert&) = delete;
      Convert& operator=(const Convert&) = delete;
      ~Convert() = default;
    };
  }
}

#endif
