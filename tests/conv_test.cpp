//
// Copyright (c) 2018 James A. Chappell
//
// Conversion tests
//

#include "Convert.h"

#include <string>

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(to_fahrenheit)
{
   BOOST_CHECK(Convert::c2f(0.0) == 32.0);
   BOOST_CHECK(Convert::c2f(100.0) == 212.0);
   BOOST_CHECK(Convert::c2f(-40.0) == -40.0);
}
