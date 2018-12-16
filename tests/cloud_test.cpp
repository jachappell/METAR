//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder tests
//

#include "Clouds.h"

#include <string>

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(cloud_layer_CLR)
{
  auto result = Clouds::Create("CLR");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::CLR);
  BOOST_CHECK(!result->hasAltitude());
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_SKC)
{
  auto result = Clouds::Create("SKC");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::SKC);
  BOOST_CHECK(!result->hasAltitude());
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_NSC)
{
  auto result = Clouds::Create("NSC");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::NSC);
  BOOST_CHECK(!result->hasAltitude());
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_FEW)
{
  auto result = Clouds::Create("FEW105");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::FEW);
  BOOST_CHECK(result->Altitude() == 105);
  BOOST_CHECK(!result->hasCloudType());
  BOOST_CHECK(!result->Temporary());
}

BOOST_AUTO_TEST_CASE(cloud_layer_SCT)
{
  auto result = Clouds::Create("SCT045");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::SCT);
  BOOST_CHECK(result->Altitude() == 45);
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_BKN)
{
  auto result = Clouds::Create("BKN005");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::BKN);
  BOOST_CHECK(result->Altitude() == 5);
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_OVC)
{
  auto result = Clouds::Create("OVC050");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(result->Altitude() == 50);
  BOOST_CHECK(!result->hasCloudType());
}

BOOST_AUTO_TEST_CASE(cloud_layer_TCU)
{
  auto result = Clouds::Create("OVC050TCU");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(result->Altitude() == 50);
  BOOST_CHECK(result->hasCloudType());
  BOOST_CHECK(result->CloudType() == Clouds::type::TCU);
}

BOOST_AUTO_TEST_CASE(cloud_layer_CB)
{
  auto result = Clouds::Create("OVC050CB");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(result->Altitude() == 50);
  BOOST_CHECK(result->hasCloudType());
  BOOST_CHECK(result->CloudType() == Clouds::type::CB);
}

BOOST_AUTO_TEST_CASE(cloud_layer_ACC)
{
  auto result = Clouds::Create("OVC050ACC");
  
  BOOST_CHECK(result->Cover() == Clouds::cover::OVC);
  BOOST_CHECK(result->Altitude() == 50);
  BOOST_CHECK(result->hasCloudType());
  BOOST_CHECK(result->CloudType() == Clouds::type::ACC);
}
