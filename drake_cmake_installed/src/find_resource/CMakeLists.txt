# SPDX-License-Identifier: MIT-0

drake_example_add_executable(find_resource_example find_resource_example.cc)
drake_example_add_cc_test(NAME find_resource_example COMMAND find_resource_example)

drake_example_add_py_test(NAME find_resource_example_py COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/find_resource_example.py"
)
