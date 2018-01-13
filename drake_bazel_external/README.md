# Bazel Project with Drake as an External

This pulls in Drake via the Bazel workspace mechanism.

For an introduction to Bazel, refer to https://bazel.build/.

## Instructions

First, install the required Ubuntu packages:

```
sudo ../scripts/setup/linux/ubuntu/xenial/install_prereqs
```

Then, to build and test all apps:
```
bazel test //...
```
