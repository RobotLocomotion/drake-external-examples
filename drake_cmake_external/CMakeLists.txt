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
  URL https://gitlab.com/libeigen/eigen/-/archive/5.0.1/eigen-5.0.1.tar.gz
  URL_HASH SHA256=e9c326dc8c05cd1e044c71f30f1b2e34a6161a3b6ecf445d56b53ff1669e3dec
  TLS_VERIFY ON
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=${CMAKE_VERBOSE_MAKEFILE}
    # Eigen's default option is to build vendored dependencies and other
    # development tools (tests, documentation, etc.) when building from source
    # via `ExternalProject_Add`. Since these tools aren't needed for this
    # example, we'll disable them explicitly.
    -DEIGEN_BUILD_BLAS:BOOL=OFF
    -DEIGEN_BUILD_LAPACK:BOOL=OFF
    -DEIGEN_BUILD_TESTING:BOOL=OFF
    -DEIGEN_BUILD_DOC:BOOL=OFF
    -DEIGEN_BUILD_DEMOS:BOOL=OFF
  BUILD_ALWAYS ON
)

# This shows how to rebuild fmt from source as part of Drake's CMake build.
# If you'd rather just use your operating system's fmt, then this stanza could
# be removed (as well as removing -DWITH_USER_FMT:BOOL=ON, below).
# If you rebuild fmt from source, then you must also rebuild spdlog from source.
ExternalProject_Add(fmt
  URL https://github.com/fmtlib/fmt/archive/refs/tags/12.1.0.tar.gz
  URL_HASH SHA256=ea7de4299689e12b6dddd392f9896f08fb0777ac7168897a244a6d6085043fea
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
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.tar.gz
  URL_HASH SHA256=d8862955c6d74e5846b3f580b1605d2428b11d97a410d86e2fb13e857cd3a744
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
    # The Drake build also supports installing a static drake::drake library,
    # instead of shared (the default). This is incompatible with certain other
    # options, and as a result, changes their default values; see
    # https://drake.mit.edu/from_source.html for the full details. To enable
    # this option, and disable incompatible installation options, uncomment the
    # following line.
    # -DBUILD_SHARED_LIBS:BOOL=OFF
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
