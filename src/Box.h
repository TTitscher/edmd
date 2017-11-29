#pragma once

#include "Plane.h"
#include <cassert>
#include <vector>

namespace EDMD
{

//! @brief creates 6 planes from start to end that form a axis aligned box
//! @param start start coordinate
//! @param end end coordinate
std::vector<Plane> Box(Eigen::Vector3d start, Eigen::Vector3d end)
{
    assert(start.x() < end.x());
    assert(start.y() < end.y());
    assert(start.z() < end.z());

    std::vector<Plane> planes;
    planes.push_back(Plane(start, Eigen::Vector3d(1, 0, 0)));
    planes.push_back(Plane(start, Eigen::Vector3d(0, 1, 0)));
    planes.push_back(Plane(start, Eigen::Vector3d(0, 0, 1)));

    planes.push_back(Plane(end, Eigen::Vector3d(-1, 0, 0)));
    planes.push_back(Plane(end, Eigen::Vector3d(0, -1, 0)));
    planes.push_back(Plane(end, Eigen::Vector3d(0, 0, -1)));
    return planes;
}

std::vector<Plane> UnitBox()
{
    return Box(Eigen::Vector3d::Zero(), Eigen::Vector3d::Ones());
}

} /* EDMD */
