# SPDX-License-Identifier: MIT-0

add_subdirectory(find_resource)
add_subdirectory(particle)
add_subdirectory(simple_bindings)
add_subdirectory(simple_continuous_time_system)

drake_example_add_py_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
