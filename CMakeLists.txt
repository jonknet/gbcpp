cmake_minimum_required(VERSION 3.19)
project(GBCppClion)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_directories(lib)

#add_library(cpuimpl_lib STATIC cpu.cpp include/cpuimpl.h)

set(SOURCE_FILES ppu.cpp mm.cpp cpu.cpp utils.cpp main.cpp)

add_executable(GBCppClion ${SOURCE_FILES})

target_link_libraries(GBCppClion SDL2 SDL2main fmtd)

#add_subdirectory(tests)

install(FILES dll/fmtd.dll dll/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
