# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed_apt)

include(CTest)
include(cmake/drake_example_add_test.cmake)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
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

# Drake target configuration wrappers that set common toolchain-related
# semantics (linking drake::drake, deprecation policy) across the example
# targets in this project.
function(drake_example_add_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  # Treat any use of a deprecated Drake C++ API as a hard compile error.
  # You might decide to ignore deprecation warnings in your own project.
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

function(drake_example_add_library target)
  add_library(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

add_subdirectory(src)
