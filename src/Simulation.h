#pragma once

#include "Event.h"
#include "SphereVsPlane.h"
#include "SphereVsSphere.h"
#include <set>
#include <vector>

namespace EDMD
{
class Simulation
{
public:
    Simulation(std::vector<Sphere>& spheres, std::vector<Plane> planes)
        : mSpheres(spheres)
        , mWalls(planes)
        , N(spheres.size())
    {
        Initialize();
    }

    double DoStep()
    {
        Event e = PopNext();
        PerformCollision(e);
        RemoveOldEvents(e);

        AddAllEvents(mSpheres[e.First()], e.Time());
        if (e.Second() < N)
            AddAllEvents(mSpheres[e.Second()], e.Time());
        return e.Time();
    }

private:
    void RemoveOldEvents(Event e)
    {
        RemoveOldEvents(e.First());
        if (e.Second() < N)
            RemoveOldEvents(e.Second());
    }


    void RemoveOldEvents(int sphereId)
    {
        // std::remove_if does not work on std::set.
        for (auto it = mGlobalEvents.begin(); it != mGlobalEvents.end();)
        {
            if (it->First() == sphereId || it->Second() == sphereId)
                it = mGlobalEvents.erase(it);
            else
                ++it;
        }
    }

    void PerformCollision(Event e)
    {
        if (e.Second() < N)
        {
            auto& s1 = mSpheres[e.First()];
            auto& s2 = mSpheres[e.Second()];
            s1.MoveAndGrow(e.Time());
            s2.MoveAndGrow(e.Time());
            Eigen::Vector3d v1, v2;
            std::tie(v1, v2) = VelocityAfterCollision(s1, s2);
            s1.Velocity(v1);
            s2.Velocity(v2);
        }
        else
        {
            auto& s = mSpheres[e.First()];
            auto& p = mWalls[e.Second() - N];
            s.MoveAndGrow(e.Time());
            s.Velocity(VelocityAfterCollision(s, p));
        }
    }

    Event PopNext()
    {
        Event next = *mGlobalEvents.begin();
        mGlobalEvents.erase(mGlobalEvents.begin());
        return next;
    }

    //! @brief builds the initial global event list
    void Initialize()
    {
        for (const auto& s : mSpheres)
            AddAllEvents(s, 0.);
    }

    void AddAllEvents(const Sphere& s, double t)
    {
        auto events = FindAllEvents(s, t);
        for (auto e : events)
            mGlobalEvents.insert(e);
    }

    std::vector<Event> FindAllEvents(const Sphere& s, double time) const
    {
        const size_t numSpheres = mSpheres.size();
        std::vector<Event> events;

        for (const auto& sphere : mSpheres)
        {
            double t = PredictedCollisionTime(s, sphere);
            if (t != Inf() && t > time)
                events.push_back(Event(t, std::min(s.Id(), sphere.Id()), std::max(s.Id(), sphere.Id())));
        }

        for (size_t i = 0; i < mWalls.size(); ++i)
        {
            double t = PredictedCollisionTime(s, mWalls[i]);
            if (t != Inf() && t > time)
                events.push_back(Event(t, s.Id(), i + numSpheres));
        }
        return events;
    }

    std::vector<Sphere>& mSpheres;
    std::vector<Plane> mWalls;

    std::set<Event> mGlobalEvents;

    int N;
};
} /* EDMD */