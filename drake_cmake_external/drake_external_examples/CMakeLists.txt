# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_external_examples)

find_package(drake CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

# Drake target configuration wrappers that set common toolchain-related
# semantics (linking drake::drake, deprecation policy) across the example
# targets in this project.
function(drake_example_add_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  # Treat any use of a deprecated Drake C++ API as a hard compile error.
  # You might decide to ignore deprecation warnings in your own project.
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

function(drake_example_add_library target)
  add_library(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

drake_example_add_executable(simple_continuous_time_system simple_continuous_time_system.cc)

include(CTest)

add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
add_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
set_tests_properties(import_all_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
)

add_test(NAME solver_disabled_test
  COMMAND Python3::Interpreter -B -m unittest solver_disabled_test
)
set_tests_properties(solver_disabled_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/solver_disabled_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
