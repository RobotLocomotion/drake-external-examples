# CMake Project with Drake as an External

This pulls in Drake using the CMake `ExternalProject_Add(drake)` mechanism.

## Instructions

First, download the drake source to a location of your choice and run the
source setup script to install the required Ubuntu packages:

```
mkdir ~/drake_install
wget -O drake.tar.gz \
  https://github.com/RobotLocomotion/drake/archive/master.tar.gz
tar -xf drake.tar.gz -C ~/drake_install
sudo ~/drake_install/drake-master/setup/ubuntu/install_prereqs.sh
```

Keep in mind that within the top-level CMakeLists, the drake source is once
again downloaded as an external project. If the CMakeLists is modified to use
a specific version of drake other than master, and any dependencies have
changed within that version, then the same version must also be used above.

Once all necessary dependencies have been installed, build and run tests
using CMake:

```
mkdir build
cd build
cmake ..
cmake --build .

cd drake_external_examples
ctest -V .
```
