# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 16.04 (Xenial).

```
###############################################################
# Install Prerequisites
###############################################################
# Various system dependencies
sudo ../scripts/setup/linux/ubuntu/xenial/install_prereqs

# (Optionally) Install GTest
# You could also explicitly pull gtest into the CMake build directly:
#     https://github.com/google/googletest/tree/master/googletest
sudo apt-get install libgtest-dev
ls
mkdir ~/gtest && cd ~/gtest && cmake /usr/src/gtest && make
sudo cp *.a /usr/local/lib

###############################################################
# Install Drake to /opt/drake
###############################################################

# 1) A specific version (date-stamped)
# curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-20170719-xenial.tar.gz

# 2) The latest (usually last night's build)
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-xenial.tar.gz
sudo tar -xvzf drake-latest-xenial.tar.gz -C /opt

# 3) Manual Installation
# git clone https://github.com/RobotLocomotion/drake.git
# (cd drake && bazel run //:install /opt/drake)         

# 4) Manual Installation w/ Licensed Gurobi
# Install & setup gurobi (http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu)
# git clone https://github.com/RobotLocomotion/drake.git
# (cd drake && bazel run --config gurobi //:install /opt/drake)         

###############################################################
# Build Everything
###############################################################
git clone https://github.com/RobotLocomotion/drake-shambhala.git
cd drake-shambhala/drake_cmake_installed
mkdir build && cd build
cmake -Ddrake_DIR=/opt/drake/lib/cmake/drake ..
make

###############################################################
# Execute
###############################################################
# A demo
/opt/drake/bin/drake_visualizer &
(cd src/particles && exec ./uniformly_accelerated_particle_demo)         

# (Optionally) Run Tests
make test
```

# Examples

Further instructions can be found in the README files in the respective application subdirectories.

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle Demo](src/particles/README.md)
* [Find Resources](src/find_resource/README.md)
