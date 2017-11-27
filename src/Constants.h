#pragma once
#include <limits>

namespace EDMD
{
inline constexpr double Inf()
{
    return std::numeric_limits<double>::max();
}
} /* EDMD */
