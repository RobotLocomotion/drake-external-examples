# Catkin Project with an Installed Drake

This example uses the [`catkin`](https://wiki.ros.org/catkin) build system, as
used by the Robot Operating System (ROS), with an installed instance of the
Drake [binary packages](https://drake.mit.edu/from_binary.html).

## Instructions

If you wish use `catkin` from the Ubuntu package archive, without ROS Noetic,
install the required packages as follows:
```
sudo ../scripts/setup/linux/ubuntu/focal/install_prereqs
```

If you wish use `catkin` from ROS Noetic package archive, install the required
packages and configure your environment as follows:
```
sudo ../scripts/setup/linux/ubuntu/focal/install_prereqs --ros-noetic
source /opt/ros/noetic/setup.bash
```

To build the `drake_catkin_installed` library:
```
catkin_make --cmake-args "-DCMAKE_PREFIX_PATH=/path/to/drake;$CMAKE_PREFIX_PATH"
```

To build and run the `drake_catkin_installed_test` test executable:
```
catkin_make run_tests --cmake-args "-DCMAKE_PREFIX_PATH=/path/to/drake;$CMAKE_PREFIX_PATH"
```

*If the Drake binary package is installed to `/opt/drake`, you may omit the
`--cmake-args <args>` argument.*
