#pragma once
#include <cassert>
#include <iomanip>
#include <ostream>

namespace EDMD
{

class Event
{
public:
    //! @brief Event class that stores the collision partners as ids
    //! @param first
    Event(double time, int first, int second)
        : mTime(time)
        , mFirst(first)
        , mSecond(second)
    {
        assert(first >= 0);
        assert(first < second);
    }

    Event(const Event&) = default;
    Event(Event&&) = default;

    Event& operator=(const Event&) = default;
    Event& operator=(Event&&) = default;


    bool operator<(const Event& other) const
    {
        // return Time() < other.Time();
        if (Time() != other.Time())
            return Time() < other.Time();
        if (First() != other.First())
            return First() < other.First();
        return Second() < other.Second();
    }

    double Time() const
    {
        return mTime;
    }
    int First() const
    {
        return mFirst;
    }
    int Second() const
    {
        return mSecond;
    }

private:
    double mTime;
    int mFirst;
    int mSecond;
};

enum class EventType
{
    SPHERE,
    WALL
};


class EventInfo
{
public:
    EventInfo(Event e, int n)
        : mE(e)
        , mN(n)
    {
    }

    friend std::ostream& operator<<(std::ostream& out, EventInfo eventInfo)
    {
        const auto& e = eventInfo.mE;
        out.precision(15);
        out << "t=" << std::setw(20) << e.Time() << ": " << to_string(eventInfo.Type()) << " between " << e.First()
            << " and " << e.Second();
        return out;
    }

    EventType Type() const
    {
        if (mE.Second() < mN)
            return EventType::SPHERE;
        else
            return EventType::WALL;
    }

    static std::string to_string(EventType t)
    {
        if (t == EventType::SPHERE)
            return "Sphere collision";
        if (t == EventType::WALL)
            return " Wall  collision";
        throw;
    }

private:
    Event mE;
    int mN;
};

} /* EDMD */
