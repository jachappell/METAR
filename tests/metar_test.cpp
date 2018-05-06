//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder tests
//

#include "Metar.h"

#include <string>

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(uninitialized_observation_time)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasDay());
  BOOST_CHECK(!metar.hasHour());
  BOOST_CHECK(!metar.hasMinute());
}

BOOST_AUTO_TEST_CASE(observation_time)
{
  Metar metar("123456Z");

  BOOST_CHECK(metar.Day() == 12);
  BOOST_CHECK(metar.Hour() == 34);
  BOOST_CHECK(metar.Minute() == 56);
}

BOOST_AUTO_TEST_CASE(uninitialized_temperature)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasTemperature());
  BOOST_CHECK(!metar.hasDewPoint());
}

BOOST_AUTO_TEST_CASE(temperature_both_positive1)
{

  char buffer[8];
  strcpy(buffer, "08/06");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 8);
  BOOST_CHECK(metar.DewPoint() == 6);
}

BOOST_AUTO_TEST_CASE(temperature_negative_dew_point)
{
  char buffer[8];
  strcpy(buffer, "01/M01");
  
  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 1);
  BOOST_CHECK(metar.DewPoint() == -1);
}

BOOST_AUTO_TEST_CASE(temperature_both_negative)
{
  char buffer[8];
  strcpy(buffer, "M14/M15");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == -14);
  BOOST_CHECK(metar.DewPoint() == -15);
}

BOOST_AUTO_TEST_CASE(temperature_both_positive2)
{
  char buffer[8];
  strcpy(buffer, "15/14");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 15);
  BOOST_CHECK(metar.DewPoint() == 14);
}

BOOST_AUTO_TEST_CASE(temperature_no_dew_positive)
{
  Metar metar("15/");

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(!metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == 15);
}

BOOST_AUTO_TEST_CASE(temperature_no_dew_negative)
{
  Metar metar("M07/");

  BOOST_CHECK(metar.hasTemperature());
  BOOST_CHECK(!metar.hasDewPoint());

  BOOST_CHECK(metar.Temperature() == -7);
}

BOOST_AUTO_TEST_CASE(uninitialized_temperatureNA)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasTemperatureNA());
  BOOST_CHECK(!metar.hasDewPointNA());
}

BOOST_AUTO_TEST_CASE(temperatureNA_1)
{
  char buffer[10];
  strcpy(buffer, "T00830067");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == 8.3);
  BOOST_CHECK(metar.DewPointNA() == 6.7);
}


BOOST_AUTO_TEST_CASE(temperatureNA_2)
{
  char buffer[10];
  strcpy(buffer, "T01830167");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == 18.3);
  BOOST_CHECK(metar.DewPointNA() == 16.7);
}

BOOST_AUTO_TEST_CASE(temperatureNA_both_negative)
{
  char buffer[10];
  strcpy(buffer, "T10171018");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasTemperatureNA());
  BOOST_CHECK(metar.hasDewPointNA());

  BOOST_CHECK(metar.TemperatureNA() == -1.7);
  BOOST_CHECK(metar.DewPointNA() == -1.8);
}

BOOST_AUTO_TEST_CASE(uninitialized_wind)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(!metar.hasWindSpeedUnits());
}

BOOST_AUTO_TEST_CASE(wind_kt)
{
  char buffer[9];
  strcpy(buffer, "25005KT");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 250);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(wind_kt_3digit)
{
  Metar metar("240105KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 240);
  BOOST_CHECK(metar.WindSpeed() == 105);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(wind_kt_3digit_gust)
{
  Metar metar("240105G121KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 240);
  BOOST_CHECK(metar.WindSpeed() == 105);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 121);

  BOOST_CHECK(metar.WindSpeedUnits() ==  Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_vrb_kt_3digit_gust)
{
  Metar metar("VRB105G121KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());
  BOOST_CHECK(metar.isVariableWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 105);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 121);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_kt_gust)
{
  Metar metar("25005G12KT");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());
  BOOST_CHECK(!metar.isVariableWindDirection());

  BOOST_CHECK(metar.WindDirection() == 250);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 12);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_mps)
{
  char buffer[9];
  strcpy(buffer, "04503MPS");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 45);
  BOOST_CHECK(metar.WindSpeed() == 3);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::MPS);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(wind_mps_vrb)
{
  Metar metar("VRB03MPS");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 3);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::MPS);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(wind_mps_3digit_gust)
{
  Metar metar("08090G102MPS");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 80);
  BOOST_CHECK(metar.WindSpeed() == 90);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 102);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::MPS);
}

