//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder tests
//

#include "metar.h"

#include <string>
#include <iostream>

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_CASE(bad_observation_time_string)
{
  Metar metar("1234S6Z");

  BOOST_CHECK(!metar.hasDay());
  BOOST_CHECK(!metar.hasHour());
  BOOST_CHECK(!metar.hasMinute());
}

BOOST_AUTO_TEST_CASE(good_observation_time_string)
{
  Metar metar("123456Z");

  BOOST_CHECK(metar.Day() == 12);
  BOOST_CHECK(metar.Hour() == 34);
  BOOST_CHECK(metar.Minute() == 56);
}

BOOST_AUTO_TEST_CASE(bad_temperature_string)
{

  char buffer[8];
  strcpy(buffer, "8/06");
  
  Metar metar(buffer);

  BOOST_CHECK(!metar.hasTemperature());
  BOOST_CHECK(!metar.hasDewPoint());
}

BOOST_AUTO_TEST_CASE(good_temperature_string_both_positive1)
{

  char buffer[8];
  strcpy(buffer, "08/06");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 8);
  BOOST_CHECK(metar.DewPoint() == 6);
}

BOOST_AUTO_TEST_CASE(good_temperature_string_negative_dew_point)
{
  char buffer[8];
  strcpy(buffer, "01/M01");
  
  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 1);
  BOOST_CHECK(metar.DewPoint() == -1);
}

BOOST_AUTO_TEST_CASE(good_temperature_string_both_negative)
{
  char buffer[8];
  strcpy(buffer, "M14/M15");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == -14);
  BOOST_CHECK(metar.DewPoint() == -15);
}

BOOST_AUTO_TEST_CASE(good_temperature_string_both_positive2)
{
  char buffer[8];
  strcpy(buffer, "15/14");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 15);
  BOOST_CHECK(metar.DewPoint() == 14);
}

BOOST_AUTO_TEST_CASE(bad_temperatureNA_string)
{
  char buffer[10];
  strcpy(buffer, "T0083O067");

  Metar metar(buffer);

  BOOST_CHECK(!metar.hasTemperatureNA());
  BOOST_CHECK(!metar.hasDewPointNA());
}

BOOST_AUTO_TEST_CASE(good_temperatureNA_string1)
{
  char buffer[10];
  strcpy(buffer, "T00830067");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == 8.3);
  BOOST_CHECK(metar.DewPointNA() == 6.7);
}


BOOST_AUTO_TEST_CASE(good_temperatureNA_string2)
{
  char buffer[10];
  strcpy(buffer, "T01830167");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == 18.3);
  BOOST_CHECK(metar.DewPointNA() == 16.7);
}

BOOST_AUTO_TEST_CASE(good_temperatureNA_string_both_negative)
{
  char buffer[10];
  strcpy(buffer, "T10171018");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == -1.7);
  BOOST_CHECK(metar.DewPointNA() == -1.8);
}

BOOST_AUTO_TEST_CASE(bad_wind_string_kt)
{
  char buffer[9];
  strcpy(buffer, "250O5KT");

  Metar metar(buffer);

  BOOST_CHECK(!metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(!metar.hasWindSpeedUnits());
}

BOOST_AUTO_TEST_CASE(bad_wind_string_mps)
{
  Metar metar("2S005G12MPS");

  BOOST_CHECK(!metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(!metar.hasWindSpeedUnits());
}

BOOST_AUTO_TEST_CASE(good_wind_string_kt)
{
  char buffer[9];
  strcpy(buffer, "25005KT");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 250);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(good_wind_string_kt_3digit)
{
  Metar metar("240105KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 240);
  BOOST_CHECK(metar.WindSpeed() == 105);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);;
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(good_wind_string_kt_3digit_gust)
{
  Metar metar("240105G121KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 240);
  BOOST_CHECK(metar.WindSpeed() == 105);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 121);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);
}

BOOST_AUTO_TEST_CASE(good_wind_string_vrb_kt_3digit_gust)
{
  Metar metar("VRB105G121KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 105);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 121);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);
}

BOOST_AUTO_TEST_CASE(good_wind_string_kt_gust)
{
  Metar metar("25005G12KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 250);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 12);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);
}

BOOST_AUTO_TEST_CASE(good_wind_string_mps)
{
  char buffer[9];
  strcpy(buffer, "04503MPS");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 45);
  BOOST_CHECK(metar.WindSpeed() == 3);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "MPS") == 0);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(good_wind_string_mps_vrb)
{
  Metar metar("VRB03MPS");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 3);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "MPS") == 0);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(good_wind_string_mps_3digit_gust)
{
  Metar metar("08090G102MPS");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 80);
  BOOST_CHECK(metar.WindSpeed() == 90);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 102);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "MPS") == 0);
}

