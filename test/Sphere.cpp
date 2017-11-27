YAML:25:11: error: unknown enumerated scalar
Standard: Cpp14
          ^~~~~
Error reading /home/schmaus/edmd/.clang-format: Invalid argument
#include "Sphere.h"
#include "BoostUnitTest.h"
#include <iostream>

BOOST_AUTO_TEST_CASE(blub) {
  EDMD::Sphere s;
  BOOST_CHECK(true);
}
