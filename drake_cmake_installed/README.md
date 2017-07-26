# CMake Project with an Installed Drake

This uses the CMake `find_package(drake)` mechanism to find an installed instance of Drake.

# Locally on Xenial

```
curl -O https://s3.amazonaws.com/drake-packages/drake/nightly/drake-20170719-xenial.tar.gz
sudo tar -xvzf drake-20170719-xenial.tar.gz -C /opt
mkdir build
cd build
cmake ..
make
```

# On Travis CI

# On Circle CI


**TODO**
