cmake_minimum_required(VERSION 3.13)
project(SuffixAutomata)

set (CMAKE_CXX_STANDART 17)

add_executable(SuffixAutomata src/Main.cpp src/SuffixAutomata.h src/SuffixAutomata.cpp src/ShrinkingArray.hpp)

add_executable(InputGenerator src/InputGenerator.cpp)