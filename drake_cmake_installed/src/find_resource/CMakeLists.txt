# SPDX-License-Identifier: MIT-0

add_executable(find_resource_example find_resource_example.cc)
target_link_libraries(find_resource_example drake::drake)
# Treat any use of a deprecated Drake C++ API as a hard compile error.
# We set this in our examples to ensure they stay up-to-date with latest
# Drake. You might decide to remove this or ignore deprecation warnings
# in your own project.
target_compile_options(find_resource_example PRIVATE
  -Werror=deprecated-declarations
)
add_test(NAME find_resource_example COMMAND find_resource_example)

add_test(NAME find_resource_example_py COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/find_resource_example.py"
)
set_tests_properties(find_resource_example_py PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
)
