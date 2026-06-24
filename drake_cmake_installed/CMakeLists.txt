# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed)

include(CTest)
include(cmake/drake_example_add_target.cmake)
include(cmake/drake_example_add_test.cmake)

find_package(drake CONFIG REQUIRED)

find_package(Python3
  ${drake_PYTHON_VERSION} EXACT REQUIRED
  COMPONENTS Interpreter Development
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip # release 1.17.0
  URL_HASH SHA256=9a56a54ae784394ff664cd55e8f4c9a03b503ebf0cb99576321c78ab3d87ca84
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
