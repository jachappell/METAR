//
// Copyright (c) 2018 James A. Chappell
//
// METAR decoder tests
//

#include "Phenom.h"

#include <string>

#define BOOST_TEST_MODULE METAR
#include <boost/test/included/unit_test.hpp>

using namespace Storage_B::Weather;

BOOST_AUTO_TEST_CASE(phenoms)
{
  {
    auto result = Phenom::Create("BR");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::MIST);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
    BOOST_CHECK(p.Blowing() == false);
    BOOST_CHECK(p.Freezing() == false);
    BOOST_CHECK(p.Drifting() == false);
    BOOST_CHECK(p.Vicinity() == false);
    BOOST_CHECK(p.Partial() == false);
    BOOST_CHECK(p.Shallow() == false);
    BOOST_CHECK(p.Patches() == false);
    BOOST_CHECK(p.Temporary() == false);
    
    BOOST_CHECK(p[1] == Phenom::phenom::NONE);
  }

  {
    auto result = Phenom::Create("DS");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::DUST_STORM);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("DU");

    const auto& p = *result;

    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::DUST);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("DZ");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::DRIZZLE);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("FC");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::FUNNEL_CLOUD);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("FG");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::FOG);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("FU");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SMOKE);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("GR");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::HAIL);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("GS");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SMALL_HAIL);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("HZ");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::HAZE);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("IC");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::ICE_CRYSTALS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("PE");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::ICE_PELLETS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("PL");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::ICE_PELLETS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("PO");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::DUST_SAND_WHORLS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("PY");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SPRAY);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("RA");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::RAIN);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("SA");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SAND);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("SG");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SNOW_GRAINS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("SN");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SNOW);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("SQ");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SQUALLS);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("SS");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::SAND_STORM);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("TS");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::THUNDER_STORM);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("UP");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::UNKNOWN_PRECIP);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("VA");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 1);
    BOOST_CHECK(p[0] == Phenom::phenom::VOLCANIC_ASH);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }
}

BOOST_AUTO_TEST_CASE(phenom_intensity)
{
  {
    auto result = Phenom::Create("RA");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::RAIN);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::NORMAL);
  }

  {
    auto result = Phenom::Create("-RA");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::RAIN);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::LIGHT);
  }

  {
    auto result = Phenom::Create("+RA");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::RAIN);
    BOOST_CHECK(p.Intensity() == Phenom::intensity::HEAVY);
  }
}
BOOST_AUTO_TEST_CASE(phenom_proximity)
{
    auto result = Phenom::Create("VCFG");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::FOG);
    BOOST_CHECK(p.Vicinity() == true);
}

BOOST_AUTO_TEST_CASE(phenom_blowing)
{
    auto result = Phenom::Create("BLSN");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::SNOW);
    BOOST_CHECK(p.Blowing() == true);
}

BOOST_AUTO_TEST_CASE(phenom_drifting)
{
    auto result = Phenom::Create("DRSA");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::SAND);
    BOOST_CHECK(p.Drifting() == true);
}

BOOST_AUTO_TEST_CASE(phenom_freezing)
{
    auto result = Phenom::Create("FZDZ");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::DRIZZLE);
    BOOST_CHECK(p.Freezing() == true);
}

BOOST_AUTO_TEST_CASE(phenom_shower)
{
    auto result = Phenom::Create("SHRA");

    const auto& p = *result;
  
    BOOST_CHECK(p.NumPhenom() == 2);
    BOOST_CHECK(p[0] == Phenom::phenom::SHOWER);
    BOOST_CHECK(p[1] == Phenom::phenom::RAIN);
}

BOOST_AUTO_TEST_CASE(phenom_shower_vicinity)
{
    auto result = Phenom::Create("VCSH");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::SHOWER);
    BOOST_CHECK(p.Vicinity() == true);
}

BOOST_AUTO_TEST_CASE(phenom_partial)
{
    auto result = Phenom::Create("PRFG");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::FOG);
    BOOST_CHECK(p.Partial() == true);
}

BOOST_AUTO_TEST_CASE(phenom_shallow)
{
    auto result = Phenom::Create("MIFG");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::FOG);
    BOOST_CHECK(p.Shallow() == true);
}

BOOST_AUTO_TEST_CASE(phenom_patches)
{
    auto result = Phenom::Create("BCFG");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::FOG);
    BOOST_CHECK(p.Patches() == true);
}

BOOST_AUTO_TEST_CASE(phenom_vicinity_blowing)
{
    auto result = Phenom::Create("VCBLSN");

    const auto& p = *result;
  
    BOOST_CHECK(p[0] == Phenom::phenom::SNOW);
    BOOST_CHECK(p.Vicinity() == true);
    BOOST_CHECK(p.Blowing() == true);
}
