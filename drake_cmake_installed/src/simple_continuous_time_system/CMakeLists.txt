# SPDX-License-Identifier: MIT-0

drake_example_add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
