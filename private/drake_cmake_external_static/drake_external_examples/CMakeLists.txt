# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.16)
project(drake_external_examples)

find_package(drake CONFIG REQUIRED)

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)

include(CTest)

add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)

# Verify that drake::drake is a static library, as we've requested in
# CMakeLists.txt.
get_target_property(drake_type drake::drake TYPE)
if (NOT drake_type STREQUAL "STATIC_LIBRARY")
  message(FATAL_ERROR
    "Expected drake::drake to be STATIC_LIBRARY, but got ${drake_type}."
  )
endif()

# Verify that Python-related variables are not defined.
if (DEFINED drake_PYTHON_VERSION OR DEFINED drake_PYTHON_DIR)
  message(FATAL_ERROR
    "Expected Python variables undefined, but got ${drake_PYTHON_VERSION} ${drake_PYTHON_DIR}."
  )
endif()

# Verify that the pybind11 headers are not installed.
include(CheckIncludeFileCXX)
get_target_property(drake_include_dirs drake::drake INTERFACE_INCLUDE_DIRECTORIES)
list(GET drake_include_dirs 0 drake_include_dir)
file(GLOB pybind11_dir "${drake_include_dir}/pybind11")
if(pybind11_dir)
  message(FATAL_ERROR "Found unexpected 'pybind11' in installed include directory.")
endif()

# Verify that the LCM runtime is disabled.
add_executable(lcm_disabled lcm_disabled_test.cc)
target_link_libraries(lcm_disabled drake::drake)
add_test(NAME lcm_disabled_test COMMAND lcm_disabled)
