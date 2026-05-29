# SPDX-License-Identifier: MIT-0

add_library(particle particle.cc particle.h)
target_link_libraries(particle drake::drake)
# Treat any use of a deprecated Drake C++ API as a hard compile error.
# We set this in our examples to ensure they stay up-to-date with latest
# Drake. You might decide to remove this or ignore deprecation warnings
# in your own project.
target_compile_options(particle PRIVATE
  -Werror=deprecated-declarations
)

add_executable(particle_test particle_test.cc)
target_link_libraries(particle_test particle drake::drake gtest)
target_compile_options(particle_test PRIVATE
  -Werror=deprecated-declarations
)
add_test(NAME cc_particle_test COMMAND particle_test)
set_tests_properties(cc_particle_test PROPERTIES LABELS small TIMEOUT 60)

add_test(NAME python_particle_test
  COMMAND Python3::Interpreter -B -m unittest particle_test
)
set_tests_properties(python_particle_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${drake_PYTHON_DIR}"
  LABELS small
  REQUIRED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/particle_test.py"
  TIMEOUT 60
  WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
