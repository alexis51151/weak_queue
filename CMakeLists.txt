cmake_minimum_required(VERSION 3.22)
project(COMP_522)

set(CMAKE_CXX_STANDARD 11)

add_executable(ms_queue_strong src/main.cpp
        src/ms_queue_strong/ms_queue_strong.cpp)
add_executable(test_seq tests/test_seq.cpp)
add_executable(test_par tests/test_par.cpp)
add_executable(test_cds tests/test_cds.cpp)

target_link_libraries(ms_queue_strong PUBLIC -latomic)
target_link_libraries(test_seq PUBLIC -latomic)
target_link_libraries(test_par PUBLIC -latomic)


include_directories(include)