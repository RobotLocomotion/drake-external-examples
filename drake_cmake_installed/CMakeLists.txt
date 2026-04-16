# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed)

include(CTest)

find_package(drake CONFIG REQUIRED)

option(DRAKE_COMPILE_IGNORE_DEPRECATED
  "Silence compile-time warnings at deprecated Drake C++ API call sites" OFF)
option(DRAKE_COMPILE_DEPRECATION_IS_ERROR
  "Make use of deprecated Drake C++ APIs a compile error" OFF)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
add_subdirectory(third_party)
