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

By default, the `install_prereqs` script gets the latest version of Drake
(usually last night's build). Ignore this if that version is desired.
Otherwise, the following are alternative options for which version of Drake to
download.

To use any of these options, instead of running `install_prereqs`,
run the code for whichever option you prefer below. Each option
installs Drake into `/opt/drake`.

1. A date-stamped version

Replace `20260401` with the desired date.

```bash
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-dev_0.0.20260401-1_amd64-noble.deb
sudo apt-get install --no-install-recommends ./drake-dev_0.0.20260401-1_amd64-noble.deb
```

See [Apt Nightly Releases](https://drake.mit.edu/apt.html#nightly-releases)
for more information.

2. A stable release

Stable release packages are available as GitHub release attachments. Replace
`1.51.1` below with the desired release, being sure to use `-L` to follow
redirects properly:

```bash
curl -LO https://github.com/RobotLocomotion/drake/releases/download/v1.51.1/drake-dev_1.51.1-1_amd64-noble.deb
sudo apt-get install --no-install-recommends ./drake-dev_1.51.1-1_amd64-noble.deb
```

See [Apt Stable Releases](https://drake.mit.edu/apt.html#stable-releases)
for more information.
