//
// Copyright (c) 2018 James A. Chappell
//
// Conversion tests
//

#include "Utils.h"

#include <iostream>

using namespace std;

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(humidity_100)
{
  BOOST_TEST(Utils::Humidity(20.0, 20.0) == 100.0);
}

BOOST_AUTO_TEST_CASE(humidity_50, * boost::unit_test::tolerance(0.0001))
{
  BOOST_TEST(Utils::Humidity(20.0, 9.261352) == 50.0);
}

BOOST_AUTO_TEST_CASE(wind_chill_none)
{
  BOOST_TEST(Utils::WindChill(5.0, 4.8) == 5.0);
  BOOST_TEST(Utils::WindChill(15.0, 10.0) == 15);
}

BOOST_AUTO_TEST_CASE(wind_chill, * boost::unit_test::tolerance(0.05))
{
  BOOST_TEST(Utils::WindChill(5.0, 13.0) == 2.1);
  BOOST_TEST(Utils::WindChill(10.0, 20.0) == 7.4);
  BOOST_TEST(Utils::WindChill(0.0, 50.0) == -8.1);
}

