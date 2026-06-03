# SPDX-License-Identifier: MIT-0

drake_example_add_executable(find_resource_example find_resource_example.cc)
add_test(NAME find_resource_example COMMAND find_resource_example)

add_test(NAME find_resource_example_py COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/find_resource_example.py"
)
set_tests_properties(find_resource_example_py PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
)
