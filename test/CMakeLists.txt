add_executable(Sphere Sphere.cpp)
target_link_libraries(Sphere Eigen3::Eigen Boost::unit_test_framework)
add_test(unit::Sphere Sphere)
