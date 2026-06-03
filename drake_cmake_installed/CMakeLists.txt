# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed)

include(CTest)

find_package(drake CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Wrappers around add_executable / add_library that additionally treat any
# use of a deprecated Drake C++ API as a hard compile error. We use these in
# our examples to ensure they stay up-to-date with latest Drake. You might
# decide to call add_executable / add_library directly or ignore deprecation
# warnings in your own project.
macro(drake_example_add_executable target)
  add_executable(${target} ${ARGN})
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endmacro()

macro(drake_example_add_library target)
  add_library(${target} ${ARGN})
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endmacro()

macro(drake_example_pybind11_add_module target)
  pybind11_add_module(${target} ${ARGN})
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endmacro()

add_subdirectory(src)
add_subdirectory(third_party)
