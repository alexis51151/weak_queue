cmake_minimum_required(VERSION 3.22)
project(COMP_522)

set(CMAKE_CXX_STANDARD 11)

add_executable(ms_queue_strong main.cpp
        ms_queue_strong.h
        pointer.h
        node.h)

add_executable(test_seq test_seq.cpp)
add_executable(test_par test_par.cpp)


target_link_libraries(ms_queue_strong PUBLIC -latomic)
target_link_libraries(test_seq PUBLIC -latomic)
target_link_libraries(test_par PUBLIC -latomic)
