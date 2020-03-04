# Drake External Examples

Examples of how to use [Drake](https://github.com/RobotLocomotion/drake) in your
own project:

* [`drake_bazel_external`](./drake_bazel_external)
* [`drake_bazel_installed`](./drake_bazel_installed)
* [`drake_cmake_external`](./drake_cmake_external)
* [`drake_cmake_installed`](./drake_cmake_installed)

## Continuous Integration

Scripts are provided for various CI instances in `scripts/continuous_integration`. The intended purpose of each is described below:

* `circle_ci` : exemplifies how to put a project depending on a Drake installation on Circle CI
* `jenkins` : provides complete coverage of all example projects

| **Subproject** | **Circle CI** | **Jenkins** |
|:---:|:---:|:---:|
| `drake_bazel_external` | - | o |
| `drake_bazel_installed` | o | o |
| `drake_cmake_external` | - | o |
| `drake_cmake_installed` | o | o | 
|| [![CircleCI](https://img.shields.io/circleci/project/github/RobotLocomotion/drake-external-examples/master.svg)](https://circleci.com/gh/RobotLocomotion/drake-external-examples) | [![Jenkins](https://img.shields.io/jenkins/s/https/drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/job/master.svg)](https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/) |

Note, the Circle CI jobs only build and test `drake_bazel_installed` and
`drake_cmake_installed` since these are the exemplary cases for lightweight,
open-source builds on public CI servers.
