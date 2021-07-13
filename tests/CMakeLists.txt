cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 17)
project(tests CXX)

add_executable(tests test2.cpp)

link_directories("C:/Program Files (x86)/Windows SDK/")

include(../PVS-Studio.cmake)

target_compile_options(tests PRIVATE /MTd)

