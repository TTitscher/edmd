#include "BoostUnitTest.h"

#include "SphereVsSphere.h"

using namespace EDMD;

BOOST_AUTO_TEST_CASE(NoGrow)
{
    Sphere s1;
    s1.position = Eigen::Vector3d(0, 0, 0);
    s1.velocity = Eigen::Vector3d(0, 0, 0);
    s1.radius = 2;
    s1.growthRate = 0;

    Sphere s2;
    s2.position = Eigen::Vector3d(0, 10, 0);
    s2.velocity = Eigen::Vector3d(0, -2, 0);
    s2.radius = 2;
    s2.growthRate = 0;

    const double expected = 3;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), expected, 1.e-10);

    MoveAndGrow(s1, expected);
    MoveAndGrow(s2, expected);

    // velocities after collision should swap
    Eigen::Vector3d v1, v2;
    std::tie(v1, v2) = VelocityAfterCollision(s1, s2);
    BoostUnitTest::CheckEigenMatrix(s1.velocity, v2);
    BoostUnitTest::CheckEigenMatrix(s2.velocity, v1);
}

BOOST_AUTO_TEST_CASE(Grow)
{
    Sphere s1;
    s1.position = Eigen::Vector3d(0, 0, 0);
    s1.velocity = Eigen::Vector3d(0, 0, 0);
    s1.radius = 2;
    s1.growthRate = 0;

    Sphere s2;
    s2.position = Eigen::Vector3d(0, 10, 0);
    s2.velocity = Eigen::Vector3d(0, 0, 0);
    s2.radius = 2;
    s2.growthRate = 1;

    const double expected = 6;
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), expected, 1.e-10);

    // perform collision
    MoveAndGrow(s1, expected);
    MoveAndGrow(s2, expected);
    std::tie(s1.velocity, s2.velocity) = VelocityAfterCollision(s1, s2);

    // no more collisions expected
    BOOST_CHECK_CLOSE(PredictedCollisionTime(s1, s2), -1., 1.e-10);
}
