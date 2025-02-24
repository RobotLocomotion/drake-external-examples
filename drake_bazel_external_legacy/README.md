# Bazel Project with Drake as an External (legacy style)

This pulls in Drake and builds it from source via Bazel's WORKSPACE mechanism,
which is deprecated as of Bazel 8 and will be removed in Bazel 9. Therefore,
we do not recommend using this example for new projects; instead, refer to
the `drake_bazel_external` example. The legacy example primarily serves
as a regression test for the deprecated behavior, and therefore uses Drake's
minimum supported Bazel version.

For an introduction to Bazel, refer to
[Getting Started with Bazel](https://bazel.build/start).

## Instructions

First, run the `install_prereqs` script to download the
Drake source to `drake/` (from the current directory).
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

## Python Versions

By default, Python 3 is the Python interpreter that Drake will use when built
with Bazel. To see which Python versions are supported, see the
[supported configurations](https://drake.mit.edu/developers.html#supported-configurations).
