# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed_apt)

include(CTest)

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

# Treat any use of a deprecated Drake API as a hard error: the compile flag
# fails the C++ compile, and the test environment (set per-test in src/)
# makes pydrake throw at the call site. We set these in our examples to
# ensure they stay up-to-date with latest Drake. You might decide to remove
# them or set to 'ignore' in your own project. Applied here (after the gtest
# target) so it scopes only to our own example sources.
add_compile_options(-Werror=deprecated-declarations)

add_subdirectory(src)
