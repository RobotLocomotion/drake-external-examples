# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.10)
project(drake_ament_cmake_installed)

set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake CONFIG REQUIRED)
find_package(drake CONFIG REQUIRED PATHS /opt/drake)

add_library(${PROJECT_NAME}
  include/${PROJECT_NAME}/particle.h
  src/particle.cc
)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} drake::drake)

install(DIRECTORY include/ DESTINATION include)

install(FILES LICENSE
  DESTINATION share/doc/${PROJECT_NAME}
)

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

if(BUILD_TESTING)
  find_package(ament_cmake_gtest CONFIG REQUIRED)
  find_package(ament_lint_auto CONFIG REQUIRED)

  ament_add_gtest(${PROJECT_NAME}_test test/particle_test.cc)
  target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})

  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_export_interfaces(export_${PROJECT_NAME})
ament_export_libraries(${PROJECT_NAME})

# This is required to make the package findable by downstream packages.
ament_export_dependencies(drake)

ament_package()
