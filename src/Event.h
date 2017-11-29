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
        out << "t=" << std::setw(20) << e.Time() << ": " << eventInfo.EventType() << " between " << e.First() << " and "
            << e.Second();
        return out;
    }

    std::string EventType() const
    {
        if (mE.Second() < mN)
            return "Sphere collision";
        else
            return " Wall  collision";
    }

private:
    Event mE;
    int mN;
};

} /* EDMD */
