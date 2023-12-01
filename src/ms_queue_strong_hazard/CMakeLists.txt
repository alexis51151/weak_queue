cmake_minimum_required(VERSION 3.22)
project(COMP_522)

set(CMAKE_CXX_STANDARD 11)

add_executable(test_seq tests/test_seq.cpp)
add_executable(test_par tests/test_par.cpp)