# CMake Project with Drake Installed Using APT

This example uses the [`cmake`](https://cmake.org/) build system with an
instance of Drake installed using the
[APT](https://manpages.ubuntu.com/manpages/noble/man8/apt.8.html) package
manager.

## Instructions

First, run the `install_prereqs` script to download and install the
`drake-dev.deb` package from the latest
[release](https://github.com/RobotLocomotion/drake/releases). Most content
installs to `/opt/drake`. This script also installs the necessary packages for
this example.

```bash
setup/install_prereqs
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
