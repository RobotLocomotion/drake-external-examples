# CMake Project with Drake as an External

This pulls in Drake using the CMake `ExternalProject_Add(drake)` mechanism.

## Instructions

First, install the required Ubuntu packages:

```
sudo setup/install_prereqs
```

This script will first download the drake source and then call drake's source 
`install_prereqs` script to install any required packages. It is expected that
the script will have been run prior to building with CMake as described below.

Within the top-level CMakeLists, the drake source is once again downloaded as
an external project. Keep in mind that if a specific commit of drake is used 
rather than master, and any dependencies have changed within that commit, then
the appropriate `install_prereqs` script will need to be run.

To build and run tests using CMake:

```
mkdir build
cd build
cmake ..
cmake --build .
ctest -V .
```
