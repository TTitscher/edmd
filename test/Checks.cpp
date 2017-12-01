#include "BoostUnitTest.h"

#include "Checks.h"
#include "Utils.h"

using namespace EDMD;

BOOST_AUTO_TEST_CASE(NonuniqueIds)
{
    Sphere s0(V(0, 0, 0), V(0, 0, 0), 0, 0, 42);
    Sphere s1(V(2, 0, 0), V(0, 0, 0), 0, 0, 42);
    std::vector<Sphere> spheres({s0, s1});
    BOOST_CHECK_THROW(CheckUniqueIds(spheres), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(UniqueIds)
{
    Sphere s0(V(0, 0, 0), V(0, 0, 0), 0, 0, 12);
    Sphere s1(V(2, 0, 0), V(0, 0, 0), 0, 0, 42);
    std::vector<Sphere> spheres({s0, s1});
    BOOST_CHECK_NO_THROW(CheckUniqueIds(spheres));
}