BOOST_AUTO_TEST_CASE(good_wind_string_kph)
{
  char buffer[9];
  strcpy(buffer, "04005KPH");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 40);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KPH") == 0);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(good_wind_string_vrb_kph_2digit_gust)
{
  Metar metar("VRB05G21KPH");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 5);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 21);

  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KPH") == 0);
}

BOOST_AUTO_TEST_CASE(bad_altA_string)
{
  char buffer[9];
  strcpy(buffer, "A1P02");

  Metar metar(buffer);

  BOOST_CHECK(!metar.hasAltimeterA());
}

BOOST_AUTO_TEST_CASE(bad_wind_var_string)
{
  Metar metar("O90V150");

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
}

BOOST_AUTO_TEST_CASE(good_wind_var_string)
{
  Metar metar("090V150");

  BOOST_CHECK(metar.hasMinWindDirection());
  BOOST_CHECK(metar.hasMaxWindDirection());

  BOOST_CHECK(metar.MinWindDirection() == 90);
  BOOST_CHECK(metar.MaxWindDirection() == 150);
}

BOOST_AUTO_TEST_CASE(good_altA_string)
{
  char buffer[9];
  strcpy(buffer, "A3006");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 30.06);
}

BOOST_AUTO_TEST_CASE(bad_altQ_string)
{
  char buffer[9];
  strcpy(buffer, "Q12J2");

  Metar metar(buffer);

  BOOST_CHECK(!metar.hasAltimeterQ());
}

BOOST_AUTO_TEST_CASE(good_altQ_string)
{
  char buffer[9];
  strcpy(buffer, "Q1020");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasAltimeterQ());
  BOOST_CHECK(metar.AltimeterQ() == 1020);
}

BOOST_AUTO_TEST_CASE(bad_slp_string)
{
  char buffer[9];
  strcpy(buffer, "SLPl77");

  Metar metar(buffer);

  BOOST_CHECK(!metar.hasSeaLevelPressure());
}

BOOST_AUTO_TEST_CASE(good_slp_string)
{
  char buffer[9];
  strcpy(buffer, "SLP177");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasSeaLevelPressure());
  BOOST_CHECK(metar.SeaLevelPressure() == 1017.7);
}

BOOST_AUTO_TEST_CASE(bad_visibility_string_meters)
{
  Metar metar("15O0");

  BOOST_CHECK(!metar.hasVisibility());
  BOOST_CHECK(!metar.hasVisibilityUnits());
}

BOOST_AUTO_TEST_CASE(good_visibility_string_meters)
{
  Metar metar("1500");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 1500);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "M") == 0);
}

BOOST_AUTO_TEST_CASE(bad_visibility_string_sm)
{
  Metar metar1("1OSM");

  BOOST_CHECK(!metar1.hasVisibility());
  BOOST_CHECK(!metar1.hasVisibilityUnits());

  Metar metar2("l0SM");

  BOOST_CHECK(!metar2.hasVisibility());
  BOOST_CHECK(!metar2.hasVisibilityUnits());
}

