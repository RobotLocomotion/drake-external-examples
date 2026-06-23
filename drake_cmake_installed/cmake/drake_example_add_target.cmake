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

function(drake_example_pybind11_add_module target)
  pybind11_add_module(${target} ${ARGN})
  target_link_libraries(${target} PUBLIC drake::drake)
  target_compile_options(${target} PRIVATE -Werror=deprecated-declarations)
endfunction()