BOOST_AUTO_TEST_CASE(wind_kph)
{
  char buffer[9];
  strcpy(buffer, "04005KPH");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(metar.hasWindDirection());

  BOOST_CHECK(metar.WindDirection() == 40);
  BOOST_CHECK(metar.WindSpeed() == 5);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KPH);
  
  BOOST_CHECK(!metar.hasWindGust());
}

BOOST_AUTO_TEST_CASE(wind_vrb_kph_2digit_gust)
{
  Metar metar("VRB05G21KPH");

  BOOST_CHECK(metar.hasWindSpeed());
  BOOST_CHECK(!metar.hasWindDirection());

  BOOST_CHECK(metar.WindSpeed() == 5);
  
  BOOST_CHECK(metar.hasWindGust());
  BOOST_CHECK(metar.WindGust() == 21);

  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KPH);
}

BOOST_AUTO_TEST_CASE(uninitialized_altA)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasAltimeterA());
}

BOOST_AUTO_TEST_CASE(uninitialized_wind_var)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
}

BOOST_AUTO_TEST_CASE(wind_var)
{
  Metar metar("090V150");

  BOOST_CHECK(metar.hasMinWindDirection());
  BOOST_CHECK(metar.hasMaxWindDirection());

  BOOST_CHECK(metar.MinWindDirection() == 90);
  BOOST_CHECK(metar.MaxWindDirection() == 150);
}

BOOST_AUTO_TEST_CASE(altA)
{
  char buffer[9];
  strcpy(buffer, "A3006");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 30.06);
}

BOOST_AUTO_TEST_CASE(uninitialized_altQ)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasAltimeterQ());
}

BOOST_AUTO_TEST_CASE(altQ)
{
  char buffer[9];
  strcpy(buffer, "Q1020");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasAltimeterQ());
  BOOST_CHECK(metar.AltimeterQ() == 1020);
}

BOOST_AUTO_TEST_CASE(uninitialized_slp)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasSeaLevelPressure());
}

BOOST_AUTO_TEST_CASE(slp)
{
  char buffer[9];
  strcpy(buffer, "SLP177");

  Metar metar(buffer);

  BOOST_CHECK(metar.hasSeaLevelPressure());
  BOOST_CHECK(metar.SeaLevelPressure() == 1017.7);
}

BOOST_AUTO_TEST_CASE(uninitialized_visibility)
{
  Metar metar("");

  BOOST_CHECK(!metar.hasVisibility());
  BOOST_CHECK(!metar.hasVisibilityUnits());
}

BOOST_AUTO_TEST_CASE(visibility_meters)
{
  Metar metar("1500");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 1500);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "M") == 0);
}

