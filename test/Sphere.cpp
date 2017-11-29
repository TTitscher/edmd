#include "Sphere.h"
#include "BoostUnitTest.h"
#include <iostream>

EDMD::Sphere TestSphere()
{
    EDMD::Sphere s(Eigen::Vector3d::Zero(), Eigen::Vector3d::Constant(2), 1, 42, 0);
    return s;
}

BOOST_AUTO_TEST_CASE(Volume)
{
    BOOST_CHECK_CLOSE(EDMD::Volume(TestSphere()), 4. / 3. * M_PI, 1.e-10);
}

BOOST_AUTO_TEST_CASE(Energy)
{
    auto s = TestSphere();
    double v = s.Velocity().norm();
    BOOST_CHECK_CLOSE(EDMD::KineticEnergy(s), 0.5 * EDMD::Volume(s) * v * v, 1.e-10);
}

BOOST_AUTO_TEST_CASE(MoveAndGrow)
{
    auto s = TestSphere();
    double t = 1;
    s.MoveAndGrow(t);

    BOOST_CHECK_CLOSE(s.TimeOfLastUpdate(), t, 1.e-10);
    BOOST_CHECK_CLOSE(s.Radius(), 43, 1.e-10);
    BoostUnitTest::CheckEigenMatrix(s.Position(), Eigen::Vector3d::Constant(2));

    // remain unchanged
    BoostUnitTest::CheckEigenMatrix(s.Velocity(), TestSphere().Velocity());
    BOOST_CHECK_CLOSE(s.GrowthRate(), TestSphere().GrowthRate(), 1.e-10);
}
