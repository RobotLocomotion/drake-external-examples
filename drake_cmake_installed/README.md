# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Instructions

These instructions are only supported for Ubuntu 16.04 (Xenial).

```
# Install PreRequisites
<TODO: install pre-requisite debs list>

# Install Drake to /opt/drake
# curl -O https://s3.amazonaws.com/drake-packages/drake/nightly/drake-20170719-xenial.tar.gz
# Alternatively, the latest (usually last night's build)
curl -O https://s3.amazonaws.com/drake-packages/drake/nightly/drake-latest-xenial.tar.gz
sudo tar -xvzf drake-latest-xenial.tar.gz -C /opt

# Install GTest
sudo apt-get install libgtest-dev
cd /usr/src/gtest
sudo mkdir build && cd build && sudo cmake .. && sudo make
sudo cp *.a /usr/lib

# Build Everything
mkdir build && cd build
cmake ..
make -j5

# Run a demo
/opt/drake/bin/drake_visualizer &
(cd src/particles && exec ./uniformly_accelerated_particle_demo)         

# Run Tests
make test
```

# Examples

Further instructions can be found in the README files in the respective application subdirectories.

* [Simple Continuous Time System](src/simple_continuous_time_system/README.md)
* [Particle Demo](src/particles/README.md)

# Deploying a Project on CI

## Travis CI

## Circle CI
