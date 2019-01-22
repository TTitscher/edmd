#pragma once

#include <iostream>
#include <set>
#include <vector>

#include "Checks.h"
#include "Event.h"
#include "EventInfo.h"
#include "SphereVsPlane.h"
#include "SphereVsSphere.h"

namespace EDMD
{


class GlobalEvents
{
public:
    GlobalEvents(size_t N)
        : N(N)
    {
        mLocalEvents.resize(N);
    }

    //! @brief Returns the most recent event and removes it from the global event list
    Event PopNext()
    {
        auto it = mGlobalEvents.begin();
        if (it == mGlobalEvents.end())
            throw std::runtime_error("Empty global event list!");
        Event next = *it;
        mGlobalEvents.erase(it);
        return next;
    }

    //! @brief Add to global and local event lists
    void AddEvent(Event e)
    {
        mGlobalEvents.insert(e);
        assert(IsSphere(e.First()));
        mLocalEvents[e.First()].insert(e);
        if (IsSphere(e.Second()))
            mLocalEvents[e.Second()].insert(e);
    }

    //! @brief Remove `e` and all corresponding events from the local and global event lists
    //! @remark A corresponding event is an event that includes the same spheres as `e`
    void RemoveCorrespondingEvents(Event e)
    {
        // should be removed from mGlobalEvents via PopNext()
        assert(mGlobalEvents.find(e) == mGlobalEvents.end());
        RemoveCorrespondingEvents(e.First());
        RemoveCorrespondingEvents(e.Second());
    }


private:
    void RemoveCorrespondingEvents(int id)
    {
        if (not IsSphere(id))
            return;
        std::set<Event>& correspondingEvents = mLocalEvents[id];
        for (Event e : correspondingEvents)
        {
            mGlobalEvents.erase(e);
            // if e.First() == id then mLocalEvents[e.First()] == correspondingEvents
            // So deleting from it would invalidate the surrounding loop
            // over correspondingEvents.
            if (e.First() != id)
                mLocalEvents[e.First()].erase(e);
            if (e.Second() != id and IsSphere(e.Second()))
                mLocalEvents[e.Second()].erase(e);
        }
        correspondingEvents.clear();
    }

    bool IsSphere(int id) const
    {
        return id < N;
    }

    size_t N;
    std::vector<std::set<Event>> mLocalEvents;
    std::set<Event> mGlobalEvents;
};

class Simulation
{
public:
    Simulation(std::vector<Sphere>& spheres, std::vector<Plane> planes)
        : mSpheres(spheres)
        , mWalls(planes)
        , mEvents(spheres.size())
        , N(spheres.size())
    {
        CheckUniqueIds(spheres);
        InitialEventLists();
    }

    double DoStep()
    {
        Event e = mEvents.PopNext();
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

        mEvents.RemoveCorrespondingEvents(e);
        AddAllEvents(mSpheres[e.First()], e.Time());
        AddAllEvents(mSpheres[e.Second()], e.Time());
    }

    void WallCollision(Event e)
    {
        auto& s = mSpheres[e.First()];
        auto& p = mWalls[e.Second() - N];
        s.MoveAndGrow(e.Time());
        s.Velocity(VelocityAfterCollision(s, p));

        mEvents.RemoveCorrespondingEvents(e);
        AddAllEvents(mSpheres[e.First()], e.Time());
    }

    //! @brief builds the initial global event list
    void InitialEventLists(double t = 0)
    {
        for (const auto& s : mSpheres)
            AddAllEvents(s, t);
    }

    void AddAllEvents(const Sphere& s, double t)
    {
        for (const auto& sphere : mSpheres)
        {
            double t = PredictedCollisionTime(s, sphere);
            if (t != Inf())
                mEvents.AddEvent(Event(t, s.Id(), sphere.Id()));
        }

        for (size_t i = 0; i < mWalls.size(); ++i)
        {
            double t = PredictedCollisionTime(s, mWalls[i]);
            if (t != Inf())
                mEvents.AddEvent(Event(t, s.Id(), i + N));
        }
    }

    std::vector<Sphere>& mSpheres;
    std::vector<Plane> mWalls;

    GlobalEvents mEvents;
    int N;

    bool mDebug = false;
};
} // namespace EDMD
