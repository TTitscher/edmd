#pragma once
#include "Sphere.h"
#include <set>
#include <stdexcept>
#include <vector>

namespace EDMD
{
void CheckUniqueIds(const std::vector<Sphere>& spheres)
{
    std::set<int> sphereIds;
    for (const auto& sphere : spheres)
    {
        auto it = sphereIds.insert(sphere.Id());
        if (not it.second)
            throw std::runtime_error("You have to provide unique ids for all spheres. Id " +
                                     std::to_string(sphere.Id()) + " duplicate.");
    }
}

} /* EDMD */
