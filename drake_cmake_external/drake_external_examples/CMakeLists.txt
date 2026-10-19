# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_external_examples)

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
find_package(Git REQUIRED)
FetchContent_Declare(
  nanobind
  GIT_REPOSITORY https://github.com/wjakob/nanobind.git
  GIT_TAG v3.1.0
  GIT_SHALLOW ON
  # Reset-then-apply so re-configures don't fail on an already-patched tree.
  PATCH_COMMAND "${GIT_EXECUTABLE}" checkout -- cmake/nanobind-config.cmake
    COMMAND "${GIT_EXECUTABLE}" apply
      "${CMAKE_CURRENT_SOURCE_DIR}/patches/nanobind-stable-abi-linux.patch"
)
FetchContent_MakeAvailable(nanobind)

add_subdirectory(src)
