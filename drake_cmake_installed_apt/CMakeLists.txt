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
  URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.tar.gz # release 1.17.0
  URL_HASH SHA256=745c55415660044610f7fcd3af7a6420d5de16a7dbb9ebfe2e131275676232be
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
