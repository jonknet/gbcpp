cmake_minimum_required(VERSION 3.19)
project(GBCppClion)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES main.cpp cpu.cpp mm.cpp ops.cpp ppu.cpp utils.cpp oplookup.cpp helpers.h helpers.cpp)

add_executable(GBCppClion ${SOURCE_FILES})