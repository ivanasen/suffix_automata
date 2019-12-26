cmake_minimum_required(VERSION 3.15)
project(SuffixAutomaton)

set(CMAKE_CXX_STANDARD 17)

add_executable(SuffixAutomaton src/main.cpp src/SuffixAutomaton.cpp src/SuffixAutomaton.h)

add_executable(InputGenerator src/InputGenerator.cpp)