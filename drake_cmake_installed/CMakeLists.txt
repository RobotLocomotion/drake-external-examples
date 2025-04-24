# SPDX-License-Identifier: MIT-0

# Note that the minimum required version of CMake to consume a binary
# installation of Drake is lower than the minimum required version
# when building Drake from source (as seen in drake_cmake_external).
# The maximum version below allows for the use of modern policies
# with newer versions of CMake.
cmake_minimum_required(VERSION 3.9...4.0)
project(drake_cmake_installed)

include(CTest)

find_package(drake CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

execute_process(COMMAND ${Python3_EXECUTABLE}-config --exec-prefix
  OUTPUT_VARIABLE PYTHON_EXEC_PREFIX
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
list(APPEND CMAKE_PREFIX_PATH "${PYTHON_EXEC_PREFIX}")

get_filename_component(DRAKE_PYTHONPATH
  "${drake_DIR}/../../python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages"
  REALPATH
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
add_subdirectory(third_party)
