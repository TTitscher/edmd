foreach(unittest 
        SphereVsSphere 
        SphereVsPlane)

    add_executable(${unittest} ${unittest}.cpp)
    target_link_libraries(${unittest} Eigen3::Eigen Boost::unit_test_framework)
    add_test(unit::${unittest} ${unittest})
endforeach()
