# SPDX-License-Identifier: MIT-0

add_executable(deprecated_example deprecated_example.cc)
target_link_libraries(deprecated_example drake::drake)

if(DRAKE_COMPILE_IGNORE_DEPRECATED AND DRAKE_COMPILE_DEPRECATION_IS_ERROR)
  message(FATAL_ERROR
    "DRAKE_COMPILE_IGNORE_DEPRECATED and DRAKE_COMPILE_DEPRECATION_IS_ERROR "
    "are mutually exclusive.")
endif()
if(DRAKE_COMPILE_IGNORE_DEPRECATED)
  target_compile_definitions(deprecated_example
    PRIVATE DRAKE_COMPILE_IGNORE_DEPRECATED)
elseif(DRAKE_COMPILE_DEPRECATION_IS_ERROR)
  target_compile_definitions(deprecated_example
    PRIVATE DRAKE_COMPILE_DEPRECATION_IS_ERROR)
endif()
