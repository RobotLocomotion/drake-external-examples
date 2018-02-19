# PCL Examples

These examples are designed to illustrate and exercise build and execution
of a project that depends on both Drake and PCL.

This has been tested and is currently officially supported on Ubuntu
Xenial (16.04).

## Drake-PCL Caveats

### Eigen

Drake's version of Eigen is compatible with PCL's (system) but not vice
versa. Make sure you discover them in the correct order.

```
find_package(drake CONFIG REQUIRED)
find_package(pcl CONFIG REQUIRED)
```

so that a version of Eigen that is compatible to both projects is discovered
and used. Note that `CONFIG` and `REQUIRED` are merely good practice if you
are looking for the installed cmake modules). If you do not do this, then you
will receive a static assertion failure from Drake:

```
Drake requires Eigen >= v3.3.3.
static_assert(EIGEN_VERSION_AT_LEAST(3, 3, 3),
```

### VTK

The system built version of PCL with VTK6 has a dependency issue (this is strictly a
PCL/VTK6 problem). Either exclude vtkproj4 or use the version of VTK6 shipped by ROS
which includes a bugfix.

```
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
 ```

## Simple PCL Example

### How do I build it?

Follow the instructions in the [README](../../README.md) for this project.

### How do I run it?

```
# Switch to the build directory
cd build/src/pcl
# Run the example
./simple_pcl_example
```

### Gory Details

In particular, the problems that are exercised by this example include:

* Use of different versions of Eigen by both Drake and PCL
* Use of different versions of VTK

The Eigen issue is resolved simply by ensuring that Drake's bundled Eigen is
discovered first as noted earlier.

VTK itself has been extremely well engineered so that different versions of
VTK can co-exist within the same project. On Ubuntu Xenial, Drake uses a very
recent version of VTK (8) whilst PCL uses the system version (6.2). The only
software engineering employed by Drake is to hide the VTK header trail so
header conflicts are not transferred to downstream projects.

