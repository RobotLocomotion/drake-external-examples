# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 16.04 (Xenial).

```shell
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
# curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-20171015-xenial.tar.gz

# 2) The latest (usually last night's build)
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-xenial.tar.gz
sudo tar -xvzf drake-latest-xenial.tar.gz -C /opt

# 3) Manual Installation
# git clone https://github.com/RobotLocomotion/drake.git
# (mkdir drake-build && cd drake-build && cmake -DCMAKE_INSTALL_PREFIX=/opt/drake ../drake && make)

# 4) Manual Installation w/ Licensed Gurobi
# Install & setup gurobi (http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu)
# git clone https://github.com/RobotLocomotion/drake.git
# (mkdir drake-build && cd drake-build && cmake -DCMAKE_INSTALL_PREFIX=/opt/drake -DWITH_GUROBI=ON ../drake && make)

###############################################################
# Build Everything
###############################################################
git clone https://github.com/RobotLocomotion/drake-shambhala.git
cd drake-shambhala
mkdir drake_cmake_installed-build && cd drake_cmake_installed-build
cmake -Ddrake_DIR=/opt/drake/lib/cmake/drake ../drake_cmake_installed
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

Drake specific Examples:

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle Demo](src/particles/README.md)
* [Find Resources](src/find_resource/README.md)

Compatibility Examples:

* [PCL](src/pcl/README.md)

# Developer Testing

If you are a Drake Developer making build or API changes that may affect the
downstream interface, please test this locally on your system.

These build instructions are adapted from those above, but will use an existing
source tree of Drake (but *not* installing it to `/opt/drake`),
build this project, and then run all available tests:

```shell
# Build development version of Drake, ensuring no old artifacts are present.
cd drake  # Where you are developing.
rm -rf ../drake-build && mkdir ../drake-build && cd ../drake-build
cmake ../drake  # Configure Gurobi, Mosek, etc, if needed.
# Build locally.
make
# Record the build's install directory.
drake_install=${PWD}/install

# Build Drake Shambhala using development version of Drake.
cd ..
# Clone `drake-shambhala` if you have not already.
git clone https://github.com/RobotLocomotion/drake-shambhala
cd drake-shambhala
# Follow "Install Prerequisites" in the instructions linked above if you
# have not already.
mkdir drake_cmake_installed-build && cd drake_cmake_installed-build
cmake -Ddrake_DIR=${drake_install}/lib/cmake/drake ../drake_cmake_installed
make
ctest
```
