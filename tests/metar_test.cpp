//
// Copyright (c) 2020 James A. Chappell
//
// METAR decoder tests
//

#include "Metar.h"
#include "Clouds.h"
#include "Phenom.h"

#include <string>

#include <boost/test/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_SUITE(MetarTests)

BOOST_AUTO_TEST_CASE(uninitialized_observation_time)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->Day().has_value());
  BOOST_CHECK(!metar->Hour().has_value());
  BOOST_CHECK(!metar->Minute().has_value());
}

BOOST_AUTO_TEST_CASE(observation_time)
{
  auto metar = Metar::Create("123456Z");

  BOOST_CHECK(metar->Day() == 12);
  BOOST_CHECK(metar->Hour() == 34);
  BOOST_CHECK(metar->Minute() == 56);
}

BOOST_AUTO_TEST_CASE(uninitialized_temperature)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->Temperature().has_value());
  BOOST_CHECK(!metar->DewPoint().has_value());
}

BOOST_AUTO_TEST_CASE(temperature_both_positive1)
{

  char buffer[8];
  strcpy(buffer, "08/06");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == 8);
  BOOST_CHECK(metar->DewPoint() == 6);
}

BOOST_AUTO_TEST_CASE(temperature_negative_dew_point)
{
  char buffer[8];
  strcpy(buffer, "01/M01");
  
  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == 1);
  BOOST_CHECK(metar->DewPoint() == -1);
}

BOOST_AUTO_TEST_CASE(temperature_both_negative)
{
  char buffer[8];
  strcpy(buffer, "M14/M15");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == -14);
  BOOST_CHECK(metar->DewPoint() == -15);
}

BOOST_AUTO_TEST_CASE(temperature_both_positive2)
{
  char buffer[8];
  strcpy(buffer, "15/14");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == 15);
  BOOST_CHECK(metar->DewPoint() == 14);
}

BOOST_AUTO_TEST_CASE(temperature_no_dew_positive)
{
  auto metar = Metar::Create("15/");

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(!metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == 15);
}

BOOST_AUTO_TEST_CASE(temperature_no_dew_negative)
{
  auto metar = Metar::Create("M07/");

  BOOST_CHECK(metar->Temperature().has_value());
  BOOST_CHECK(!metar->DewPoint().has_value());

  BOOST_CHECK(metar->Temperature() == -7);
}

BOOST_AUTO_TEST_CASE(uninitialized_temperatureNA)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->TemperatureNA().has_value());
  BOOST_CHECK(!metar->DewPointNA().has_value());
}

BOOST_AUTO_TEST_CASE(temperatureNA_1)
{
  char buffer[10];
  strcpy(buffer, "T00830067");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->TemperatureNA().has_value());
  BOOST_CHECK(metar->DewPointNA().has_value());

  BOOST_CHECK(metar->TemperatureNA() == 8.3);
  BOOST_CHECK(metar->DewPointNA() == 6.7);
}


BOOST_AUTO_TEST_CASE(temperatureNA_2)
{
  char buffer[10];
  strcpy(buffer, "T01830167");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->TemperatureNA().has_value());
  BOOST_CHECK(metar->DewPointNA().has_value());

  BOOST_CHECK(metar->TemperatureNA() == 18.3);
  BOOST_CHECK(metar->DewPointNA() == 16.7);
}

BOOST_AUTO_TEST_CASE(temperatureNA_both_negative)
{
  char buffer[10];
  strcpy(buffer, "T10171018");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->TemperatureNA().has_value());
  BOOST_CHECK(metar->DewPointNA().has_value());

  BOOST_CHECK(metar->TemperatureNA() == -1.7);
  BOOST_CHECK(metar->DewPointNA() == -1.8);
}

BOOST_AUTO_TEST_CASE(temperatureNA_NoDew)
{
  char buffer[10];
  strcpy(buffer, "T0028");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->TemperatureNA().has_value());
  BOOST_CHECK(!metar->DewPointNA().has_value());

  BOOST_CHECK(metar->TemperatureNA() == 2.8);
}

