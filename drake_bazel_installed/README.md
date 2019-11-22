# Bazel Project with Drake as a precompiled External

This pulls in an installed copy of Drake via the Bazel workspace mechanism.

For an introduction to Bazel, refer to
[Getting Started with Bazel](https://docs.bazel.build/versions/master/getting-started.html).

## Instructions

First, install the required Ubuntu packages:

```
sudo ../scripts/setup/linux/ubuntu/bionic/install_prereqs
```

Then, to build and test all apps:
```
bazel test //...
```
