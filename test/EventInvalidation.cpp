#include "BoostUnitTest.h"

#include "Simulation.h"
#include "Utils.h"

using namespace EDMD;

/*
 *
 * ^ y
 * |
 * |
 *  ---> x
 *
 * ------------- (wall)
 *
 *
 *     (0)      <---(1)
 *
 *     ^
 *     |
 *    (2)
 *
 *
 *  The next event of sphere (2) is the collision with (0) at t~2s. This
 *  collision is invalidated by the collision (0)-(1) at t~1. The simulation
 *  must now correctly predict the collision (2)-Wall at t=3.
 *
 *  (0) and (2) are not perfectly aligned to avoid a event (1)-(2) after the
 *  elastic collision (0)-(1).
 *
 */

Plane Wall()
{
    return Plane(V(0, -1, 0), V(0, 1, 0));
}

std::vector<Sphere> Spheres()
{
    Sphere s0(V(0.5, 1, 0), V(0, 0, 0), 1, 0, 0);
    Sphere s1(V(8, 1, 0), V(-6, 0, 0), 1, 0, 1);
    Sphere s2(V(0, 9, 0), V(0, -3, 0), 1, 0, 2);

    return {s0, s1, s2};
}

BOOST_AUTO_TEST_CASE(EventInvalidation)
{
    auto s = Spheres();
    auto wall = Wall();
    Simulation simulation(s, {wall});
    simulation.SetDebug(true);

    BOOST_CHECK_CLOSE(simulation.DoStep(), 1, 10); // 1 += 10%
    BOOST_CHECK_CLOSE(simulation.DoStep(), 3, 1.e-10); // exactly at t=3

    BOOST_CHECK_THROW(simulation.DoStep(), std::runtime_error);
}
