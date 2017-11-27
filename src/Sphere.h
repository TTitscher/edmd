#pragma once
#include <Eigen/Core>

namespace EDMD
{
struct Sphere
{
    Eigen::Vector3d position = Eigen::Vector3d::Zero();
    Eigen::Vector3d velocity = Eigen::Vector3d::Zero();
    double radius = 0;
    double growthRate = 0;
    double timeOfLastUpdate = 0;
    int id = 0;
};

void MoveAndGrow(Sphere& rS, double time)
{
    double dT = time - rS.timeOfLastUpdate;
    rS.position += rS.velocity * dT;
    rS.radius += rS.growthRate * dT;
    rS.timeOfLastUpdate = time;
}

double Volume(const Sphere& s)
{
    return 4. / 3. * M_PI * s.radius * s.radius * s.radius;
}

double KineticEnergy(const Sphere& s)
{
    return .5 * s.radius * s.radius * s.radius * s.velocity.dot(s.velocity);
}
} /* EDMD */
