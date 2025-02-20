# SPDX-License-Identifier: MIT-0

# TODO(eric.cousineau): Link to documentation on superproject example pending
# resolution of: https://gitlab.kitware.com/cmake/cmake/issues/18336

cmake_minimum_required(VERSION 3.16)
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
# be removed (as well as removing -DWITH_USER_EIGEN:BOOLEAN=ON, below).
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
  BUILD_ALWAYS ON
)

# This shows how to rebuild fmt from source as part of Drake's CMake build.
# If you'd rather just use your operating system's fmt, then this stanza could
# be removed (as well as removing -DWITH_USER_FMT:BOOLEAN=ON, below).
# If you rebuild fmt from source, then you must also rebuild spdlog from source.
ExternalProject_Add(fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/6.1.2.tar.gz
  URL_HASH SHA256=1cafc80701b746085dddf41bd9193e6d35089e1c6ec1940e037fcb9c98f62365
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
# be removed (as well as removing -DWITH_USER_SPDLOG:BOOLEAN=ON, below).
# If you rebuild spdlog from source, then you must also rebuild fmt from source.
ExternalProject_Add(spdlog
  DEPENDS fmt
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.6.0.tar.gz
  URL_HASH SHA256=0421667c9f2fc78e6548d44f7bc5921be0f03e612df384294c16cedb93d967f8
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
    -DSPDLOG_FMT_EXTERNAL:BOOLEAN=ON
    -Dfmt_DIR=${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt
)

# This is the external project PREFIX (not to be confused with the install
# prefix) which will contain, among other things, the checkout of Drake's
# source. This should match the default value, but for paranoia's sake, we'll
# also pass this explicitly to ExternalProject_Add.
set(DRAKE_PREFIX "${PROJECT_BINARY_DIR}/drake-prefix")

ExternalProject_Add(drake
  DEPENDS eigen fmt spdlog
  # URL https://github.com/RobotLocomotion/drake/archive/master.tar.gz
  # Or from a commit (download and use "shasum -a 256 'xxx.tar.gz'" on it to
  # get the URL_HASH.
  URL https://github.com/RobotLocomotion/drake/archive/f848b02282cda2128475aa9a10db704a086b9752.tar.gz
  URL_HASH SHA256=baee279a679268eddb98fa8ec0356dae8ec0e86520e80e5559e73030f9c8cca2
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    -DPYTHON_EXECUTABLE:FILEPATH=${Python3_EXECUTABLE}
    -DWITH_USER_EIGEN:BOOLEAN=ON
    -DWITH_USER_FMT:BOOLEAN=ON
    -DWITH_USER_SPDLOG:BOOLEAN=ON
    # The Drake build has options to turn features on/off.
    # See https://drake.mit.edu/from_source.html for the full list.
    # Here, we demonstrate how to set an arbitrary option
    # for one of the open-source dependencies.
    -DWITH_CSDP:BOOLEAN=OFF
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
