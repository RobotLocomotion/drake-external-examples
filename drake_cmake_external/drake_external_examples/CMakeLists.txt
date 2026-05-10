# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.20...4.3)
project(drake_external_examples)

find_package(drake CONFIG REQUIRED)

find_package(Python3 ${drake_PYTHON_VERSION} EXACT REQUIRED COMPONENTS Interpreter Development)

# Treat any use of a deprecated Drake API as a hard error: the compile flag
# fails the C++ compile, and the test environment makes pydrake throw at the
# call site. We set these in our examples to ensure they stay up-to-date with
# latest Drake. You might decide to remove them or set to 'ignore' in your own
# project.
add_compile_options(-Werror=deprecated-declarations)

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
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR};DRAKE_DEPRECATION_RUNTIME_SEVERITY=error"
)

add_test(NAME solver_disabled_test
  COMMAND Python3::Interpreter -B -m unittest solver_disabled_test
)
set_tests_properties(solver_disabled_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR};DRAKE_DEPRECATION_RUNTIME_SEVERITY=error"
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/solver_disabled_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
