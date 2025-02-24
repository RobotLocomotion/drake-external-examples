# Bazel Project with Drake as an External

This pulls in Drake and builds it from source via Bazel's module mechanism.

For an introduction to Bazel, refer to
[Getting Started with Bazel](https://bazel.build/start).

## Instructions

First, run the `install_prereqs` script to download
the Drake source to `drake/` (from the current directory).
This also runs Drake's setup script to install the required Ubuntu packages:

```bash
setup/install_prereqs
```

Then, to build and test all apps:

```bash
bazel test //...
```

As an example to run a binary directly:

```bash
bazel run //apps:simple_logging_example
```

You may also run the binary directly per the `bazel-bin/...` path that the
above command prints out; however, be aware that your working directories may
cause differences.  This is important when using tools like
`drake::FindResource` / `pydrake.common.FindResource`.
You may generally want to stick to using `bazel run` when able.

### Using a local checkout of Drake

To use Drake sources on disk instead of downloaded from github, pass the flag
``--override_module=drake=/home/user/stuff/drake`` to bazel on the command line
or add a line such as the following to ``user.bazelrc`` in the current directory:

```bash
build --override_module=drake=/home/user/stuff/drake
```

## Python Versions

By default, Python 3 is the Python interpreter that Drake will use when built
with Bazel. To see which Python versions are supported, see the
[supported configurations](https://drake.mit.edu/developers.html#supported-configurations).
