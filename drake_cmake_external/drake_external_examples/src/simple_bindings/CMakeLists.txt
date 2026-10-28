# SPDX-License-Identifier: MIT-0

# nanobind is fetched at the project top-level (see ../../CMakeLists.txt).
drake_example_nanobind_add_module(simple_bindings simple_bindings.cc)

drake_example_add_py_test(NAME simple_bindings_test COMMAND
    "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/simple_bindings_test.py")

set_property(TEST simple_bindings_test APPEND PROPERTY ENVIRONMENT
    "PYTHONPATH=$<TARGET_FILE_DIR:simple_bindings>:${drake_PYTHON_DIR}")
