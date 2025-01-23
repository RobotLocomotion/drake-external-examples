# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 22.04 (Jammy).

```bash
###############################################################
# Download Drake and Install Prerequisites
###############################################################

# Download Drake source to $HOME/drake/ and
# install various system dependencies
setup/install_prereqs

###############################################################
# Alternative Drake Versions
###############################################################

# The script above gets the latest version of Drake (usually
# last night's build). Ignore this step if that version is desired.
# Otherwise, the following are alternative options for which
# version of Drake to download:

# 1) A specific version (date-stamped)
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-20240214-jammy.tar.gz
tar -xvzf drake-20240214-jammy.tar.gz -C $HOME

# 2) Manual Installation
git clone https://github.com/RobotLocomotion/drake.git
(cd drake && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake .. && make)

# 3) Manual Installation w/ Licensed Gurobi
# Install & setup gurobi (http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu)
git clone https://github.com/RobotLocomotion/drake.git
(cd drake && mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake -DWITH_GUROBI=ON .. && make)

###############################################################
# Build Everything
###############################################################

git clone https://github.com/RobotLocomotion/drake-external-examples.git
cd drake-external-examples/drake_cmake_installed
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=$HOME/drake ..
make

# (Optionally) Run Tests
make test
```

# Examples

Drake-specific examples:

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle System](src/particle)
* [Find Resources](src/find_resource/README.md)

# Developer Testing

If you are a Drake Developer making build or API changes that may affect the
downstream interface, please test this locally on your system.

These build instructions are adapted from those above, but will use an existing
source tree of Drake (but *not* installing it to `$HOME/drake`),
build this project, and then run all available tests:

```bash
# Build development version of Drake, ensuring no old artifacts are present.
cd drake  # Where you are developing.
rm -rf build && mkdir build && cd build
cmake .. # Configure Gurobi, Mosek, etc, if needed.
# Build locally.
make
# Record the build's install directory.
drake_install=${PWD}/install

# Build this project using a development version of Drake.
cd ..
# Clone this repository if you have not already.
git clone https://github.com/RobotLocomotion/drake-external-examples.git
cd drake-external-examples/drake_cmake_installed
# Follow "Install Prerequisites" in the instructions linked above if you
# have not already.
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=${drake_install} ..
make
ctest
```
