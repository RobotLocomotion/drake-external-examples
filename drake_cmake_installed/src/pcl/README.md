# PCL Examples

These examples are designed to illustrate and exercise build and execution of a project that depends
on both Drake and PCL. 

This has been tested and is currently officially supported on Ubuntu Xenial (16.04).

### How do I build it?

Follow the instructions in the [README](../../README.md) for this project.

### How do I run it?

```
# Switch to the build directory
cd build/src/pcl
# Run the example
./simple_pcl_example
```

## Gory Details

In particular, known problems that are exercised include:

* Use of different (patch) versions of Eigen by both Drake and PCL
* Use of different (major) versions of VTK

The Eigen issue is resolves simply by ensuring that Drake's bundled Eigen is discovered first.

VTK itself has been extremely well engineered so that different versions of VTK can co-exist
within the same project. The only engineering constraint on Drake is that it hides the VTK header
trail so header conflicts are not transferred to downstream projects. Note that PCL uses system VTK.

