cmake_minimum_required(VERSION 3.5)
project(drake_example_pendulum_description)

find_package(ament_cmake REQUIRED)

install(DIRECTORY urdf meshes
  DESTINATION share/${PROJECT_NAME})

ament_package()
