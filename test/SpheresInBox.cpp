#include "BoostUnitTest.h"

#include "Box.h"
#include "Event.h"
#include "Sphere.h"

using namespace EDMD;
constexpr double EPS = 1.e-4;

void Run(size_t numSpheres)
{
    auto box = Box(-Eigen::Vector3d::Ones(), Eigen::Vector3d::Ones());

    std::vector<Sphere> spheres;
    for (int i = 0; i < numSpheres; ++i)
        Sphere s(Eigen::Vector3d::Random(), Eigen::Vector3d::Random(), 0, 1, i);
}


BOOST_AUTO_TEST_CASE(SpheresInBox)
{
    Run(10);
}