BOOST_AUTO_TEST_CASE(uninitialized_wind)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->WindSpeed().has_value());
  BOOST_CHECK(!metar->WindDirection().has_value());
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(!metar->WindSpeedUnits().has_value());
}

BOOST_AUTO_TEST_CASE(wind_kt)
{
  char buffer[9];
  strcpy(buffer, "25005KT");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 250);
  BOOST_CHECK(metar->WindSpeed() == 5);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(!metar->WindGust().has_value());
}

BOOST_AUTO_TEST_CASE(wind_kt_3digit)
{
  auto metar = Metar::Create("240105KT");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 240);
  BOOST_CHECK(metar->WindSpeed() == 105);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(!metar->WindGust().has_value());
}

BOOST_AUTO_TEST_CASE(wind_kt_3digit_gust)
{
  auto metar = Metar::Create("240105G121KT");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 240);
  BOOST_CHECK(metar->WindSpeed() == 105);
  
  BOOST_CHECK(metar->WindGust().has_value());
  BOOST_CHECK(metar->WindGust() == 121);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_vrb_kt_3digit_gust)
{
  auto metar = Metar::Create("VRB105G121KT");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(!metar->WindDirection().has_value());
  BOOST_CHECK(metar->isVariableWindDirection());

  BOOST_CHECK(metar->WindSpeed() == 105);
  
  BOOST_CHECK(metar->WindGust().has_value());
  BOOST_CHECK(metar->WindGust() == 121);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_kt_gust)
{
  auto metar = Metar::Create("25005G12KT");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());
  BOOST_CHECK(!metar->isVariableWindDirection());

  BOOST_CHECK(metar->WindDirection() == 250);
  BOOST_CHECK(metar->WindSpeed() == 5);

  BOOST_CHECK(metar->WindGust().has_value());
  BOOST_CHECK(metar->WindGust() == 12);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
}

BOOST_AUTO_TEST_CASE(wind_mps)
{
  char buffer[9];
  strcpy(buffer, "04503MPS");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 45);
  BOOST_CHECK(metar->WindSpeed() == 3);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::MPS);
  
  BOOST_CHECK(!metar->WindGust().has_value());
}

BOOST_AUTO_TEST_CASE(wind_mps_vrb)
{
  auto metar = Metar::Create("VRB03MPS");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(!metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindSpeed() == 3);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::MPS);
  
  BOOST_CHECK(!metar->WindGust().has_value());
}

BOOST_AUTO_TEST_CASE(wind_mps_3digit_gust)
{
  auto metar = Metar::Create("08090G102MPS");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 80);
  BOOST_CHECK(metar->WindSpeed() == 90);
  
  BOOST_CHECK(metar->WindGust().has_value());
  BOOST_CHECK(metar->WindGust() == 102);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::MPS);
}

BOOST_AUTO_TEST_CASE(wind_kph)
{
  char buffer[9];
  strcpy(buffer, "04005KPH");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindDirection() == 40);
  BOOST_CHECK(metar->WindSpeed() == 5);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KPH);
  
  BOOST_CHECK(!metar->WindGust().has_value());
}

BOOST_AUTO_TEST_CASE(wind_vrb_kph_2digit_gust)
{
  auto metar = Metar::Create("VRB05G21KPH");

  BOOST_CHECK(metar->WindSpeed().has_value());
  BOOST_CHECK(!metar->WindDirection().has_value());

  BOOST_CHECK(metar->WindSpeed() == 5);
  
  BOOST_CHECK(metar->WindGust().has_value());
  BOOST_CHECK(metar->WindGust() == 21);

  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KPH);
}

BOOST_AUTO_TEST_CASE(uninitialized_altA)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->AltimeterA().has_value());
}

