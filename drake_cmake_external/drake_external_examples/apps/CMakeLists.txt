# SPDX-License-Identifier: MIT-0

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)
add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)

add_test(NAME import_all_test COMMAND
  "${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/import_all_test.py"
)
set_tests_properties(import_all_test PROPERTIES
  ENVIRONMENT "PYTHONPATH=${PYTHONPATH}"
)
