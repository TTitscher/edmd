#include "BoostUnitTest.h"
#include "Constants.h"
#include "SphereVsPlane.h"

using namespace EDMD;

constexpr double EPS = 1.e-4;

std::vector<Plane> UnitBox()
{
    std::vector<Plane> planes;
    planes.push_back(Plane(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(1, 0, 0)));
    planes.push_back(Plane(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 1, 0)));
    planes.push_back(Plane(Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 1)));

    planes.push_back(Plane(Eigen::Vector3d(1, 1, 1), Eigen::Vector3d(-1, 0, 0)));
    planes.push_back(Plane(Eigen::Vector3d(1, 1, 1), Eigen::Vector3d(0, -1, 0)));
    planes.push_back(Plane(Eigen::Vector3d(1, 1, 1), Eigen::Vector3d(0, 0, -1)));
    return planes;
}

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

        MoveAndGrow(sphere, closestEventTime);
        sphere.velocity = VelocityAfterCollision(sphere, closestEventPlane);
    }
    return sphere;
}

BOOST_AUTO_TEST_CASE(SphereInBoxGrow)
{
    Sphere s;
    s.position = Eigen::Vector3d(0.5, 0.5, 0.5);
    s.velocity = Eigen::Vector3d::Zero();
    s.radius = 0;
    s.growthRate = 1;
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1).radius, 0.5 - EPS);
}

BOOST_AUTO_TEST_CASE(SphereInBoxSimultaneousEvents)
{
    Sphere s;
    s.position = Eigen::Vector3d(0.5, 0.5, 0.5);
    s.velocity = Eigen::Vector3d::Ones(); // simultaneous collision with 3 walls
    s.radius = 0;
    s.growthRate = 1;
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1000).radius, 0.5 - EPS);
}

BOOST_AUTO_TEST_CASE(SphereInBoxRandomVelocity)
{
    Sphere s;
    s.position = Eigen::Vector3d(0.5, 0.5, 0.5);
    s.velocity = Eigen::Vector3d::Random();
    s.radius = 0;
    s.growthRate = 1;
    BOOST_CHECK_GT(SimulateSphereInUnitBox(s, 1000).radius, 0.5 - EPS);
}
