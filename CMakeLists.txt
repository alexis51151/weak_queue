cmake_minimum_required(VERSION 3.22)
project(COMP_522)

set(CMAKE_CXX_STANDARD 11)

add_executable(ms_queue_strong src/main.cpp
        src/ms_queue_strong/ms_queue_strong.cpp
        src/ms_queue_strong_hazard/hazard_ptr.cpp
        src/ms_queue_strong_hazard/hazard_ptr.h)

target_link_libraries(ms_queue_strong PUBLIC -latomic)

include_directories(include)