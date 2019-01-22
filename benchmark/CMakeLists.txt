add_executable(EventListType EventListType.cpp)
target_link_libraries(EventListType benchmark::benchmark) 

add_executable(Simulation Simulation.cpp)
target_link_libraries(Simulation benchmark::benchmark Eigen3::Eigen -lprofiler) 
