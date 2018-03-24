//
// Copyright (c) 2018 James A. Chappell
//
// Conversion tests
//

#include "Convert.h"

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(to_fahrenheit)
{
  BOOST_TEST(Convert::c2f(0.0) == 32.0);
  BOOST_TEST(Convert::c2f(100.0) == 212.0);
  BOOST_TEST(Convert::c2f(-40.0) == -40.0);
}

BOOST_AUTO_TEST_CASE(to_celsius)
{
  BOOST_TEST(Convert::f2c(32.0) == 0.0);
  BOOST_TEST(Convert::f2c(212.0) == 100.0);
  BOOST_TEST(Convert::f2c(-40.0) == -40.0);
}
