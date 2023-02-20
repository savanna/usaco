cmake_minimum_required(VERSION 3.1)
project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# The version number.
set (VERSION_MAJOR 0)
set (VERSION_MINOR 3)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(test test.cpp)
