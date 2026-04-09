# Find Resources

Simply tests the Drake find resource API from outside of Drake, i.e., finds and
loads resources that are part of the Drake install.

## How do I build it?

Follow the instructions in the [README](../../README.md) for this project.

## How do I run it?

```bash
cd build
# Run the sample program directly.
./src/find_resource/find_resource_example
# Run all tests belonging to this subdirectory.
ctest --test-dir src/find_resource
# Run all tests matching "find_resource".
ctest -R find_resource
```

Note that there is no interaction nor output from this program. It is merely
intended to exercise the code.
