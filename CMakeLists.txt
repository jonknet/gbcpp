cmake_minimum_required(VERSION 3.19)
project(GBCppClion)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)

include_guard(GLOBAL)

set(SOURCE_FILES ppu.cpp mm.cpp cpu.cpp cpuimpl.cpp ops.cpp utils.cpp main.cpp)

add_executable(GBCppClion ${SOURCE_FILES})

target_link_libraries(GBCppClion SDL2 SDL2main fmtd)