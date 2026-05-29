# SPDX-License-Identifier: MIT-0

# Treat any use of a deprecated Drake C++ API as a hard compile error.
# We set this in our examples to ensure they stay up-to-date with latest
# Drake. You might decide to remove this or ignore deprecation warnings
# in your own project.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=deprecated-declarations")

add_subdirectory(find_resource)
add_subdirectory(particle)
add_subdirectory(simple_bindings)
add_subdirectory(simple_continuous_time_system)

add_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
set_tests_properties(import_all_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
)
