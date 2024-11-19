# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.16)
project(drake_cmake_installed_apt)

include(CTest)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads MODULE REQUIRED)

add_library(gtest STATIC
  third_party/googletest/1.10/googletest/include/gtest/gtest.h
  third_party/googletest/1.10/googletest/src/gtest-all.cc
)
target_include_directories(gtest PUBLIC
  third_party/googletest/1.10/googletest/include
)
target_link_libraries(gtest Threads::Threads)

find_package(drake CONFIG REQUIRED PATHS /opt/drake)
find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

get_filename_component(DRAKE_PYTHONPATH "${drake_DIR}" DIRECTORY)
get_filename_component(DRAKE_PYTHONPATH "${DRAKE_PYTHONPATH}" DIRECTORY)
set(DRAKE_PYTHONPATH
  "${DRAKE_PYTHONPATH}/python${drake_PYTHON_VERSION}/site-packages"
)

add_subdirectory(src)

