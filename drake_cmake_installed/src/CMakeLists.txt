# SPDX-License-Identifier: MIT-0

# Treat any use of a deprecated Drake API as a hard error: the compile flag
# fails the C++ compile, and the test environment (set per-test below)
# makes pydrake throw at the call site. We set these in our examples to
# ensure they stay up-to-date with latest Drake. You might decide to remove
# them or set to 'ignore' in your own project.
add_compile_options(-Werror=deprecated-declarations)

add_subdirectory(find_resource)
add_subdirectory(particle)
add_subdirectory(simple_bindings)
add_subdirectory(simple_continuous_time_system)

add_test(NAME import_all_test COMMAND
  "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
set_property(TEST import_all_test APPEND PROPERTY ENVIRONMENT
  "PYTHONPATH=${drake_PYTHON_DIR}"
  "DRAKE_DEPRECATION_RUNTIME_SEVERITY=error"
  "PYTHONWARNINGS=default"
)
