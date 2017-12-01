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
 *    (0)      <---(1)
 *
 *     ^
 *     |
 *    (2)
 *
 *  The next event of sphere (2) is the collision with (0) at t=2s. This
 *  collision is invalidated by the collision (0)-(1) at t=1. The simulation
 *  must now correctly predict the collision (2)-Wall at t=3.
 *
 *  Due to the fully elastic collision (0)-(1), there will be no event
 *  of (1) and (2).
 */

Plane Wall()
{
    return Plane(V(0, -1, 0), V(0, 1, 0));
}

std::vector<Sphere> Spheres()
{
    Sphere s0(V(0, 1, 0), V(0, 0, 0), 1, 0, 0);
    Sphere s1(V(8, 1, 0), V(-6, 0, 0), 1, 0, 1);
    Sphere s2(V(0, 9, 0), V(0, -3, 0), 1, 0, 2);

    return {s0, s1, s2};
}

BOOST_AUTO_TEST_CASE(CheckAssumptions)
{
    auto s = Spheres();
    auto wall = Wall();
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[1], s[0]), 1., 1.e-10);
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[2], s[0]), 2., 1.e-10);
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[2], wall), 3., 1.e-10);

    // no wall collisions of 0 and 1
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[0], wall), Inf(), 1.e-10);
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[1], wall), Inf(), 1.e-10);

    // perform first collision
    s[0].MoveAndGrow(1.);
    s[1].MoveAndGrow(1.);
    Eigen::Vector3d v0, v1;
    std::tie(v0, v1) = VelocityAfterCollision(s[0], s[1]);
    s[0].Velocity(v0);
    s[1].Velocity(v1);

    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[2], s[0]), Inf(), 1.e-10);
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[2], s[1]), Inf(), 1.e-10);

    BOOST_CHECK_CLOSE(PredictedCollisionTime(s[2], wall), 3., 1.e-10);
}

BOOST_AUTO_TEST_CASE(EventInvalidation)
{
    auto s = Spheres();
    auto wall = Wall();
    Simulation simulation(s, {wall});
    simulation.SetDebug(true);

    simulation.DoStep();
    simulation.DoStep();

    BOOST_CHECK_THROW(simulation.DoStep(), std::runtime_error);
}
