# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_cmake_installed)

include(CTest)
include(cmake/drake_example_add_target.cmake)
include(cmake/drake_example_add_test.cmake)

find_package(drake CONFIG REQUIRED)

find_package(Python3
  ${drake_PYTHON_VERSION} EXACT REQUIRED
  COMPONENTS Interpreter Development.Module
)

# nanobind resolves Python through the unversioned `Python::Module` target
# (distinct from the versioned `Python3` above). Find it in this (parent) scope
# so the target is visible where nanobind_add_module builds the extension in
# src/simple_bindings.
find_package(Python
  ${drake_PYTHON_VERSION} EXACT REQUIRED
  COMPONENTS Interpreter Development.Module
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

# Drake uses upstream nanobind and does not ship it, so downstream projects must
# provide their own copy that is ABI-compatible with pydrake. That requires a
# matching nanobind ABI version (see nanobind's FAQ on inter-extension type
# visibility, and its changelog for ABI-version info). Pinning the same version
# Drake builds against is the simplest way to guarantee it. Find Drake's version
# in the nanobind `bazel_dep` in its MODULE.bazel.
# Here we fetch and build it as part of the CMake build. A project with its own
# dependency infrastructure (pip, conan, vcpkg, a system package, ...) can
# instead delete this block and use `find_package(nanobind CONFIG REQUIRED)`.
# TODO(#21572): Drake should publish the nanobind version it builds against
# (e.g. in its installed CMake config) so downstream projects need not hardcode
# this magic number by reading MODULE.bazel.
find_package(Patch REQUIRED)
FetchContent_Declare(
  nanobind
  URL https://files.pythonhosted.org/packages/c7/1f/e28fae715401e8f57400e72aeaff4fde12046d31828164f34f61f0816da8/nanobind-3.1.0.tar.gz
  URL_HASH SHA256=6661e3dc1434d78781ccf7913442d90361dab395c3424efadb63bbb6ddea7ea3
  TLS_VERIFY ON
  PATCH_COMMAND "${Patch_EXECUTABLE}" -p0
    "--input=${CMAKE_CURRENT_SOURCE_DIR}/patches/nanobind-stable-abi-linux.patch"
)
FetchContent_MakeAvailable(nanobind)

add_subdirectory(src)
