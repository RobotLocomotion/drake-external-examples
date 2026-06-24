# SPDX-License-Identifier: MIT-0

drake_example_add_library(particle particle.cc particle.h)

drake_example_add_executable(particle_test particle_test.cc)
target_link_libraries(particle_test PUBLIC particle GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(particle_test
  PROPERTIES
    LABELS small
    TIMEOUT 60
)

drake_example_add_py_test(NAME python_particle_test
  COMMAND Python3::Interpreter -B -m unittest particle_test
)
set_tests_properties(python_particle_test PROPERTIES
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/particle_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
