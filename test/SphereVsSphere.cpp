#include "BoostUnitTest.h"

#include "SphereVsSphere.h"

using namespace EDMD;

BOOST_AUTO_TEST_CASE(NoGrow)
{
    Sphere s1(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), 2, 0, 0);
    Sphere s2(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(-2, 0, 0), 2, 0, 1);

    const double expected = 3;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), expected, 1.e-10);

    s1.MoveAndGrow(expected);
    s2.MoveAndGrow(expected);

    // velocities after collision should swap
    Eigen::Vector3d v1, v2;
    std::tie(v1, v2) = VelocityAfterCollision(s1, s2);
    BoostUnitTest::CheckEigenMatrix(s1.Velocity(), v2);
    BoostUnitTest::CheckEigenMatrix(s2.Velocity(), v1);
}

BOOST_AUTO_TEST_CASE(Grow)
{
    Sphere s1(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), 2, 0, 0);
    Sphere s2(Eigen::Vector3d(10, 0, 0), Eigen::Vector3d(0, 0, 0), 2, 1, 1);

    const double expected = 6;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), expected, 1.e-10);

    // perform collision
    s1.MoveAndGrow(expected);
    s2.MoveAndGrow(expected);
    Eigen::Vector3d v1, v2;
    std::tie(v1, v2) = VelocityAfterCollision(s1, s2);
    s1.Velocity(v1);
    s2.Velocity(v2);

    // no more collisions expected
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), Inf(), 1.e-10);
}
