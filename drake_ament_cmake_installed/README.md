# Ament CMake Project with an Installed Drake

This example uses the [`ament_cmake`](https://index.ros.org/doc/ros2/Tutorials/Ament-CMake-Documentation/)
build system and [`colcon`](https://colcon.readthedocs.io) command line tool, as
used by the Robot Operating System (ROS) 2, with an installed instance of the
Drake [binary packages](https://drake.mit.edu/from_binary.html).

## Instructions

To use `ament_cmake` and `colcon` from the ROS 2 Humble package archive, install
the required packages and configure your environment as follows:
```
sudo setup/install_prereqs --ros-humble
source /opt/ros/humble/setup.bash
```

To build the `drake_ament_cmake_installed` library:
```
colcon build --cmake-args "-DCMAKE_PREFIX_PATH=/path/to/drake;$CMAKE_PREFIX_PATH"
```

*If the Drake binary package is installed to `/opt/drake`, you may omit the
`--cmake-args <args>` argument.*

To build and run the `drake_ament_cmake_installed` test executable:
```
colcon test
```
