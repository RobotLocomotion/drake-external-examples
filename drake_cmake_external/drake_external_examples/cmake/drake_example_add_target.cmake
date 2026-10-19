# SPDX-License-Identifier: MIT-0

include_guard(GLOBAL)

# Drake target configuration wrappers that set common toolchain-related
# semantics (linking drake::drake, deprecation policy) across the example
# targets in this project.

function(drake_example_add_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  # Treat any use of a deprecated Drake C++ API as a hard compile error.
  # You might decide to ignore deprecation warnings in your own project.
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

function(drake_example_add_library target)
  add_library(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()

# TODO(#21572): This should be shipped as part of drake
function(drake_example_nanobind_add_module target)
  # Match pydrake's nanobind so our module shares its type registry.
  nanobind_add_module(${target} STABLE_ABI NB_DOMAIN pydrake ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()
