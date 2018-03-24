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
