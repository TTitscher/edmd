foreach(unittest 
        Id
        SphereVsSphere 
        SphereVsPlane
        )

    add_executable(${unittest} ${unittest}.cpp)
    target_link_libraries(${unittest} Eigen3::Eigen Boost::unit_test_framework)
    add_test(unit::${unittest} ${unittest})
endforeach()


foreach(integrationtest 
        SphereInBox 
        SpheresInBox 
        )

    add_executable(${integrationtest} ${integrationtest}.cpp)
    target_link_libraries(${integrationtest} Eigen3::Eigen Boost::unit_test_framework)
    add_test(integration::${integrationtest} ${integrationtest})
endforeach()
