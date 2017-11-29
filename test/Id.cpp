#include "Id.h"
#include "BoostUnitTest.h"

BOOST_AUTO_TEST_CASE(IdAtZero)
{
    EDMD::Id id(0);
    BOOST_CHECK_EQUAL(id(), 0);
    BOOST_CHECK_EQUAL(id(), 1);
}

BOOST_AUTO_TEST_CASE(IdAtN)
{
    EDMD::Id id(42);
    BOOST_CHECK_EQUAL(id(), 42);
    BOOST_CHECK_EQUAL(id(), 43);
}


BOOST_AUTO_TEST_CASE(IdReverse)
{
    EDMD::Id id(-10, EDMD::Id::Reverse());
    BOOST_CHECK_EQUAL(id(), -10);
    BOOST_CHECK_EQUAL(id(), -11);
}
