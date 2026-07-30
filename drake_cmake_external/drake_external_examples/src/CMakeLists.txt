# SPDX-License-Identifier: MIT-0

drake_example_add_py_test(NAME import_all_test
  COMMAND
    Python3::Interpreter -B "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
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

add_subdirectory(find_resource)
add_subdirectory(particle)
add_subdirectory(simple_bindings)
add_subdirectory(simple_continuous_time_system)
