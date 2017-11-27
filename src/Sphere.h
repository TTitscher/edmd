#include <Eigen/Core>

namespace EDMD
{
struct Sphere
{
    Eigen::Vector3d position;
    Eigen::Vector3d velocity;
    double radius;
};
} /* EDMD */
