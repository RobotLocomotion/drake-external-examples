# SPDX-License-Identifier: MIT-0

# Note that the minimum required version of CMake to consume a binary
# installation of Drake is lower than the minimum required version
# when building Drake from source (as seen in drake_cmake_external).
# The maximum version below allows for the use of modern policies
# with newer versions of CMake.
cmake_minimum_required(VERSION 3.9...4.2)
project(drake_cmake_installed)

include(CTest)

find_package(drake CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
add_subdirectory(third_party)
