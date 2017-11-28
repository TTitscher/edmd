#pragma once
#include <Eigen/Core>

namespace EDMD
{
class Plane
{
public:
    Plane(Eigen::Vector3d position, Eigen::Vector3d direction)
        : mPosition(position)
        , mDirection(direction.normalized())
    {
    }

    Eigen::Vector3d Position() const
    {
        return mPosition;
    }

    Eigen::Vector3d Direction() const
    {
        return mDirection;
    }

private:
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mDirection;
};
} /* EDMD */
