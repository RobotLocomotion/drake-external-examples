# SPDX-License-Identifier: MIT-0

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)
add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
