#include "BoostUnitTest.h"

#include "Box.h"
#include "Event.h"
#include "Sphere.h"

using namespace EDMD;
constexpr double EPS = 1.e-4;

void Run(size_t numSpheres)
{
    auto box = Box(-Eigen::Vector3d::Ones(), Eigen::Vector3d::Ones());

    std::vector<Sphere> spheres(numSpheres);
    for (Sphere& sphere : spheres)
    {
        sphere.position = Eigen::Vector3d::Random();
        sphere.velocity = Eigen::Vector3d::Random();
        sphere.radius = 0;
        sphere.growthRate = 1;
    }
}


BOOST_AUTO_TEST_CASE(SpheresInBox)
{
    Run(10);
}
