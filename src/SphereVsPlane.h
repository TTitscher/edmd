#pragma once
#include "Plane.h"
#include "Sphere.h"

namespace EDMD
{
Eigen::Vector3d VelocityAfterCollision(Sphere s, Plane p)
{
    const double normalVelocity = s.velocity.dot(p.direction);

    // include growthrate to avoid new collision.
    if (normalVelocity < 0.)
        // sphere moves towards wall:
        return s.velocity + (s.growthRate - 2 * normalVelocity) * p.direction;

    return s.velocity + s.growthRate * p.direction;
}

double PredictedCollisionTime(Sphere s, Plane p)
{
    const double staticDistanceToWall = p.direction.dot(s.position - p.position) - s.radius;
    const double dynamicDistanceToWall = s.velocity.dot(p.direction) - s.growthRate;

    if (staticDistanceToWall < 1e-15 && dynamicDistanceToWall >= 0)
        // sphere touches wall, not penetrating it in the future.
        return -1.;

    const double timeCollision = -staticDistanceToWall / dynamicDistanceToWall;
    if (timeCollision >= 0)
        return s.timeOfLastUpdate + timeCollision;

    return -1;
}
} /* EDMD */
