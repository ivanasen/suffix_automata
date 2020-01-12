cmake_minimum_required(VERSION 3.13)
project(SuffixAutomata)

set (CMAKE_CXX_STANDART 17)

add_executable(SuffixAutomata src/Main.cpp src/SuffixAutomata.h src/SuffixAutomata.cpp src/CompactArray.hpp)

add_executable(InputGenerator src/InputGenerator.cpp)

add_executable(RunTests tests/Main.cpp src/SuffixAutomata.h src/SuffixAutomata.cpp)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