BOOST_AUTO_TEST_CASE(uninitialized_wind_var)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->MinWindDirection().has_value());
  BOOST_CHECK(!metar->MaxWindDirection().has_value());
}

BOOST_AUTO_TEST_CASE(wind_var)
{
  auto metar = Metar::Create("090V150");

  BOOST_CHECK(metar->MinWindDirection().has_value());
  BOOST_CHECK(metar->MaxWindDirection().has_value());

  BOOST_CHECK(metar->MinWindDirection() == 90);
  BOOST_CHECK(metar->MaxWindDirection() == 150);
}

BOOST_AUTO_TEST_CASE(altA)
{
  char buffer[9];
  strcpy(buffer, "A3006");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 30.06);
}

BOOST_AUTO_TEST_CASE(uninitialized_altQ)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->AltimeterQ().has_value());
}

BOOST_AUTO_TEST_CASE(altQ)
{
  char buffer[9];
  strcpy(buffer, "Q1020");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterQ() == 1020);
}

BOOST_AUTO_TEST_CASE(uninitialized_slp)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->SeaLevelPressure().has_value());
}

BOOST_AUTO_TEST_CASE(slp)
{
  char buffer[9];
  strcpy(buffer, "SLP177");

  auto metar = Metar::Create(buffer);

  BOOST_CHECK(metar->SeaLevelPressure().has_value());
  BOOST_CHECK(metar->SeaLevelPressure() == 1017.7);
}

BOOST_AUTO_TEST_CASE(uninitialized_visibility)
{
  auto metar = Metar::Create("");

  BOOST_CHECK(!metar->Visibility().has_value());
  BOOST_CHECK(!metar->VisibilityUnits().has_value());
}

BOOST_AUTO_TEST_CASE(visibility_meters)
{
  auto metar = Metar::Create("1500");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == 1500);

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::M);
}

BOOST_AUTO_TEST_CASE(visibility_integer_sm)
{
  auto metar = Metar::Create("10SM");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == 10);

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_1)
{
  auto metar = Metar::Create("1/4SM");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == 0.25);

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  BOOST_CHECK(!metar->isVisibilityLessThan());
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_2)
{
  auto metar = Metar::Create("5/16SM");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == (5.0 / 16.0));

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
}

BOOST_AUTO_TEST_CASE(visibility_fraction_sm_3)
{
  auto metar = Metar::Create("2 1/2SM");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == 2.5);

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
}

BOOST_AUTO_TEST_CASE(visibility_LT)
{
  auto metar = Metar::Create("M1/4SM");

  BOOST_CHECK(metar->Visibility().has_value());
  BOOST_CHECK(metar->Visibility() == 0.25);

  BOOST_CHECK(metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  BOOST_CHECK(metar->isVisibilityLessThan());
  BOOST_CHECK(!metar->isCAVOK());
}

BOOST_AUTO_TEST_CASE(visibility_CAVOK)
{
  auto metar = Metar::Create("CAVOK");

  BOOST_CHECK(!metar->Visibility().has_value());
  BOOST_CHECK(!metar->VisibilityUnits().has_value());
  BOOST_CHECK(metar->isCAVOK());
}

BOOST_AUTO_TEST_CASE(uninitialized_vert_visibility)
{
  auto metar = Metar::Create("");
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());
}

BOOST_AUTO_TEST_CASE(vert_visibility)
{
  auto metar = Metar::Create("VV105");
  
  BOOST_CHECK(metar->VerticalVisibility().has_value());
  BOOST_CHECK(metar->VerticalVisibility() == 10500);
}

BOOST_AUTO_TEST_CASE(uninitialized_cloud_layer)
{
  auto metar = Metar::Create("");
  
  BOOST_CHECK(metar->NumCloudLayers() == 0);
  BOOST_CHECK(metar->Layer(0) == nullptr);
}

