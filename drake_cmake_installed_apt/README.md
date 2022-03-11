# CMake Project with Drake Installed Using APT

This example uses the [`cmake`](https://cmake.org/) build system with an
instance of Drake installed using the
[APT](https://manpages.ubuntu.com/manpages/focal/man8/apt.8.html) package
manager.

## Instructions

Install the `drake-dev` APT package by following the instructions found at:

<https://drake.mit.edu/from_binary.html#apt-packages-for-monthly-tagged-releases>

For this example, also install the `build-essential` and `cmake` APT packages:
```bash
sudo apt-get update
sudo apt-get --no-install-recommends install build-essential cmake 
```

To build the `drake_cmake_installed_apt` example:
```bash
mkdir build
cd build
cmake ..
make
```

To run the `drake_cmake_installed_apt` tests:
```bash
cd build
ctest .
```
