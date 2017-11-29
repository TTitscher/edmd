#include "BoostUnitTest.h"

#include "SphereVsPlane.h"

using namespace EDMD;

BOOST_AUTO_TEST_CASE(NoGrow)
{
    Sphere s(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 0, 0), 2, 0, 0);
    Plane p(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(-1, 0, 0));

    const double expected = 8;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), expected, 1.e-10);

    s.MoveAndGrow(expected);

    // velocities after collision should swap
    Eigen::Vector3d v = VelocityAfterCollision(s, p);
    BoostUnitTest::CheckEigenMatrix(s.Velocity(), -v);
}

BOOST_AUTO_TEST_CASE(Grow)
{
    Sphere s(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), 2, 1, 0);
    Plane p(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(-1, 0, 0));

    const double expected = 8;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), expected, 1.e-10);

    // perform collision
    s.MoveAndGrow(expected);
    s.Velocity(VelocityAfterCollision(s, p));

    // no more collisions expected
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), Inf(), 1.e-10);
}
