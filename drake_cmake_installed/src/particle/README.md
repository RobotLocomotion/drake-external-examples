# Particle

This is the hello world for the `drake::systems` classes.

## How do I build it?

Follow the instructions in the [README](../../README.md) for this project.

## How do I run it?

```bash
cd build
# Run all tests belonging to this subdirectory.
ctest --test-dir src/particle
# Run all tests matching "particle".
ctest -R particle
```

Note that there is no interaction nor output from this program. It is merely
intended to exercise the code.
