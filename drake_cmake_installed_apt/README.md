# CMake Project with Drake Installed Using APT

This example uses the [`cmake`](https://cmake.org/) build system with an
instance of Drake installed using the
[APT](https://manpages.ubuntu.com/manpages/noble/man8/apt.8.html) package
manager.

# Instructions

## Download and Install Prerequisites

First, run the `install_prereqs` script to download and install the
`drake-dev.deb` package from the latest
[release](https://github.com/RobotLocomotion/drake/releases). Most content
installs to `/opt/drake`. This script also installs the necessary packages for
this example.

```bash
setup/install_prereqs
```

See [below](#alternative-versions) for alternative versions of
Drake to download.

## Build Everything

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

## Alternative Versions

By default, `install_prereqs` script gets the latest version
of Drake (usually last night's build). Ignore this if that
version is desired. Otherwise, the following are alternative
options for which version of Drake to download.

To use any of these options, instead of running `install_prereqs`,
run the code for whichever option you prefer below. Each option
installs Drake into `/opt/drake`.

1. A specific version (date-stamped)

```bash
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-dev_0.0.20250301-1_amd64-noble.deb
sudo apt-get install --no-install-recommends ./drake-dev_0.0.20250301-1_amd64-noble.deb
```

See [Apt Nightly Releases](https://drake.mit.edu/apt.html#nightly-releases)
for more information.

2. Manual installation

```bash
git clone https://github.com/RobotLocomotion/drake.git
cd drake
setup/install_prereqs
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=./drake .. && make install
cmake -E tar czf drake-noble.tar.gz drake
cd .. && mkdir drake_deb
bazel run //tools/release_engineering:repack_deb -- --tgz drake-noble.tar.gz --output-dir drake_deb
sudo apt-get install --no-install-recommends drake_deb/drake-dev_<version>-1_amd64.deb
```

Note that `<version>` is determined based on the contents of the
`drake/share/doc/drake/VERSION.TXT` file.

3. Manual installation w/ licensed Gurobi

Install and setup Gurobi:
<http://drake.mit.edu/bazel.html?highlight=gurobi#install-on-ubuntu>.

Follow the same instructions as the previous example (2), but additionally
configure CMake with `-DWITH_GUROBI=ON`. See
<https://drake.mit.edu/from_source.html> for a more detailed list of CMake
options when configuring Drake's build.

Finally, for the code in this example, ensure you have `python3-dev`
installed if on Ubuntu:

```bash
sudo apt-get update
sudo apt-get --no-install-recommends install python3-dev
```
