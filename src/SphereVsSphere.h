#pragma once
#include "Constants.h"
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
    const Eigen::Vector3d n = (s1.Position() - s2.Position()).normalized();

    // 2) Velocity() split:
    // 2.1) normal Velocity()
    const double velocityNormal1 = n.dot(s1.Velocity());
    const double velocityNormal2 = n.dot(s2.Velocity());

    // 2.2) tangential Velocity()
    const Eigen::Vector3d velocityTransversal1 = s1.Velocity() - n * velocityNormal1;
    const Eigen::Vector3d velocityTransversal2 = s2.Velocity() - n * velocityNormal2;

    // 3.1) 1D collision
    const double mass1 = s1.Radius() * s1.Radius() * s1.Radius();
    const double mass2 = s2.Radius() * s2.Radius() * s2.Radius();
    double velocityNormalNew1 = VelocityAfterCollision1D(velocityNormal1, velocityNormal2, mass1, mass2);
    double velocityNormalNew2 = VelocityAfterCollision1D(velocityNormal2, velocityNormal1, mass2, mass1);

    // 3.2) make sure, that the new normal velocities are at least
    // fast as the growth rates --> no simultaneous re-collision

    const double velocityNormalEffective = velocityNormalNew1 - velocityNormalNew2;

    if (velocityNormalEffective < 0.)
    {
        // add the missing effective Velocity() to the spheres
        const double velocityDifference1 = -velocityNormalEffective * mass2 / (mass1 + mass2);
        const double velocityDifference2 = -velocityNormalEffective - velocityDifference1;

        velocityNormalNew1 += velocityDifference1;
        velocityNormalNew2 -= velocityDifference2;
    }

    // 3.3) add normal Velocity() boost
    const double velocityExtra = 2.0;
    velocityNormalNew1 += velocityExtra * s1.GrowthRate();
    velocityNormalNew2 -= velocityExtra * s2.GrowthRate();

    // 4) combine normal and tangential velocities, add growth rate for collision buffer
    return {velocityNormalNew1 * n + velocityTransversal1, velocityNormalNew2 * n + velocityTransversal2};
}

double PredictedCollisionTime(Sphere s1, Sphere s2)
{
    if (s1.Id() == s2.Id())
        return Inf();
    // sync both spheres to the more recent time
    if (s1.TimeOfLastUpdate() > s2.TimeOfLastUpdate())
        std::swap(s1, s2);

    const double baseTime = s1.TimeOfLastUpdate();

    const Eigen::Vector3d dP =
            s1.Position() - (s2.Position() + s2.Velocity() * (s1.TimeOfLastUpdate() - s2.TimeOfLastUpdate()));
    const Eigen::Vector3d dV = s1.Velocity() - s2.Velocity();

    const double dR = s1.Radius() + s2.Radius() + s2.GrowthRate() * (s1.TimeOfLastUpdate() - s2.TimeOfLastUpdate());
    const double dG = s1.GrowthRate() + s2.GrowthRate();

    const double a = dV.dot(dV) - dG * dG;
    const double b = 2 * (dV.dot(dP) - dR * dG);
    const double c = dP.dot(dP) - dR * dR;

    if (c < -2.e-10 * s1.Radius())
    {
        throw std::runtime_error("Sphere " + std::to_string(s1.Id()) + " overlaps with Sphere " +
                                 std::to_string(s2.Id()));
    }

    double discriminant = b * b - 4 * a * c;

    // allow small negative values ...
    if (discriminant < -1e-12)
        return Inf();

    // ... an treat them as zero
    discriminant = std::max(discriminant, 0.);

    if (b < 0.)
        return baseTime + 2 * c / (-b + std::sqrt(discriminant));

    if (a < 0.)
        return baseTime + (-b - std::sqrt(discriminant)) / (2 * a);

    return Inf();
}
} /* EDMD */
