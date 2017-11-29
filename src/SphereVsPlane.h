#pragma once
#include "Constants.h"
#include "Plane.h"
#include "Sphere.h"

namespace EDMD
{
Eigen::Vector3d VelocityAfterCollision(Sphere s, Plane p)
{
    const double normalVelocity = s.Velocity().dot(p.Direction());

    // include growthrate to avoid new collision.
    if (normalVelocity < 0.)
        // sphere moves towards wall:
        return s.Velocity() + (s.GrowthRate() - 2 * normalVelocity) * p.Direction();

    return s.Velocity() + s.GrowthRate() * p.Direction();
}

double PredictedCollisionTime(Sphere s, Plane p)
{
    const double staticDistanceToWall = p.Direction().dot(s.Position() - p.Position()) - s.Radius();
    const double dynamicDistanceToWall = s.Velocity().dot(p.Direction()) - s.GrowthRate();

    if (staticDistanceToWall < 1e-15 && dynamicDistanceToWall >= 0)
        // sphere touches wall, not penetrating it in the future.
        return Inf();

    const double timeCollision = -staticDistanceToWall / dynamicDistanceToWall;
    if (timeCollision >= 0)
        return s.TimeOfLastUpdate() + timeCollision;

    return Inf();
}
} /* EDMD */
