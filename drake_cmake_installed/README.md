# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 16.04 (Xenial).

```
# Install Prerequisites
<TODO: install pre-requisite debs list>

# Install Drake to /opt/drake
# curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-20170719-xenial.tar.gz
# Alternatively, the latest (usually last night's build)
curl -O https://drake-packages.csail.mit.edu/drake/nightly/drake-latest-xenial.tar.gz
sudo tar -xvzf drake-latest-xenial.tar.gz -C /opt

# (Optionally) Install GTest
# You could also explicitly pull gtest into the CMake build directly:
#     https://github.com/google/googletest/tree/master/googletest
sudo apt-get install libgtest-dev
ls
mkdir ~/gtest && cd ~/gtest && cmake /usr/src/gtest && make
sudo cp *.a /usr/local/lib

# Clone & Build Everything
git clone https://github.com/RobotLocomotion/drake-shambhala.git
mkdir build && cd build
cmake ..
make

# Run a demo
/opt/drake/bin/drake_visualizer &
(cd src/particles && exec ./uniformly_accelerated_particle_demo)         

# (Optionally) Run Tests
make test
```

# Examples

Further instructions can be found in the README files in the respective application subdirectories.

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle Demo](src/particles/README.md)

# Deploying a Project on CI

## Travis CI

## Circle CI