BOOST_AUTO_TEST_CASE(visibility_integer_sm)
{
  Metar metar("10SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 10);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_1)
{
  Metar metar("1/4SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 0.25);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  BOOST_CHECK(!metar.isVisibilityLessThan());
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_2)
{
  Metar metar("5/16SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == (5.0 / 16.0));

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_3)
{
  Metar metar("2 1/2SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 2.5);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
}

BOOST_AUTO_TEST_CASE(visibility_LT)
{
  Metar metar("M1/4SM");

  BOOST_CHECK(metar.hasVisibility());
  BOOST_CHECK(metar.Visibility() == 0.25);

  BOOST_CHECK(metar.hasVisibilityUnits());
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  BOOST_CHECK(metar.isVisibilityLessThan());
  BOOST_CHECK(!metar.isCAVOK());
}

BOOST_AUTO_TEST_CASE(visibility_CAVOK)
{
  Metar metar("CAVOK");

  BOOST_CHECK(!metar.hasVisibility());
  BOOST_CHECK(!metar.hasVisibilityUnits());
  BOOST_CHECK(metar.isCAVOK());
}

BOOST_AUTO_TEST_CASE(uninitialized_vert_visibility)
{
  Metar metar("");
  
  BOOST_CHECK(!metar.hasVerticalVisibility());
}

BOOST_AUTO_TEST_CASE(vert_visibility)
{
  Metar metar("VV105");
  
  BOOST_CHECK(metar.hasVerticalVisibility());
  BOOST_CHECK(metar.VerticalVisibility() == 10500);
}

BOOST_AUTO_TEST_CASE(uninitialized_cloud_layer)
{
  Metar metar("");
  
  BOOST_CHECK(metar.NumCloudLayers() == 0);
  BOOST_CHECK(metar.Layer(0) == nullptr);
}

BOOST_AUTO_TEST_CASE(cloud_layer_CLR)
{
  Metar metar("CLR");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::CLR);
  BOOST_CHECK(!metar.Layer(0)->hasAltitude());
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_SKC)
{
  Metar metar("SKC");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::SKC);
  BOOST_CHECK(!metar.Layer(0)->hasAltitude());
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_NSC)
{
  Metar metar("NSC");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::NSC);
  BOOST_CHECK(!metar.Layer(0)->hasAltitude());
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_FEW)
{
  Metar metar("FEW105");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::FEW);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 10500);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_SCT)
{
  Metar metar("SCT045");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::SCT);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 4500);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_BKN)
{
  Metar metar("BKN005");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::BKN);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 500);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_OVC)
{
  Metar metar("OVC050");
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 5000);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_2_layers)
{
  Metar metar("BKN004 OVC008");
  
  BOOST_CHECK(metar.NumCloudLayers() == 2);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::BKN);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 400);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
  BOOST_CHECK(metar.Layer(1)->Cover() ==  Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(1)->Altitude() == 800);
  BOOST_CHECK(!metar.Layer(1)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_3_layers)
{
  Metar metar("FEW004 SCT080 OVC120");
  
  BOOST_CHECK(metar.NumCloudLayers() == 3);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::FEW);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 400);
  BOOST_CHECK(!metar.Layer(0)->hasCloudType());
  BOOST_CHECK(metar.Layer(1)->Cover() == Metar::SkyCondition::cover::SCT);
  BOOST_CHECK(metar.Layer(1)->Altitude() == 8000);
  BOOST_CHECK(!metar.Layer(1)->hasCloudType());
  BOOST_CHECK(metar.Layer(2)->Cover() == Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(2)->Altitude() == 12000);
  BOOST_CHECK(!metar.Layer(2)->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_3_layers_cloud_types)
{
  Metar metar("FEW004TCU SCT080CB OVC120ACC");
  
  BOOST_CHECK(metar.NumCloudLayers() == 3);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::FEW);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 400);
  BOOST_CHECK(metar.Layer(0)->CloudType() ==  Metar::SkyCondition::type::TCU);
  BOOST_CHECK(metar.Layer(1)->Cover() == Metar::SkyCondition::cover::SCT);
  BOOST_CHECK(metar.Layer(1)->Altitude() == 8000);
  BOOST_CHECK(metar.Layer(1)->CloudType() == Metar::SkyCondition::type::CB);
  BOOST_CHECK(metar.Layer(2)->Cover() ==  Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(2)->Altitude() == 12000);
  BOOST_CHECK(metar.Layer(2)->CloudType() == Metar::SkyCondition::type::ACC);
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
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 10);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);

  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 1500);
  
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
  std::string metar_str = "METAR LBBG 041600Z 12012MPS 090V150 1400 R04/P1500N R22/P1500U +SN BKN022 OVC050 M04/M07 Q1020 NOSIG 8849//91=";

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
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::MPS);

  BOOST_CHECK(metar.hasMinWindDirection());
  BOOST_CHECK(metar.hasMaxWindDirection());

  BOOST_CHECK(metar.MinWindDirection() == 90);
  BOOST_CHECK(metar.MaxWindDirection() == 150);

  BOOST_CHECK(metar.Visibility() == 1400);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "M") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 2);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::BKN);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 2200);
  BOOST_CHECK(metar.Layer(1)->Cover() ==  Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(1)->Altitude() == 5000);
  
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
  std::string metar_str = "SPECI KSTL 221513Z 07005KT 2SM -RA BR OVC005 02/02 A3041 RMK AO2 P0001 T00220022";

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
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 2);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 500);
  
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
  BOOST_CHECK(metar.isVariableWindDirection());
  BOOST_CHECK(metar.WindSpeed() == 4);
  BOOST_CHECK(!metar.hasWindGust());
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 10);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::CLR);
  BOOST_CHECK(!metar.Layer(0)->hasAltitude());
  
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
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar.hasMinWindDirection());
  BOOST_CHECK(!metar.hasMaxWindDirection());
  
  BOOST_CHECK(metar.Visibility() == 0.5);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 0);

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

