# Bazel Project with Drake installed via APT

This uses the binary copy of Drake installed via APT into /opt/drake via
a Bazel workspace.

For an introduction to Bazel, refer to
[Getting Started with Bazel](https://docs.bazel.build/versions/master/getting-started.html).

## Instructions

First, install Drake via APT per https://drake.mit.edu/apt.html.

Then, to build and test all apps:
```
bazel test //...
```
