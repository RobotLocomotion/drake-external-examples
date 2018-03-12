# -*- mode: cmake -*-
# vi: set ft=cmake :

# Copyright (c) 2018, Toyota Research Institute.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

find_package(pybind11 CONFIG REQUIRED)

# N.B. Since we create our own CMake for `pybind11`, our definitions for
# `pybind11::module` and `pybind11::pybind11` do not work the same as it would
# for pybind11's documented usage of the interface target libraries:
# http://pybind11.readthedocs.io/en/stable/compiling.html#advanced-interface-library-target
# For this reason, we use `pybind11` as it's a simpler for now.
pybind11_add_module(example_module MODULE example_module.cc)
# N.B. Because `pybind11_add_module` does not use
# `pybind11::module` and `pybind11_INCLUDE_DIRS` from `cps2cmake` should be
# empty, we must explicitly link it to `pybind11::module`.
target_link_libraries(example_module PUBLIC drake::drake pybind11::module)
# Revert visibilty preset set by `pybind11_add_module` (workaround for
# RobotLocomotion/drake#8540).
set_target_properties(example_module PROPERTIES CXX_VISIBILITY_PRESET default)

add_test(NAME example_module_test COMMAND
    "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/example_module_test.py")
set_tests_properties(example_module_test PROPERTIES
    # Using `pybind11/tests/test_cmake_build/installed_target` as an example.
    ENVIRONMENT "PYTHONPATH=$<TARGET_FILE_DIR:example_module>:${PYTHONPATH}")
