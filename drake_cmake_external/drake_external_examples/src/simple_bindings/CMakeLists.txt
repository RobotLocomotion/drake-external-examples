# SPDX-License-Identifier: MIT-0

find_package(pybind11 CONFIG REQUIRED)

drake_example_pybind11_add_module(simple_bindings MODULE simple_bindings.cc)
# N.B. `pybind11_add_module` normally sets the default visibility to "hidden"
# to avoid warnings. However, we need the default visibility to be public so
# template instantions that are bound in Python (e.g. `drake::Value<>`)
# maintain the same RTTI across translation units.
# Additionally, in Drake we avoid mixing hidden and public symbols, so we
# can confidently re-enable this setting for Drake's symbols.
# For more information, see:
# https://github.com/pybind/pybind11/issues/2479
set_target_properties(simple_bindings PROPERTIES CXX_VISIBILITY_PRESET default)

drake_example_add_py_test(NAME simple_bindings_test COMMAND
    "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/simple_bindings_test.py")

set_property(TEST simple_bindings_test APPEND PROPERTY ENVIRONMENT
    "PYTHONPATH=$<TARGET_FILE_DIR:simple_bindings>:${drake_PYTHON_DIR}")
