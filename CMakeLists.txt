cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(edmd VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(Eigen3 3.2 REQUIRED NO_MODULE)

include_directories(src)


find_package(benchmark)
if(benchmark_FOUND)
    message(STATUS "GoogleBenchmark found")
    add_subdirectory(benchmark)
else()
    message(STATUS "GoogleBenchmark NOT found - Benchmarks won't be build!")
endif()

enable_testing()
add_subdirectory(test)