BOOST_AUTO_TEST_CASE(good_visibility_string_integer_sm)
{
  Metar metar("10SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 10);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(good_visibility_string_fraction_sm_1)
{
  Metar metar("1/4SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 0.25);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(good_visibility_string_fraction_sm_2)
{
  Metar metar("5/16SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == (5.0 / 16.0));

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(good_visibility_string_fraction_sm_3)
{
  Metar metar("2 1/2SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 2.5);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(bad_vert_visibility_string)
{
  Metar metar("VV0O7");
  
  BOOST_CHECK(!metar.hasVerticalVisibility());
}

BOOST_AUTO_TEST_CASE(good_vert_visibility_string)
{
  Metar metar("VV105");
  
  BOOST_CHECK(metar.hasVerticalVisibility());
  BOOST_CHECK(metar.VerticalVisibility() == 10500);
}

BOOST_AUTO_TEST_CASE(real_METAR_1)
{
  char buffer[100];
  strcpy(buffer, "KSTL 231751Z 27009KT 10SM OVC015 09/06 A3029 RMK AO2 SLP260 T00940061 10100 20078 53002");

  Metar metar(buffer);
  
  BOOST_CHECK(!metar.hasMETAR());

  BOOST_CHECK(metar.hasICAO());
  BOOST_CHECK(strcmp(metar.ICAO(), "KSTL") == 0);

  BOOST_CHECK(metar.Day() == 23);
  BOOST_CHECK(metar.Hour() == 17);
  BOOST_CHECK(metar.Minute() == 51);

  BOOST_CHECK(metar.WindDirection() == 270);
  BOOST_CHECK(metar.WindSpeed() == 9);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 10);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == 9);
  BOOST_CHECK(metar.DewPoint() == 6);
  
  BOOST_CHECK(!metar.hasAltimeterQ());
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 30.29);

  BOOST_CHECK(metar.hasSeaLevelPressure());
  BOOST_CHECK(metar.SeaLevelPressure() == 1026.0);

  BOOST_CHECK(metar.TemperatureNA() == 9.4);
  BOOST_CHECK(metar.DewPointNA() == 6.1);
}

BOOST_AUTO_TEST_CASE(real_METAR_2)
{
  string metar_str = "METAR LBBG 041600Z 12012MPS 090V150 1400 R04/P1500N R22/P1500U +SN BKN022 OVC050 M04/M07 Q1020 NOSIG 8849//91=";

  Metar metar(metar_str.c_str());

  BOOST_CHECK(metar.hasMETAR());
  BOOST_CHECK(strcmp(metar.METAR(), "METAR") == 0);

  BOOST_CHECK(metar.hasICAO());
  BOOST_CHECK(strcmp(metar.ICAO(), "LBBG") == 0);

  BOOST_CHECK(metar.Day() == 4);
  BOOST_CHECK(metar.Hour() == 16);
  BOOST_CHECK(metar.Minute() == 0);

  BOOST_CHECK(metar.WindDirection() == 120);
  BOOST_CHECK(metar.WindSpeed() == 12);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "MPS") == 0);

  BOOST_CHECK(metar.hasMinWindDirection());
  BOOST_CHECK(metar.hasMaxWindDirection());

  BOOST_CHECK(metar.MinWindDirection() == 90);
  BOOST_CHECK(metar.MaxWindDirection() == 150);

  BOOST_CHECK(metar.Visibility() == 1400);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "M") == 0);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == -4);
  BOOST_CHECK(metar.DewPoint() == -7);
  
  BOOST_CHECK(!metar.hasAltimeterA());
  BOOST_CHECK(metar.hasAltimeterQ());
  BOOST_CHECK(metar.AltimeterQ() == 1020);
  
  BOOST_CHECK(!metar.hasSeaLevelPressure());

  BOOST_CHECK(!metar.hasTemperatureNA());
  BOOST_CHECK(!metar.hasDewPointNA());
}

