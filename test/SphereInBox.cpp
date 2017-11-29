#include "BoostUnitTest.h"
#include "Box.h"
#include "Constants.h"
#include "SphereVsPlane.h"

using namespace EDMD;

constexpr double EPS = 1.e-4;

//! @brief Single `sphere` in a unit box growing for `numEvents` events
//! @return sphere after numEvents collisions
Sphere SimulateSphereInUnitBox(Sphere sphere, int numEvents)
{
    auto box = UnitBox();
    for (int i = 0; i < numEvents; ++i)
    {
        double closestEventTime = Inf();
        Plane closestEventPlane = box.front();

        for (auto plane : box)
        {
            double t = PredictedCollisionTime(sphere, plane);
            if (t < closestEventTime)
            {
                closestEventTime = t;
                closestEventPlane = plane;
            }
        }

        sphere.MoveAndGrow(closestEventTime);
        sphere.Velocity(VelocityAfterCollision(sphere, closestEventPlane));
    }
    return sphere;
}

BOOST_AUTO_TEST_CASE(SphereInBoxGrow)
{
    Sphere s(Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d::Zero(), 0, 1, 0);
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1).Radius(), 0.5 - EPS);
}

BOOST_AUTO_TEST_CASE(SphereInBoxSimultaneousEvents)
{
    Sphere s(Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d::Ones(), 0, 1, 0);
    // simultaneous collision with 3 walls
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1000).Radius(), 0.5 - EPS);
}

BOOST_AUTO_TEST_CASE(SphereInBoxRandomVelocity)
{
    Sphere s(Eigen::Vector3d(0.5, 0.5, 0.5), Eigen::Vector3d::Random(), 0, 1, 0);
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1000).Radius(), 0.5 - EPS);
}
