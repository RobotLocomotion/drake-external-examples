# Verify installation behavior with -DBUILD_SHARED_LIBS=OFF
# -DDRAKE_INSTALL_PYTHON=OFF.

cmake_minimum_required(VERSION 3.16)

if(NOT CMAKE_ARGC GREATER_EQUAL 1)
    message(FATAL_ERROR "Expected a single argument, the CMAKE_PREFIX_PATH.")
endif()

# message(FATAL_ERROR "${CMAKE_ARGV0}")
# list(APPEND CMAKE_PREFIX_PATH "${CMAKE_ARGV0}")
# message(FATAL_ERROR "${CMAKE_PREFIX_PATH}")

find_package(drake CONFIG REQUIRED)

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
