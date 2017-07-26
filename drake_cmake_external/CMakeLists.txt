# -*- mode: cmake -*-
# vi: set ft=cmake :

# Copyright (c) 2017, Massachusetts Institute of Technology.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the Massachusetts Institute of Technology nor the names
#   of its contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
# NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE MASSACHUSETTS
# INSTITUTE OF TECHNOLOGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.5.1)
project(drake_cmake_external)

find_program(BAZEL_EXECUTABLE bazel)

if(NOT BAZEL_EXECUTABLE)
  message(FATAL_ERROR "Could NOT find bazel executable")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(BAZEL_COMPILATION_MODE dbg)
else()
  set(BAZEL_COMPILATION_MODE opt)
endif()

set(BAZEL_ENV CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER})
set(BAZEL_ARGS --compilation_mode=${BAZEL_COMPILATION_MODE} --config=unsupported_crosstool --cxxopt=-std=c++1y)

if(CMAKE_VERBOSE_MAKEFILE)
  set(BAZEL_ARGS --subcommands ${BAZEL_ARGS})
endif()

set(BAZEL_TARGETS //:install)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install" CACHE STRING "Install prefix" FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

include(ExternalProject)

ExternalProject_Add(drake
  URL https://github.com/RobotLocomotion/drake/archive/65c4366ea2b63278a286b1e22b8d464d50fbe365.tar.gz
  URL_HASH SHA256=899d98485522a7cd5251e50a7a6b8a64e40aff2a3af4951a3f0857fd938cafca
  TLS_VERIFY 1
  SOURCE_DIR "${PROJECT_BINARY_DIR}/drake"
  CONFIGURE_COMMAND :
  BUILD_COMMAND ${BAZEL_ENV} "${BAZEL_EXECUTABLE}" build ${BAZEL_ARGS} ${BAZEL_TARGETS}
  BUILD_IN_SOURCE 1
  BUILD_ALWAYS 1
  INSTALL_COMMAND ${BAZEL_ENV} "${BAZEL_EXECUTABLE}" run ${BAZEL_ARGS} ${BAZEL_TARGETS} -- "${CMAKE_INSTALL_PREFIX}"
  USES_TERMINAL_BUILD 1
  USES_TERMINAL_INSTALL 1
)

ExternalProject_Add(drake_shambhala
  DEPENDS drake
  SOURCE_DIR "${PROJECT_SOURCE_DIR}/drake_shambhala"
  CMAKE_ARGS
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
  BINARY_DIR "${PROJECT_BINARY_DIR}/drake_shambhala"
  BUILD_ALWAYS 1
)
