# SPDX-License-Identifier: MIT-0

include_guard(GLOBAL)

# Wrapper around CMake's add_test() for Python example tests. In addition
# to registering the test, it sets common environment defaults:
#   PYTHONPATH so pydrake imports resolve.
#   DRAKE_DEPRECATION_RUNTIME_SEVERITY=error so DrakeDeprecationWarnings
#     from pydrake are propagated as errors. Without this, pydrake's
#     default "once" filter sits at position 0 of the warnings filter
#     list and shadows PYTHONWARNINGS=error for this category.
#   PYTHONWARNINGS=error so any other Python warning is also promoted
#     to a test failure.
# Use this in place of add_test(NAME ...) for Python tests.
function(drake_example_add_py_test)
  cmake_parse_arguments(_arg "" "NAME" "" ${ARGN})
  add_test(${ARGN})
  set_property(TEST ${_arg_NAME} APPEND PROPERTY ENVIRONMENT
    "DRAKE_DEPRECATION_RUNTIME_SEVERITY=error"
    "PYTHONPATH=${drake_PYTHON_DIR}"
    "PYTHONWARNINGS=error"
  )
endfunction()

# Wrapper around CMake's add_test() for C++ example tests. Pure passthrough;
# exists only so call sites are symmetric with drake_example_add_py_test().
function(drake_example_add_cc_test)
  add_test(${ARGN})
endfunction()

# Wrapper around CMake's gtest_discover_tests() for C++ example tests.
include(GoogleTest)
function(drake_example_discover_gtests)
  gtest_discover_tests(${ARGN})
endfunction()
