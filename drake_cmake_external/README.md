# CMake Project with Drake as an External

This pulls in Drake using the CMake `ExternalProject_Add(drake)` mechanism.

## Instructions

First, run the `install_prereqs` script to download the
Drake source to `drake/` (from the current directory).
This also runs Drake's setup script to install the required Ubuntu packages:

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

### Using a specific commit of Drake

To use Drake sources from a specific commit, pass two cache variables to
CMake (from either the CLI or the GUI):

* `DRAKE_COMMIT_HASH`: the commit hash
* `DRAKE_COMMIT_SHA256`: the checksum of the archive downloaded from GitHub
(download https://github.com/RobotLocomotion/drake/archive/<DRAKE_COMMIT_HASH>.tar.gz
and use `shasum -a 256 'xxx.tar.gz'`)