BOOST_AUTO_TEST_CASE(cloud_layer_2_layers)
{
  auto metar = Metar::Create("BKN004 OVC008");
  
  BOOST_CHECK(metar->NumCloudLayers() == 2);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 4);
  BOOST_CHECK(!metar->Layer(0)->CloudType().has_value());
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 8);
  BOOST_CHECK(!metar->Layer(1)->CloudType().has_value());
}

BOOST_AUTO_TEST_CASE(cloud_layer_3_layers)
{
  auto metar = Metar::Create("FEW004 SCT080 OVC120");
  
  BOOST_CHECK(metar->NumCloudLayers() == 3);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 4);
  BOOST_CHECK(!metar->Layer(0)->CloudType().has_value());
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 80);
  BOOST_CHECK(!metar->Layer(1)->CloudType().has_value());
  BOOST_CHECK(metar->Layer(2)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(2)->Altitude() == 120);
  BOOST_CHECK(!metar->Layer(2)->CloudType().has_value());
}

BOOST_AUTO_TEST_CASE(cloud_layer_3_layers_tempo)
{
  auto metar = Metar::Create("KSTL FEW004 SCT080 TEMPO OVC120");
  
  BOOST_CHECK(metar->NumCloudLayers() == 3);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 4);
  BOOST_CHECK(!metar->Layer(0)->CloudType().has_value());
  BOOST_CHECK(!metar->Layer(0)->Temporary());
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 80);
  BOOST_CHECK(!metar->Layer(1)->CloudType().has_value());
  BOOST_CHECK(!metar->Layer(1)->Temporary());
  BOOST_CHECK(metar->Layer(2)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(2)->Altitude() == 120);
  BOOST_CHECK(!metar->Layer(2)->CloudType().has_value());
  BOOST_CHECK(metar->Layer(2)->Temporary());
}

BOOST_AUTO_TEST_CASE(cloud_layer_3_layers_cloud_types)
{
  auto metar = Metar::Create("FEW004TCU SCT080CB OVC120ACC");
  
  BOOST_CHECK(metar->NumCloudLayers() == 3);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 4);
  BOOST_CHECK(metar->Layer(0)->CloudType() == Clouds::type::TCU);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 80);
  BOOST_CHECK(metar->Layer(1)->CloudType() == Clouds::type::CB);
  BOOST_CHECK(metar->Layer(2)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(2)->Altitude() == 120);
  BOOST_CHECK(metar->Layer(2)->CloudType() == Clouds::type::ACC);
}

BOOST_AUTO_TEST_CASE(phenom_tempo)
{
    auto metar = Metar::Create("EDDH VCBLSN TEMPO SHSN");
  
    BOOST_CHECK(metar->NumPhenomena() == 2);

    BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::SNOW);
    BOOST_CHECK(metar->Phenomenon(0).Vicinity() == true);
    BOOST_CHECK(metar->Phenomenon(0).Blowing() == true);
    BOOST_CHECK(metar->Phenomenon(0).Temporary() == false);
    
    BOOST_CHECK(metar->Phenomenon(1)[0] == Phenom::phenom::SHOWER);
    BOOST_CHECK(metar->Phenomenon(1)[1] == Phenom::phenom::SNOW);
    BOOST_CHECK(metar->Phenomenon(1).Temporary() == true);
}

