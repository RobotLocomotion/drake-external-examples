# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed_apt)

include(CTest)
include(cmake/drake_example_add_target.cmake)
include(cmake/drake_example_add_test.cmake)

find_package(drake CONFIG REQUIRED PATHS /opt/drake)
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
  URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
  URL_HASH SHA256=65fab701d9829d38cb77c14acdc431d2108bfdbf8979e40eb8ae567edf10b27c
  TLS_VERIFY ON
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
