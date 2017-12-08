#pragma once

#include <iostream>
#include <set>
#include <vector>

#include "Checks.h"
#include "Event.h"
#include "SphereVsPlane.h"
#include "SphereVsSphere.h"


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
        CheckUniqueIds(spheres);
        Initialize();
    }

    double DoStep()
    {
        Event e = PopNext();
        EventInfo eInfo(e, N);
        if (mDebug)
            std::cout << "Processing " << eInfo << "\n";

        switch (eInfo.Type())
        {
        case EventType::SPHERE:
            SphereCollision(e);
            break;
        case EventType::WALL:
            WallCollision(e);
            break;
        }

        return e.Time();
    }

    void SetDebug(bool debug)
    {
        mDebug = debug;
    }

private:
    void SphereCollision(Event e)
    {
        auto& s1 = mSpheres[e.First()];
        auto& s2 = mSpheres[e.Second()];
        s1.MoveAndGrow(e.Time());
        s2.MoveAndGrow(e.Time());
        Eigen::Vector3d v1, v2;
        std::tie(v1, v2) = VelocityAfterCollision(s1, s2);
        s1.Velocity(v1);
        s2.Velocity(v2);

        RemoveOldEvents(e.First());
        RemoveOldEvents(e.Second());
        AddAllEvents(mSpheres[e.First()], e.Time());
        AddAllEvents(mSpheres[e.Second()], e.Time());
    }

    void WallCollision(Event e)
    {
        auto& s = mSpheres[e.First()];
        auto& p = mWalls[e.Second() - N];
        s.MoveAndGrow(e.Time());
        s.Velocity(VelocityAfterCollision(s, p));

        RemoveOldEvents(e.First());
        AddAllEvents(mSpheres[e.First()], e.Time());
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

    Event PopNext()
    {
        auto it = mGlobalEvents.begin();
        if (it == mGlobalEvents.end())
            throw std::runtime_error("Empty global event list!");
        Event next = *it;
        mGlobalEvents.erase(it);
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
            if (t != Inf() && t >= time)
                events.push_back(Event(t, std::min(s.Id(), sphere.Id()), std::max(s.Id(), sphere.Id())));
        }

        for (size_t i = 0; i < mWalls.size(); ++i)
        {
            double t = PredictedCollisionTime(s, mWalls[i]);
            if (t != Inf() && t >= time)
                events.push_back(Event(t, s.Id(), i + numSpheres));
        }
        return events;
    }

    std::vector<Sphere>& mSpheres;
    std::vector<Plane> mWalls;

    std::set<Event> mGlobalEvents;
    std::vector<Event*> mLocalEvents;

    int N;

    bool mDebug = false;
};
} /* EDMD */
