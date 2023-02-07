# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 20.04 (Focal).

```shell
###############################################################
# Install Prerequisites
###############################################################
# Various system dependencies
sudo ../scripts/setup/linux/ubuntu/focal/install_prereqs

# (Optionally) Install GTest
# You could also explicitly pull gtest into the CMake build directly:
#     https://github.com/google/googletest/tree/master/googletest
sudo apt-get install libgtest-dev
ls
mkdir ~/gtest && cd ~/gtest && cmake /usr/src/gtest && make
sudo cp *.a /usr/local/lib

###############################################################
# Install Drake to $HOME/drake
###############################################################

# 1) A specific version (date-stamped)
# curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-20220309-focal.tar.gz

# 2) The latest (usually last night's build)
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-focal.tar.gz
tar -xvzf drake-latest-focal.tar.gz -C $HOME

# 3) Manual Installation
# git clone https://github.com/RobotLocomotion/drake.git
# (mkdir drake-build && cd drake-build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake ../drake && make)

# 4) Manual Installation w/ Licensed Gurobi
# Install & setup gurobi (http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu)
# git clone https://github.com/RobotLocomotion/drake.git
# (mkdir drake-build && cd drake-build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake -DWITH_GUROBI=ON ../drake && make)

###############################################################
# Build Everything
###############################################################
git clone https://github.com/RobotLocomotion/drake-external-examples.git
cd drake-external-examples
mkdir drake_cmake_installed-build && cd drake_cmake_installed-build
cmake -DCMAKE_PREFIX_PATH=$HOME/drake ../drake_cmake_installed
make

###############################################################
# Execute
###############################################################
# A demo
$HOME/drake/bin/drake-visualizer &
(cd src/particles && exec ./uniformly_accelerated_particle)

# (Optionally) Run Tests
make test
```

# Examples

Drake specific Examples:

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle Demo](src/particles/README.md)
* [Find Resources](src/find_resource/README.md)

# Developer Testing

If you are a Drake Developer making build or API changes that may affect the
downstream interface, please test this locally on your system.

These build instructions are adapted from those above, but will use an existing
source tree of Drake (but *not* installing it to `$HOME/drake`),
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

# Build this project using a development version of Drake.
cd ..
# Clone this repository if you have not already.
git clone https://github.com/RobotLocomotion/drake-external-examples.git
cd drake-external-examples
# Follow "Install Prerequisites" in the instructions linked above if you
# have not already.
mkdir drake_cmake_installed-build && cd drake_cmake_installed-build
cmake -DCMAKE_PREFIX_PATH=${drake_install} ../drake_cmake_installed
make
ctest
```
