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
# * Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.5.1)
project(drake_cmake_installed)

# N.B. This is a temporary flag. It only really applies to Linux, as Mac
# does not need X11.
option(RUN_X11_TESTS "Run tests that require X11" OFF)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

include(CTest)

if(PYTHON_EXECUTABLE)
  find_package(PythonInterp MODULE REQUIRED)
else()
  set(DEFAULT_PYTHON_VERSION 3)
  if(UNIX AND NOT APPLE)
    find_program(LSB_RELEASE_EXECUTABLE NAMES lsb_release)
    if(LSB_RELEASE_EXECUTABLE)
      execute_process(COMMAND "${LSB_RELEASE_EXECUTABLE}" --codename --short
        OUTPUT_VARIABLE LSB_RELEASE_OUTPUT_VARIABLE
        OUTPUT_STRIP_TRAILING_WHITESPACE
      )
      if(LSB_RELEASE_OUTPUT_VARIABLE STREQUAL xenial)
        set(DEFAULT_PYTHON_VERSION 2)
      endif()
    endif()
  endif()
  set(WITH_PYTHON_VERSION ${DEFAULT_PYTHON_VERSION} CACHE STRING
    "Choose the version of Python to use, options are 2 3"
  )
  set_property(CACHE WITH_PYTHON_VERSION PROPERTY STRINGS "2;3")
  if(APPLE AND WITH_PYTHON_VERSION EQUAL 2)
    find_program(PYTHON_EXECUTABLE NAMES python2)
  endif()
  find_package(PythonInterp ${WITH_PYTHON_VERSION} EXACT MODULE REQUIRED)
endif()

find_package(drake CONFIG REQUIRED)

get_filename_component(PYTHONPATH
  "${drake_DIR}/../../python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages"
  REALPATH
)

add_subdirectory(src)