BOOST_AUTO_TEST_CASE(real_METAR_3)
{
  string metar_str = "SPECI KSTL 221513Z 07005KT 2SM -RA BR OVC005 02/02 A3041 RMK AO2 P0001 T00220022";

  Metar metar(metar_str.c_str()); 

  BOOST_CHECK(metar.hasMETAR());
  BOOST_CHECK(strcmp(metar.METAR(), "SPECI") == 0);

  BOOST_CHECK(metar.hasICAO());
  BOOST_CHECK(strcmp(metar.ICAO(), "KSTL") == 0);

  BOOST_CHECK(metar.Day() == 22);
  BOOST_CHECK(metar.Hour() == 15);
  BOOST_CHECK(metar.Minute() == 13);

  BOOST_CHECK(metar.WindDirection() == 70);
  BOOST_CHECK(metar.WindSpeed() == 5);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 2);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == 2);
  BOOST_CHECK(metar.DewPoint() == 2);
 
  BOOST_CHECK(!metar.hasAltimeterQ());
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 30.41);

  BOOST_CHECK(!metar.hasSeaLevelPressure());

  BOOST_CHECK(metar.TemperatureNA() == 2.2);
  BOOST_CHECK(metar.DewPointNA() == 2.2);
}

BOOST_AUTO_TEST_CASE(real_METAR_4)
{
  const char *metar_str = "KSTL 262051Z VRB04KT 10SM CLR 16/M01 A3023 RMK AO2 SLP242 T01561006 57015";

  Metar metar(metar_str); 

  BOOST_CHECK(metar.hasICAO());
  BOOST_CHECK(strcmp(metar.ICAO(), "KSTL") == 0);

  BOOST_CHECK(metar.Day() == 26);
  BOOST_CHECK(metar.Hour() == 20);
  BOOST_CHECK(metar.Minute() == 51);

  BOOST_CHECK(!metar.hasWindDirection());
  BOOST_CHECK(metar.WindSpeed() == 4);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 10);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == 16);
  BOOST_CHECK(metar.DewPoint() == -1);
 
  BOOST_CHECK(!metar.hasAltimeterQ());
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 30.23);

  BOOST_CHECK(metar.hasSeaLevelPressure());
  BOOST_CHECK(metar.SeaLevelPressure() == 1024.2);

  BOOST_CHECK(metar.TemperatureNA() == 15.6);
  BOOST_CHECK(metar.DewPointNA() == -0.6);
}

BOOST_AUTO_TEST_CASE(real_METAR_5)
{
  const char *metar_str = "KHLN 041610Z 28009KT 1/2SM SN FZFG VV007 M10/M12 A2998 RMK AO2 P0001 T11001117";

  Metar metar(metar_str); 

  BOOST_CHECK(metar.hasICAO());
  BOOST_CHECK(strcmp(metar.ICAO(), "KHLN") == 0);

  BOOST_CHECK(metar.Day() == 4);
  BOOST_CHECK(metar.Hour() == 16);
  BOOST_CHECK(metar.Minute() == 10);

  BOOST_CHECK(metar.WindDirection() == 280);
  BOOST_CHECK(metar.WindSpeed() == 9);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(strcmp(metar.WindSpeedUnits(), "KT") == 0);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 0.5);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);

  BOOST_CHECK(metar.hasVerticalVisibility());
  BOOST_CHECK(metar.VerticalVisibility() == 700);

  BOOST_CHECK(metar.Temperature() == -10);
  BOOST_CHECK(metar.DewPoint() == -12);
 
  BOOST_CHECK(!metar.hasAltimeterQ());
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 29.98);

  BOOST_CHECK(!metar.hasSeaLevelPressure());

  BOOST_CHECK(metar.TemperatureNA() == -10.0);
  BOOST_CHECK(metar.DewPointNA() == -11.7);
}
