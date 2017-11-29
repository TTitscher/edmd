#pragma once
#include <Eigen/Core>

namespace EDMD
{
class Sphere
{
public:
    Sphere(Eigen::Vector3d position, Eigen::Vector3d velocity, double radius, double growthRate, int id)
        : mId(id)
        , mPosition(position)
        , mVelocity(velocity)
        , mRadius(radius)
        , mGrowthRate(growthRate)
    {
    }

    void MoveAndGrow(double time)
    {
        double dT = time - mTimeOfLastUpdate;
        mPosition += mVelocity * dT;
        mRadius += mGrowthRate * dT;
        mTimeOfLastUpdate = time;
    }

    int Id() const
    {
        return mId;
    }

    Eigen::Vector3d Position() const
    {
        return mPosition;
    }

    Eigen::Vector3d Velocity() const
    {
        return mVelocity;
    }

    void Velocity(Eigen::Vector3d velocity)
    {
        mVelocity = velocity;
    }

    double Radius() const
    {
        return mRadius;
    }

    double GrowthRate() const
    {
        return mGrowthRate;
    }

    double TimeOfLastUpdate() const
    {
        return mTimeOfLastUpdate;
    }


private:
    int mId;
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mVelocity;
    double mRadius;
    double mGrowthRate;
    double mTimeOfLastUpdate = 0;
};

double Volume(const Sphere& s)
{
    return 4. / 3. * M_PI * s.Radius() * s.Radius() * s.Radius();
}

double KineticEnergy(const Sphere& s)
{
    return .5 * Volume(s) * s.Velocity().dot(s.Velocity());
}
} /* EDMD */
