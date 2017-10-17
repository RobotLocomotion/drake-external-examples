# Bazel Project with Drake as an External

This pulls in drake via the Bazel external mechanism.

## UnSupported (Yet)

While it is possible to replicate Drake's WORKSPACE and bazel macros
to reconstruct the build, the process is brittle and subject
to breakage every time the build logic changes, especially with
respect to shifts in Drake dependencies.

Nonetheless, it is an option we need to support for our own purposes,
so until Bazel has a convenient means of doing so, there is a plan
to provide build logic to support this ourselves. This is being
tracked in
[drake#7259](https://github.com/RobotLocomotion/drake/issues/7259).


