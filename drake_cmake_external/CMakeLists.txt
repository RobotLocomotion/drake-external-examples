# SPDX-License-Identifier: MIT-0

# TODO(eric.cousineau): Link to documentation on superproject example pending
# resolution of: https://gitlab.kitware.com/cmake/cmake/issues/18336

# See https://drake.mit.edu/from_source.html for the versions of CMake
# officially supported by Drake when building from source.
cmake_minimum_required(VERSION 3.22...4.0)
project(drake_cmake_external)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are None Debug Release RelWithDebInfo MinSizeRel"
    FORCE
  )
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS None Debug Release RelWithDebInfo MinSizeRel
  )
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE STRING
    "Install path prefix, prepended onto install directories" FORCE
  )
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

include(ExternalProject)

# This shows how to fetch Eigen from source as part of Drake's CMake build.
# If you'd rather just use your operating system's Eigen, then this stanza could
# be removed (as well as removing -DWITH_USER_EIGEN:BOOL=ON, below).
ExternalProject_Add(eigen
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
  URL_HASH SHA256=8586084f71f9bde545ee7fa6d00288b264a2b7ac3607b974e54d13e7162c1c72
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    # Suppress warnings from Eigen's deprecated upstream CMake.
    # 1. Using find_package(CUDA), which has been replaced by more robust CUDA
    # language support since CMake 3.27. Eigen officially enforces the OLD
    # policy, but doesn't properly cover their "unsupported" code.
    # See https://cmake.org/cmake/help/latest/module/FindCUDA.html or run
    # cmake --help-policy CMP0146 for details.
    -DCMAKE_POLICY_DEFAULT_CMP0146:STRING=OLD
    # 2. Using find_package(Boost), which has been replaced by Boost's upstream
    # package configuration file since CMake 3.30 and Boost 1.70.
    # find_package(Boost) now defers to upstream to keep up with their
    # releases. Since Eigen allows Boost from 1.53 (as of writing), we can't
    # enforce the NEW policy here.
    # See https://cmake.org/cmake/help/latest/module/FindBoost.html or run
    # cmake --help-policy CMP0167 for details.
    -DCMAKE_POLICY_DEFAULT_CMP0167:STRING=OLD
  BUILD_ALWAYS ON
)

# This shows how to rebuild fmt from source as part of Drake's CMake build.
# If you'd rather just use your operating system's fmt, then this stanza could
# be removed (as well as removing -DWITH_USER_FMT:BOOL=ON, below).
# If you rebuild fmt from source, then you must also rebuild spdlog from source.
ExternalProject_Add(fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/11.0.2.tar.gz
  URL_HASH SHA256=6cb1e6d37bdcb756dbbe59be438790db409cdb4868c66e888d5df9f13f7c027f
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    -DBUILD_SHARED_LIBS=ON
    -DCMAKE_CXX_FLAGS:STRING=-fPIC
    -DFMT_INC_DIR:PATH=include/fmt
    -DFMT_TEST=OFF
  BUILD_ALWAYS ON
)

# This shows how to rebuild spdlog from source as part of Drake's CMake build.
# If you'd rather just use your operating system's fmt, then this stanza could
# be removed (as well as removing -DWITH_USER_SPDLOG:BOOL=ON, below).
# If you rebuild spdlog from source, then you must also rebuild fmt from source.
ExternalProject_Add(spdlog
  DEPENDS fmt
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.15.0.tar.gz
  URL_HASH SHA256=9962648c9b4f1a7bbc76fd8d9172555bad1871fdb14ff4f842ef87949682caa5
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    -DSPDLOG_BUILD_SHARED=ON
    -DCMAKE_CXX_FLAGS:STRING=-fPIC
    -DSPDLOG_FMT_EXTERNAL:BOOL=ON
    -Dfmt_DIR=${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt
)

# This is the external project PREFIX (not to be confused with the install
# prefix) which will contain, among other things, the checkout of Drake's
# source. This should match the default value, but for paranoia's sake, we'll
# also pass this explicitly to ExternalProject_Add.
set(DRAKE_PREFIX "${PROJECT_BINARY_DIR}/drake-prefix")

# Options to override Drake's latest source on master with a specific commit.
# See README.md for details.
set(DRAKE_COMMIT_HASH "master" CACHE STRING "Commit hash for Drake")
set(DRAKE_COMMIT_SHA256 CACHE STRING "SHA256 hash value for Drake commit archive")

if (NOT "${DRAKE_COMMIT_SHA256}" STREQUAL "")
  string(PREPEND DRAKE_COMMIT_SHA256 "SHA256=")
endif()

ExternalProject_Add(drake
  DEPENDS eigen fmt spdlog
  URL https://github.com/RobotLocomotion/drake/archive/${DRAKE_COMMIT_HASH}.tar.gz
  URL_HASH ${DRAKE_COMMIT_SHA256}
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    -DPYTHON_EXECUTABLE:FILEPATH=${Python3_EXECUTABLE}
    -DWITH_USER_EIGEN:BOOL=ON
    -DWITH_USER_FMT:BOOL=ON
    -DWITH_USER_SPDLOG:BOOL=ON
    # The Drake build has options to turn features on/off. For the full list,
    # see https://drake.mit.edu/from_source.html. Here, we demonstrate how to
    # set an arbitrary option for one of the open-source dependencies.
    -DWITH_CSDP:BOOL=OFF
  PREFIX "${DRAKE_PREFIX}"
  BINARY_DIR "${PROJECT_BINARY_DIR}/drake"
  BUILD_ALWAYS ON
)

ExternalProject_Add(drake_external_examples
  DEPENDS drake
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/drake_external_examples"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
  BINARY_DIR "${PROJECT_BINARY_DIR}/drake_external_examples"
  BUILD_ALWAYS ON
  INSTALL_COMMAND :
)
