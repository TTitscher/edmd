#include "BoostUnitTest.h"

#include "Box.h"
#include "Event.h"
#include "Sphere.h"
#include "SphereVsPlane.h"
#include "SphereVsSphere.h"
#include <set>

#include <iostream>

using namespace EDMD;
constexpr double EPS = 1.e-4;

Event FindNextEvent(Sphere s, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes)
{
    const size_t numSpheres = spheres.size();
    Event e(Inf(), 0, 1);

    for (const auto& sphere : spheres)
    {
        double t = PredictedCollisionTime(s, sphere);
        if (t != Inf() && t < e.Time())
            e = Event(t, std::min(s.Id(), sphere.Id()), std::max(s.Id(), sphere.Id()));
    }

    for (size_t i = 0; i < planes.size(); ++i)
    {
        double t = PredictedCollisionTime(s, planes[i]);
        if (t < e.Time())
            e = Event(t, s.Id(), i + numSpheres);
    }
    return e;
}

void Run(size_t numSpheres)
{
    auto box = Box(-Eigen::Vector3d::Ones(), Eigen::Vector3d::Ones());

    std::vector<Sphere> spheres;
    for (int i = 0; i < numSpheres; ++i)
    {
        Sphere s(Eigen::Vector3d::Random(), Eigen::Vector3d::Random(), 0, 1, i);
        spheres.push_back(s);
    }

    std::set<Event> globalEvents;
    std::vector<int> nextCollisionPartner(spheres.size(), -1);
    for (int i = 0; i < numSpheres; ++i)
    {
        auto e = FindNextEvent(spheres[i], spheres, box);
        globalEvents.insert(e);
        nextCollisionPartner[e.First()] = e.Second();
        if (e.Second() < numSpheres)
            nextCollisionPartner[e.Second()] = e.First();
    }

    for (auto& e : globalEvents)
        std::cout << EventInfo(e, numSpheres) << std::endl;
}


BOOST_AUTO_TEST_CASE(SpheresInBox)
{
    Run(10);
}
