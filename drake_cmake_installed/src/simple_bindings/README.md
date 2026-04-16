# Simple Python Bindings

Provides an example of creating a simple Drake C++ system and binding it in
`pybind11`, to be used with `pydrake`.

## How do I build it?

Follow the instructions in the [README](../../README.md) for this project.

## How do I run it?

```bash
cd build
# Run all tests belonging to this subdirectory.
ctest --test-dir src/simple_bindings
# Run all tests matching "simple_bindings".
ctest -R simple_bindings
```

Note that there is no interaction nor output from this program. It is merely
intended to exercise the code.
