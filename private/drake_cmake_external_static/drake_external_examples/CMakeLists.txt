# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.16)
project(drake_external_examples)

find_package(drake CONFIG REQUIRED)

add_executable(simple_continuous_time_system simple_continuous_time_system.cc)
target_link_libraries(simple_continuous_time_system drake::drake)

include(CTest)

add_test(NAME simple_continuous_time_system
  COMMAND simple_continuous_time_system
)
