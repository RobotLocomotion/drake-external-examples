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

# N.B. The two tests below must be tested here, rather than in a separate
# CMake script executed as a test, because `find_package(drake)` is not
# supported in that environment (we've installed using the WITH_USER_* options,
# so it tries to add those libraries, which cannot be done as a script).

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
