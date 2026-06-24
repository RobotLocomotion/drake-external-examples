# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_external_examples)

include(CTest)
include(cmake/drake_example_add_target.cmake)
include(cmake/drake_example_add_test.cmake)

find_package(drake CONFIG REQUIRED)
find_package(Python3
  ${drake_PYTHON_VERSION} EXACT REQUIRED
  COMPONENTS Interpreter Development
)

drake_example_add_executable(simple_continuous_time_system
  simple_continuous_time_system.cc
)

drake_example_add_cc_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
drake_example_add_py_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)

drake_example_add_py_test(NAME solver_disabled_test
  COMMAND Python3::Interpreter -B -m unittest solver_disabled_test
)
set_tests_properties(solver_disabled_test PROPERTIES
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/solver_disabled_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