BOOST_AUTO_TEST_CASE(real_METAR_6)
{
  const char *metar_str = "KSTL 051520Z 12017KT 5SM -TSRA BR OVC007CB 06/05 A2989 RMK AO2 LTG DSNT SE OCNL LTGIC SE TSB0854 TS SE MOV NE P0004 T00560050";

  Metar metar(metar_str); 

  BOOST_CHECK(strcmp(metar.ICAO(), "KSTL") == 0);

  BOOST_CHECK(metar.Day() == 5);
  BOOST_CHECK(metar.Hour() == 15);
  BOOST_CHECK(metar.Minute() == 20);

  BOOST_CHECK(metar.WindDirection() == 120);
  BOOST_CHECK(metar.WindSpeed() == 17);
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(metar.Visibility() == 5);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 1);
  BOOST_CHECK(metar.Layer(0)->Cover() == Metar::SkyCondition::cover::OVC);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 700);
  BOOST_CHECK(metar.Layer(0)->CloudType() == Metar::SkyCondition::type::CB);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == 6);
  BOOST_CHECK(metar.DewPoint() == 5);
 
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 29.89);

  BOOST_CHECK(metar.TemperatureNA() == 5.6);
  BOOST_CHECK(metar.DewPointNA() == 5);
}

BOOST_AUTO_TEST_CASE(real_METAR_7)
{
  const char *metar_str = "KSTL 091651Z 10010KT 060V120 10SM FEW120 BKN250 07/M06 A2998 RMK AO2 SLP160 T00671056";

  Metar metar(metar_str); 

  BOOST_CHECK(strcmp(metar.ICAO(), "KSTL") == 0);

  BOOST_CHECK(metar.Day() == 9);
  BOOST_CHECK(metar.Hour() == 16);
  BOOST_CHECK(metar.Minute() == 51);

  BOOST_CHECK(metar.WindDirection() == 100);
  BOOST_CHECK(metar.WindSpeed() == 10);
  BOOST_CHECK(metar.WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(metar.MinWindDirection() == 60);
  BOOST_CHECK(metar.MaxWindDirection() == 120);
  
  BOOST_CHECK(metar.Visibility() == 10);
  BOOST_CHECK(strcmp(metar.VisibilityUnits(), "SM") == 0);
  
  BOOST_CHECK(metar.NumCloudLayers() == 2);
  BOOST_CHECK(metar.Layer(0)->Cover() ==  Metar::SkyCondition::cover::FEW);
  BOOST_CHECK(metar.Layer(0)->Altitude() == 12000);
  BOOST_CHECK(metar.Layer(1)->Cover() == Metar::SkyCondition::cover::BKN);
  BOOST_CHECK(metar.Layer(1)->Altitude() == 25000);
  
  BOOST_CHECK(!metar.hasVerticalVisibility());

  BOOST_CHECK(metar.Temperature() == 7);
  BOOST_CHECK(metar.DewPoint() == -6);
 
  BOOST_CHECK(metar.hasAltimeterA());
  BOOST_CHECK(metar.AltimeterA() == 29.98);

  BOOST_CHECK(metar.TemperatureNA() == 6.7);
  BOOST_CHECK(metar.DewPointNA() == -5.6);
  
  BOOST_CHECK(metar.SeaLevelPressure() == 1016.0);
}

