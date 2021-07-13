cmake_minimum_required(VERSION 3.12)
project(GBCppClion CXX)

set(CMAKE_CXX_STANDARD 17)

link_directories(lib)

#add_library(cpuimpl_lib STATIC cpu.cpp include/cpuimpl.h)

set(SOURCE_FILES main2.cpp)

add_executable(GBCppClion ${SOURCE_FILES})

target_compile_features(GBCppClion PUBLIC cxx_std_17)

target_include_directories(GBCppClion PUBLIC include include/lib)

target_link_libraries(GBCppClion SDL2 SDL2main stdc++fs)
#
#add_subdirectory(tests)

#install(FILES dll/fmtd.dll dll/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

