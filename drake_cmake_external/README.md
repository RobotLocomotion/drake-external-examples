# CMake Project with Drake as an External

This pulls in Drake using the CMake `ExternalProject_Add(drake)` mechanism.

## Instructions

First, run the `install_prereqs` script to download the Drake source to `drake-master/`
(from the current directory). This also runs Drake's setup script to
install the required Ubuntu packages:

```bash
setup/install_prereqs
```

Keep in mind that within the top-level CMakeLists, the drake source is once
again downloaded as an external project. If the CMakeLists is modified to use
a specific version of drake other than master, and any dependencies have
changed within that version, then the script ran above must also be modified.

Once all necessary dependencies have been installed, build and run tests
using CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .

cd drake_external_examples
ctest -V .
```
