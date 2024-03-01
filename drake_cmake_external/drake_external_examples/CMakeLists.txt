# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.10.2)
project(drake_external_examples)

include(CTest)

if(APPLE)
  set(FIND_PYTHON_INTERP_VERSION 3.12)
else()
  set(FIND_PYTHON_INTERP_VERSION 3.6)
endif()
find_package(PythonInterp ${FIND_PYTHON_INTERP_VERSION} MODULE REQUIRED)

find_package(drake CONFIG REQUIRED)

get_filename_component(PYTHONPATH
  "${drake_DIR}/../../python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/site-packages"
  REALPATH
)

add_subdirectory(apps)
