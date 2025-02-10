# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)`
mechanism to find an installed instance of Drake.

# Instructions

## Download and Install Prerequisites

First, run the `install_prereqs` script to download the
Drake source to `$HOME/drake/`. This also run's Drake's
setup script to install the required packages depending
on your operating system:

```bash
setup/install_prereqs
```

See [below](#alternative-versions) for alternative versions of
Drake to download.

## Build Everything

Then run the following to build using CMake from the current directory
(`drake-external-examples/drake_cmake_installed`):

```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=$HOME/drake ..
make
```

You can also optionally run the tests by calling `make test`.

## Alternative Versions

By default, `install_prereqs` script gets the latest version
of Drake (usually last night's build). Ignore this if that
version is desired. Otherwise, the following are alternative
options for which version of Drake to download.

To use any of these options, instead of running `install_prereqs`,
run the code for whichever option you prefer below. Each option
downloads Drake to the `$HOME` directory.

1. A specific version (date-stamped)

Replace `jammy` (for Ubuntu 22.04) with `noble` or `mac-arm64`
to download the version needed for your operating system.

```bash
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-0.0.20250131-jammy.tar.gz
tar -xvzf drake-0.0.20250131-jammy.tar.gz -C $HOME
$HOME/drake/share/drake/setup/install_prereqs
```

See [Installation via Direct Download](https://drake.mit.edu/from_binary.html)
for more information.

2. Manual installation

```bash
git clone https://github.com/RobotLocomotion/drake.git
cd drake
setup/install_prereqs
(mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake .. && make)
```

3. Manual installation w/ licensed Gurobi
Install & setup gurobi (http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu)

```bash
git clone https://github.com/RobotLocomotion/drake.git
cd drake
setup/install_prereqs
(mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=$HOME/drake -DWITH_GUROBI=ON .. && make)
```

Finally, for the code in this example, ensure you have `python3-all-dev`
installed if on Ubuntu.

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
