#pragma once
#include "Sphere.h"
#include <stdexcept>

namespace EDMD
{

double VelocityAfterCollision1D(double v1, double v2, double mass1, double mass2)
{
    return (v1 * (mass1 - mass2) + 2 * v2 * mass2) / (mass1 + mass2);
}

std::pair<Eigen::Vector3d, Eigen::Vector3d> VelocityAfterCollision(Sphere s1, Sphere s2)
{
    // 1) Get normal direction
    const Eigen::Vector3d n = (s1.position - s2.position).normalized();

    // 2) Velocity split:
    // 2.1) normal velocity
    const double velocityNormal1 = n.dot(s1.velocity);
    const double velocityNormal2 = n.dot(s2.velocity);

    // 2.2) tangential velocity
    const Eigen::Vector3d velocityTransversal1 = s1.velocity - n * velocityNormal1;
    const Eigen::Vector3d velocityTransversal2 = s2.velocity - n * velocityNormal2;

    // 3.1) 1D collision
    const double mass1 = s1.radius * s1.radius * s1.radius;
    const double mass2 = s2.radius * s2.radius * s2.radius;
    double velocityNormalNew1 = VelocityAfterCollision1D(velocityNormal1, velocityNormal2, mass1, mass2);
    double velocityNormalNew2 = VelocityAfterCollision1D(velocityNormal2, velocityNormal1, mass2, mass1);

    // 3.2) make sure, that the new normal velocities are at least
    // fast as the growth rates --> no simultaneous re-collision

    const double velocityNormalEffective = velocityNormalNew1 - velocityNormalNew2;

    if (velocityNormalEffective < 0.)
    {
        // add the missing effective velocity to the spheres
        const double velocityDifference1 = -velocityNormalEffective * mass2 / (mass1 + mass2);
        const double velocityDifference2 = -velocityNormalEffective - velocityDifference1;

        velocityNormalNew1 += velocityDifference1;
        velocityNormalNew2 -= velocityDifference2;
    }

    // 3.3) add normal velocity boost
    const double velocityExtra = 2.0;
    velocityNormalNew1 += velocityExtra * s1.growthRate;
    velocityNormalNew2 -= velocityExtra * s2.growthRate;

    // 4) combine normal and tangential velocities, add growth rate for collision buffer
    return {velocityNormalNew1 * n + velocityTransversal1, velocityNormalNew2 * n + velocityTransversal2};
}

double PredictedCollisionTime(const Sphere& sphere1, const Sphere& sphere2)
{
    // sync both spheres to the more recent time
    const Sphere& s1 = sphere1.timeOfLastUpdate > sphere2.timeOfLastUpdate ? sphere1 : sphere2;
    const Sphere& s2 = sphere1.timeOfLastUpdate > sphere2.timeOfLastUpdate ? sphere2 : sphere1;
    const double baseTime = s1.timeOfLastUpdate;

    const Eigen::Vector3d dP = s1.position - (s2.position + s2.velocity * (s1.timeOfLastUpdate - s2.timeOfLastUpdate));
    const Eigen::Vector3d dV = s1.velocity - s2.velocity;

    const double dR = s1.radius + s2.radius + s2.growthRate * (s1.timeOfLastUpdate - s2.timeOfLastUpdate);
    const double dG = s1.growthRate + s2.growthRate;

    const double a = dV.dot(dV) - dG * dG;
    const double b = 2 * (dV.dot(dP) - dR * dG);
    const double c = dP.dot(dP) - dR * dR;

    if (c < -2.e-10 * s1.radius)
    {
        throw std::runtime_error(
                                 "Sphere " + std::to_string(s1.id) + " overlaps with Sphere " + std::to_string(s2.id));
    }

    double discriminant = b * b - 4 * a * c;

    // allow small negative values ...
    if (discriminant < -1e-12)
        return -1;

    // ... an treat them as zero
    discriminant = std::max(discriminant, 0.);

    if (b < 0.)
        return baseTime + 2 * c / (-b + std::sqrt(discriminant));

    if (a < 0.)
        return baseTime + (-b - std::sqrt(discriminant)) / (2 * a);

    return -1.;
}
} /* EDMD */