BOOST_AUTO_TEST_CASE(real_METAR_1)
{
  char buffer[100];
  strcpy(buffer, "KSTL 231751Z 27009KT 10SM OVC015 09/06 A3029 RMK AO2 SLP260 T00940061 10100 20078 53002");

  auto metar = Metar::Create(buffer);
  
  BOOST_CHECK(!metar->MessageType().has_value());

  BOOST_CHECK(metar->ICAO().has_value());
  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 23);
  BOOST_CHECK(metar->Hour() == 17);
  BOOST_CHECK(metar->Minute() == 51);

  BOOST_CHECK(metar->WindDirection() == 270);
  BOOST_CHECK(metar->WindSpeed() == 9);
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar->MinWindDirection().has_value());
  BOOST_CHECK(!metar->MaxWindDirection().has_value());
  
  BOOST_CHECK(metar->Visibility() == 10);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);

  BOOST_CHECK(metar->NumCloudLayers() == 1);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 15);
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == 9);
  BOOST_CHECK(metar->DewPoint() == 6);
  
  BOOST_CHECK(!metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 30.29);

  BOOST_CHECK(metar->SeaLevelPressure().has_value());
  BOOST_CHECK(metar->SeaLevelPressure() == 1026.0);

  BOOST_CHECK(metar->TemperatureNA() == 9.4);
  BOOST_CHECK(metar->DewPointNA() == 6.1);
}

BOOST_AUTO_TEST_CASE(real_METAR_2)
{
  std::string metar_str = "METAR LBBG 041600Z 12012MPS 090V150 1400 R04/P1500N R22/P1500U +SN BKN022 OVC050 M04/M07 Q1020 NOSIG 8849//91=";

  auto metar = Metar::Create(metar_str.c_str());

  BOOST_CHECK(metar->MessageType().has_value());
  BOOST_CHECK(metar->MessageType() == Metar::message_type::METAR);

  BOOST_CHECK(metar->ICAO().has_value());
  BOOST_CHECK(metar->ICAO() == "LBBG");

  BOOST_CHECK(metar->Day() == 4);
  BOOST_CHECK(metar->Hour() == 16);
  BOOST_CHECK(metar->Minute() == 0);

  BOOST_CHECK(metar->WindDirection() == 120);
  BOOST_CHECK(metar->WindSpeed() == 12);
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::MPS);

  BOOST_CHECK(metar->MinWindDirection().has_value());
  BOOST_CHECK(metar->MaxWindDirection().has_value());

  BOOST_CHECK(metar->MinWindDirection() == 90);
  BOOST_CHECK(metar->MaxWindDirection() == 150);

  BOOST_CHECK(metar->Visibility() == 1400);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::M);

  BOOST_CHECK(metar->NumPhenomena() == 1);

  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::SNOW);
  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::HEAVY);
  
  BOOST_CHECK(metar->NumCloudLayers() == 2);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 22);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 50);
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == -4);
  BOOST_CHECK(metar->DewPoint() == -7);
  
  BOOST_CHECK(!metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterQ() == 1020);
  
  BOOST_CHECK(!metar->SeaLevelPressure().has_value());

  BOOST_CHECK(!metar->TemperatureNA().has_value());
  BOOST_CHECK(!metar->DewPointNA().has_value());
}

BOOST_AUTO_TEST_CASE(real_METAR_3)
{
  std::string metar_str = "SPECI KSTL 221513Z 07005KT 2SM -RA BR OVC005 02/02 A3041 RMK AO2 P0001 T00220022";

  auto metar = Metar::Create(metar_str.c_str()); 

  BOOST_CHECK(metar->MessageType().has_value());
  BOOST_CHECK(metar->MessageType() == Metar::message_type::SPECI);

  BOOST_CHECK(metar->ICAO().has_value());
  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 22);
  BOOST_CHECK(metar->Hour() == 15);
  BOOST_CHECK(metar->Minute() == 13);

  BOOST_CHECK(metar->WindDirection() == 70);
  BOOST_CHECK(metar->WindSpeed() == 5);
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar->MinWindDirection().has_value());
  BOOST_CHECK(!metar->MaxWindDirection().has_value());
  
  BOOST_CHECK(metar->Visibility() == 2);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);

  BOOST_CHECK(metar->NumPhenomena() == 2);

  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::RAIN);
  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::LIGHT);

  BOOST_CHECK(metar->Phenomenon(1)[0] == Phenom::phenom::MIST);
  BOOST_CHECK(metar->Phenomenon(1).Intensity() == Phenom::intensity::NORMAL);
  
  BOOST_CHECK(metar->NumCloudLayers() == 1);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 5);
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == 2);
  BOOST_CHECK(metar->DewPoint() == 2);
 
  BOOST_CHECK(!metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 30.41);

  BOOST_CHECK(!metar->SeaLevelPressure().has_value());

  BOOST_CHECK(metar->TemperatureNA() == 2.2);
  BOOST_CHECK(metar->DewPointNA() == 2.2);
}

