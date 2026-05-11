# SPDX-License-Identifier: MIT-0

add_library(particle particle.cc particle.h)
target_link_libraries(particle drake::drake)

add_executable(particle_test particle_test.cc)
target_link_libraries(particle_test particle drake::drake gtest)
add_test(NAME cc_particle_test COMMAND particle_test)
set_tests_properties(cc_particle_test PROPERTIES LABELS small TIMEOUT 60)

add_test(NAME python_particle_test
  COMMAND Python3::Interpreter -B -m unittest particle_test
)
set_tests_properties(python_particle_test PROPERTIES
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/particle_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
set_property(TEST python_particle_test APPEND PROPERTY ENVIRONMENT
  "PYTHONPATH=${drake_PYTHON_DIR}"
  "DRAKE_DEPRECATION_RUNTIME_SEVERITY=error"
  "PYTHONWARNINGS=default"
)
