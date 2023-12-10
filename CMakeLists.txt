cmake_minimum_required(VERSION 3.22)
project(COMP_522)

set(CMAKE_CXX_STANDARD 11)

add_executable(bench src/benchmark.cpp)

#target_link_libraries(bench PUBLIC -latomic)