BOOST_AUTO_TEST_CASE(real_METAR_4)
{
  const char *metar_str = "KSTL 262051Z VRB04KT 10SM CLR 16/M01 A3023 RMK AO2 SLP242 T01561006 57015";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO().has_value());
  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 26);
  BOOST_CHECK(metar->Hour() == 20);
  BOOST_CHECK(metar->Minute() == 51);

  BOOST_CHECK(!metar->WindDirection().has_value());
  BOOST_CHECK(metar->isVariableWindDirection());
  BOOST_CHECK(metar->WindSpeed() == 4);
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar->MinWindDirection().has_value());
  BOOST_CHECK(!metar->MaxWindDirection().has_value());
  
  BOOST_CHECK(metar->Visibility() == 10);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  
  BOOST_CHECK(metar->NumPhenomena() == 0);

  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::NONE);
  BOOST_CHECK(metar->Phenomenon(1)[1] == Phenom::phenom::NONE);
  
  BOOST_CHECK(metar->NumCloudLayers() == 1);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::CLR);
  BOOST_CHECK(!metar->Layer(0)->Altitude().has_value());
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == 16);
  BOOST_CHECK(metar->DewPoint() == -1);
 
  BOOST_CHECK(!metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 30.23);

  BOOST_CHECK(metar->SeaLevelPressure().has_value());
  BOOST_CHECK(metar->SeaLevelPressure() == 1024.2);

  BOOST_CHECK(metar->TemperatureNA() == 15.6);
  BOOST_CHECK(metar->DewPointNA() == -0.6);
}

BOOST_AUTO_TEST_CASE(real_METAR_5)
{
  const char *metar_str = "KHLN 041610Z 28009KT 1/2SM SN FZFG VV007 M10/M12 A2998 RMK AO2 P0001 T11001117";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO().has_value());
  BOOST_CHECK(metar->ICAO() == "KHLN");

  BOOST_CHECK(metar->Day() == 4);
  BOOST_CHECK(metar->Hour() == 16);
  BOOST_CHECK(metar->Minute() == 10);

  BOOST_CHECK(metar->WindDirection() == 280);
  BOOST_CHECK(metar->WindSpeed() == 9);
  BOOST_CHECK(!metar->WindGust().has_value());
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(!metar->MinWindDirection().has_value());
  BOOST_CHECK(!metar->MaxWindDirection().has_value());

  BOOST_CHECK(metar->NumPhenomena() == 2);

  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::SNOW);
  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::NORMAL);

  BOOST_CHECK(metar->Phenomenon(1)[0] == Phenom::phenom::FOG);
  BOOST_CHECK(metar->Phenomenon(1).Freezing() == true);
  BOOST_CHECK(metar->Phenomenon(1).Intensity() == Phenom::intensity::NORMAL);
  
  BOOST_CHECK(metar->Visibility() == 0.5);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  
  BOOST_CHECK(metar->NumCloudLayers() == 0);

  BOOST_CHECK(metar->VerticalVisibility().has_value());
  BOOST_CHECK(metar->VerticalVisibility() == 700);

  BOOST_CHECK(metar->Temperature() == -10);
  BOOST_CHECK(metar->DewPoint() == -12);
 
  BOOST_CHECK(!metar->AltimeterQ().has_value());
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 29.98);

  BOOST_CHECK(!metar->SeaLevelPressure().has_value());

  BOOST_CHECK(metar->TemperatureNA() == -10.0);
  BOOST_CHECK(metar->DewPointNA() == -11.7);
}

