#include "BoostUnitTest.h"

#include "Box.h"
#include "Simulation.h"
#include "Utils.h"

#include <iostream>

using namespace EDMD;
constexpr double EPS = 1.e-4;


BOOST_AUTO_TEST_CASE(SpheresInBox)
{
    const double r = 1;

    // four touching spheres
    Sphere s0(V(0, 0, 0), V(0, 0, 0), r, 0, 0);
    Sphere s1(V(2 * r, 0, 0), V(0, 0, 0), r, 0, 1);
    Sphere s2(V(4 * r, 0, 0), V(0, 0, 0), r, 0, 2);
    Sphere s3(V(6 * r, 0, 0), V(0, 0, 0), r, 0, 3);

    // one approaching sphere
    Sphere s4(V(10 * r, 0, 0), V(-1, 0, 0), r, 0, 4);

    std::vector<Sphere> spheres({s0, s1, s2, s3, s4});

    Simulation simulation(spheres, {/* no walls */});
    simulation.SetDebug(true);

    std::cout << "blub" << std::endl;

    const int numExpectedCollisions = 4; // 4 vs 3, 3 vs 2, 2 vs 1, 1 vs0
    const int expectedCollisionTime = 2.;

    for (int i = 0; i < numExpectedCollisions; ++i)
    {
        double collisionTime = simulation.DoStep();
        BOOST_CHECK_CLOSE(collisionTime, expectedCollisionTime, 1.e-10);
    }

    // no more collisions after that.
    BOOST_CHECK_THROW(simulation.DoStep(), std::runtime_error);
}
