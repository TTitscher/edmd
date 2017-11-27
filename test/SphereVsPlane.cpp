#include "BoostUnitTest.h"

#include "SphereVsPlane.h"

using namespace EDMD;

BOOST_AUTO_TEST_CASE(NoGrow)
{
    Sphere s;
    s.position = Eigen::Vector3d(0, 0, 0);
    s.velocity = Eigen::Vector3d(1, 0, 0);
    s.radius = 2;
    s.growthRate = 0;

    Plane p(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(-1, 0, 0));
    BOOST_TEST_MESSAGE("" << p.position);

    const double expected = 8;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), expected, 1.e-10);

    MoveAndGrow(s, expected);

    // velocities after collision should swap
    Eigen::Vector3d v = VelocityAfterCollision(s, p);
    BoostUnitTest::CheckEigenMatrix(s.velocity, -v);
}

BOOST_AUTO_TEST_CASE(Grow)
{
    Sphere s;
    s.position = Eigen::Vector3d(0, 0, 0);
    s.velocity = Eigen::Vector3d(0, 0, 0);
    s.radius = 2;
    s.growthRate = 1;

    Plane p(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(-1, 0, 0));

    const double expected = 8;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), expected, 1.e-10);

    // perform collision
    MoveAndGrow(s, expected);
    s.velocity = VelocityAfterCollision(s, p);

    // no more collisions expected
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s, p), -1., 1.e-10);
}