BOOST_AUTO_TEST_CASE(real_METAR_6)
{
  const char *metar_str = "KSTL 051520Z 12017KT 5SM -TSRA BR OVC007CB 06/05 A2989 RMK AO2 LTG DSNT SE OCNL LTGIC SE TSB0854 TS SE MOV NE P0004 T00560050";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 5);
  BOOST_CHECK(metar->Hour() == 15);
  BOOST_CHECK(metar->Minute() == 20);

  BOOST_CHECK(metar->WindDirection() == 120);
  BOOST_CHECK(metar->WindSpeed() == 17);
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(metar->Visibility() == 5);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);

  BOOST_CHECK(metar->NumPhenomena() == 2);

  BOOST_CHECK(metar->Phenomenon(0).ThunderStorm());
  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::RAIN);
  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::LIGHT);

  BOOST_CHECK(metar->Phenomenon(1)[0] == Phenom::phenom::MIST);
  BOOST_CHECK(metar->Phenomenon(1).Intensity() == Phenom::intensity::NORMAL);
  
  BOOST_CHECK(metar->NumCloudLayers() == 1);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 7);
  BOOST_CHECK(metar->Layer(0)->CloudType() == Clouds::type::CB);
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == 6);
  BOOST_CHECK(metar->DewPoint() == 5);
 
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 29.89);

  BOOST_CHECK(metar->TemperatureNA() == 5.6);
  BOOST_CHECK(metar->DewPointNA() == 5);
}

BOOST_AUTO_TEST_CASE(real_METAR_7)
{
  const char *metar_str = "KSTL 091651Z 10010KT 060V120 10SM FEW120 BKN250 07/M06 A2998 RMK AO2 SLP160 T00671056";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 9);
  BOOST_CHECK(metar->Hour() == 16);
  BOOST_CHECK(metar->Minute() == 51);

  BOOST_CHECK(metar->WindDirection() == 100);
  BOOST_CHECK(metar->WindSpeed() == 10);
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);

  BOOST_CHECK(metar->MinWindDirection() == 60);
  BOOST_CHECK(metar->MaxWindDirection() == 120);
  
  BOOST_CHECK(metar->Visibility() == 10);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  
  BOOST_CHECK(metar->NumCloudLayers() == 2);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 120);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 250);
  
  BOOST_CHECK(!metar->VerticalVisibility().has_value());

  BOOST_CHECK(metar->Temperature() == 7);
  BOOST_CHECK(metar->DewPoint() == -6);
 
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 29.98);

  BOOST_CHECK(metar->TemperatureNA() == 6.7);
  BOOST_CHECK(metar->DewPointNA() == -5.6);
  
  BOOST_CHECK(metar->SeaLevelPressure() == 1016.0);
}

BOOST_AUTO_TEST_CASE(real_METAR_8)
{
  const char *metar_str = "KSTL 192051Z 20004KT 10SM -RA FEW034 SCT048 OVC110 22/18 A2993 RMK AO2 PK WND 27032/2004 LTG DSNT E AND SE RAB06 TSB03E42 PRESFR SLP129 OCNL LTGIC DSNT E CB DSNT E MOV E P0003 60003 T02220178 58006 $";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 19);
  BOOST_CHECK(metar->Hour() == 20);
  BOOST_CHECK(metar->Minute() == 51);

  BOOST_CHECK(metar->WindDirection() == 200);
  BOOST_CHECK(metar->WindSpeed() == 4);
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(metar->Visibility() == 10);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  
  BOOST_CHECK(metar->NumPhenomena() == 1);

  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::RAIN);
  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::LIGHT);
  
  BOOST_CHECK(metar->NumCloudLayers() == 3);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 34);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 48);
  BOOST_CHECK(metar->Layer(2)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(2)->Altitude() == 110);
  
  BOOST_CHECK(metar->Temperature() == 22);
  BOOST_CHECK(metar->DewPoint() == 18);
 
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 29.93);

  BOOST_CHECK(metar->TemperatureNA() == 22.2);
  BOOST_CHECK(metar->DewPointNA() == 17.8);
  
  BOOST_CHECK(metar->SeaLevelPressure() == 1012.9);
}


