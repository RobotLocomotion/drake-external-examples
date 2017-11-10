# PCL Examples

These examples are designed to illustrate and exercise build and execution of a project that depends
on both Drake and PCL. 

This has been tested and is currently officially supported on Ubuntu Xenial (16.04).

## How to Create a Drake-PCL Project?

To ensure Drake and PCL combine compatibly the only caveat is that you discover the projects in the following order:

```
find_package(drake CONFIG REQUIRED)
find_package(pcl CONFIG REQUIRED)
```

so that a version of Eigen that is compatible to both projects is discovered and used. Note that `CONFIG` and `REQUIRED` are
merely good practice if you are looking for the installed cmake modules).

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

The Eigen issue is resolved simply by ensuring that Drake's bundled Eigen is discovered first as noted earlier.

VTK itself has been extremely well engineered so that different versions of VTK can co-exist
within the same project. On Ubuntu Xenial, Drake uses a very recent version of VTK (8) whilst
PCL uses the system version (6.2). The only software engineering employed by Drake is to hide the VTK header
trail so header conflicts are not transferred to downstream projects.

