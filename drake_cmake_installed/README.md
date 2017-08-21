# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Locally on Xenial

Download the binaries and install into /opt. This will ensure that CMake is able
to auto-discover the `drake-config.cmake` module. 

```
# A nightly build
# curl -O https://s3.amazonaws.com/drake-packages/drake/nightly/drake-20170719-xenial.tar.gz
# Alternatively, the latest (usually last night's build)
curl -O https://s3.amazonaws.com/drake-packages/drake/nightly/drake-latest-xenial.tar.gz
sudo tar -xvzf drake-latest-xenial.tar.gz -C /opt
mkdir build && cd build
cmake ..
make
```

# On Travis CI

# On Circle CI

# Examples

* Simple Continuous Time System
* Particle Demo
