# Source Examples

## Available Examples

* [Find Resources](find_resource/): Finds and loads resources that are part of
  the Drake install.
* [Particle System](particle/) and
  [Simple Continuous Time System](simple_continuous_time_system/): The
  "hello world" examples for the `drake::systems` classes.
* [Simple Bindings](simple_bindings/): Creates a simple Drake C++ system and
  binds it in `pybind11`, to be used with `pydrake`.

##  How do I run it?

First, build the project according to the instructions in the top-level
[README](../README.md) for this project.

Use the following to run the tests, replacing `particle` with the given
example/subdirectory.

```bash
cd build
# Run all tests belonging to this subdirectory.
ctest --test-dir src/particle
# Run all tests matching "particle".
ctest -R particle
```

Note that there is no interaction nor output from these programs. They are
merely intended to exercise the code.
