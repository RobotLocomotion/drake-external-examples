# Bazel Project with Drake as a Precompiled External

This pulls in a downloaded or installed binary build of Drake via the Bazel
workspace mechanism.

For an introduction to Bazel, refer to
[Getting Started with Bazel](https://bazel.build/start).

## Instructions

First, run the `install_prereqs` script to download the
Drake source to `$HOME/drake/`. This also runs Drake's
setup script to install the required Ubuntu packages:

```bash
setup/install_prereqs
```

Additionally, if you don't already have bazel or bazelisk installed, then install bazelisk:

```bash
setup/install_bazelisk
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
cause differences. This is important when using tools like
`drake::FindResource` / `pydrake.common.FindResource`.
You may generally want to stick to using `bazel run` when able.
