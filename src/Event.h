#pragma once
#include <algorithm>
#include <cassert>

namespace EDMD
{

class Event
{
public:
    //! @brief Event class that stores the collision partners as ids
    //! @param first
    Event(double time, int first, int second)
        : mTime(time)
        , mFirst(std::min(first, second))
        , mSecond(std::max(first, second))
    {
        assert(first >= 0);
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


} // namespace EDMD
