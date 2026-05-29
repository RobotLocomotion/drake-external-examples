# SPDX-License-Identifier: MIT-0

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)
# Treat any use of a deprecated Drake C++ API as a hard compile error.
# We set this in our examples to ensure they stay up-to-date with latest
# Drake. You might decide to remove this or ignore deprecation warnings
# in your own project.
target_compile_options(simple_continuous_time_system PRIVATE
  -Werror=deprecated-declarations
)
add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
