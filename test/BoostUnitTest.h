#pragma once
#define BOOST_TEST_MODULE DummyTestName
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

namespace BoostUnitTest
{
template <typename T1, typename T2>
void CheckEigenMatrix(const T1& r1, const T2& r2, double rTolerance = 1.e-10)
{
    BOOST_CHECK_EQUAL(r1.rows(), r2.rows());
    BOOST_CHECK_EQUAL(r1.cols(), r2.cols());

    double error = (r1 - r2).cwiseAbs().maxCoeff();
    bool isApprox = error < rTolerance;
    if (not isApprox)
    {
        BOOST_TEST_MESSAGE("abs max difference: " << error);
        BOOST_TEST_MESSAGE("1st matrix: \n" << r1);
        BOOST_TEST_MESSAGE("2nd matrix: \n" << r2);
    }
    BOOST_CHECK(isApprox);
}
} /* BoostUnitTest */
