#pragma once
#include <Eigen/Core>

namespace EDMD
{
struct Plane
{
    explicit Plane(Eigen::Vector3d position, Eigen::Vector3d direction)
        : position(position)
        , direction(direction.normalized())
    {
    }

    const Eigen::Vector3d position;
    const Eigen::Vector3d direction;
};
} /* EDMD */
