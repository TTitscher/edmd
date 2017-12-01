#include "BoostUnitTest.h"

#include "Box.h"
#include "Simulation.h"

#include <iostream>

using namespace EDMD;

BOOST_AUTO_TEST_CASE(EightSpheresInBox)
{
    auto walls = Box(-Eigen::Vector3d::Ones(), Eigen::Vector3d::Ones());
    std::vector<Sphere> spheres;
    for (int i = 0; i < 8; ++i)
    {
        Sphere s(Eigen::Vector3d::Random(), Eigen::Vector3d::Random(), 0, 1, i);
        spheres.push_back(s);
    }

    Simulation simulation(spheres, walls);
    double currentTime = 0;
    for (int i = 0; i < 1e3; ++i)
        currentTime = simulation.DoStep();


    spheres.front().MoveAndGrow(currentTime);
    BOOST_CHECK_GT(spheres.front().Radius(), 0.49);
}
