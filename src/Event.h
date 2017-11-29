#pragma once
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
        , mFirst(first)
        , mSecond(second)
    {
        assert(first >= 0);
        assert(first < second);
    }

    bool operator<(const Event& other) const
    {
        return Time() < other.Time();
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
} /* EDMD */
