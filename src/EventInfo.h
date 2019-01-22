#pragma once

#include "Event.h"
#include <iomanip>
#include <ostream>

namespace EDMD
{

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
} // namespace EDMD
