# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.16)
project(drake_external_examples)

find_package(drake CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

get_filename_component(DRAKE_PYTHONPATH
  "${drake_DIR}/../../python${drake_PYTHON_VERSION}/site-packages"
  REALPATH
)

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)

include(CTest)

add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
add_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
set_tests_properties(import_all_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${DRAKE_PYTHONPATH}"
)

add_test(NAME solver_disabled_test
  COMMAND Python3::Interpreter -B -m unittest solver_disabled_test
)
set_tests_properties(solver_disabled_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${DRAKE_PYTHONPATH}"
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/solver_disabled_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