BOOST_AUTO_TEST_CASE(real_METAR_9)
{
  const char *metar_str = "KSTL 261605Z 10006KT 7SM -TSRA FEW050CB OVC090 06/01 A3014 RMK AO2 LTG DSNT S AND SW TSB05 OCNL LTGIC SW-W TS SW-W MOV NE P0001 T00610006";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 26);
  BOOST_CHECK(metar->Hour() == 16);
  BOOST_CHECK(metar->Minute() == 5);

  BOOST_CHECK(metar->WindDirection() == 100);
  BOOST_CHECK(metar->WindSpeed() == 6);
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(metar->Visibility() == 7);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);

  BOOST_CHECK(metar->NumPhenomena() == 1);

  BOOST_CHECK(metar->Phenomenon(0).Intensity() == Phenom::intensity::LIGHT);
  BOOST_CHECK(metar->Phenomenon(0).ThunderStorm());
  BOOST_CHECK(metar->Phenomenon(0)[0] == Phenom::phenom::RAIN);
  
  BOOST_CHECK(metar->NumCloudLayers() == 2);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 50);
  BOOST_CHECK(metar->Layer(0)->CloudType() == Clouds::type::CB);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 90);
  
  BOOST_CHECK(metar->Temperature() == 6);
  BOOST_CHECK(metar->DewPoint() == 1);
 
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 30.14);

  BOOST_CHECK(metar->TemperatureNA() == 6.1);
  BOOST_CHECK(metar->DewPointNA() == 0.6);
}

BOOST_AUTO_TEST_CASE(real_METAR_10)
{
  const char *metar_str = "KSTL 162025Z 24004KT 10SM FEW039 SCT060 BKN090 BKN250 22/17 A2953 RMK AO2 WSHFT 1938 RAE24 TSE24 OCNL LTGICCCCG DSNT N-S CB DSNT N-S MOV NE P0015 T02170172\n U";

  auto metar = Metar::Create(metar_str); 

  BOOST_CHECK(metar->ICAO() == "KSTL");

  BOOST_CHECK(metar->Day() == 16);
  BOOST_CHECK(metar->Hour() == 20);
  BOOST_CHECK(metar->Minute() == 25);

  BOOST_CHECK(metar->WindDirection() == 240);
  BOOST_CHECK(metar->WindSpeed() == 4);
  BOOST_CHECK(metar->WindSpeedUnits() == Metar::speed_units::KT);
  
  BOOST_CHECK(metar->Visibility() == 10);
  BOOST_CHECK(metar->VisibilityUnits() == Metar::distance_units::SM);
  
  BOOST_CHECK(metar->NumCloudLayers() == 4);
  BOOST_CHECK(metar->Layer(0)->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(metar->Layer(0)->Altitude() == 39);
  BOOST_CHECK(metar->Layer(1)->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(metar->Layer(1)->Altitude() == 60);
  BOOST_CHECK(metar->Layer(2)->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(metar->Layer(2)->Altitude() == 90);
  BOOST_CHECK(metar->Layer(3)->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(metar->Layer(3)->Altitude() == 250);
  
  BOOST_CHECK(metar->Temperature() == 22);
  BOOST_CHECK(metar->DewPoint() == 17);
 
  BOOST_CHECK(metar->AltimeterA().has_value());
  BOOST_CHECK(metar->AltimeterA() == 29.53);

  BOOST_CHECK(metar->TemperatureNA() == 21.7);
  BOOST_CHECK(metar->DewPointNA() == 17.2);
}

BOOST_AUTO_TEST_SUITE_END()
