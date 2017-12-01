#pragma once
#include <Eigen/Core>

namespace EDMD
{
Eigen::Vector3d V(double x, double y, double z)
{
    return Eigen::Vector3d(x, y, z);
}
} /* EDMD */
