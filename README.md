# Drake External Examples

Examples of how to use [Drake](https://github.com/RobotLocomotion/drake) in your
own project:

* [`drake_ament_cmake_installed`](./drake_ament_cmake_installed)
* [`drake_bazel_external`](./drake_bazel_external)
* [`drake_bazel_installed`](./drake_bazel_installed)
* [`drake_catkin_installed`](./drake_catkin_installed)
* [`drake_cmake_external`](./drake_cmake_external)
* [`drake_cmake_installed`](./drake_cmake_installed)
* [`drake_cmake_installed_apt`](./drake_cmake_installed_apt)

## Continuous Integration

Scripts are provided for various CI instances in `scripts/continuous_integration`. The intended purpose of each is described below:

* `github_actions`:  exemplifies how to put a project depending on a Drake installation on GitHub Actions
* `jenkins` : provides complete coverage of additional example projects

| **Subproject** | **GitHub Actions** | **Jenkins** |
|:---:|:---:|:---:|
| `drake_ament_cmake_installed` | o | - |
| `drake_bazel_external` | - | o |
| `drake_bazel_installed` | o | o |
| `drake_catkin_installed` | o | o |
| `drake_cmake_external` | - | o |
| `drake_cmake_installed` | o | o |
| `drake_cmake_installed_apt` | o | - |
|| ![GitHub Actions](https://img.shields.io/github/workflow/status/RobotLocomotion/drake-external-examples/ci/main) | [![Jenkins](https://img.shields.io/jenkins/build.svg?jobUrl=https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/job/main)](https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/) |

Note, the GitHub Actions jobs only build and test `drake_ament_cmake_installed`,
`drake_bazel_installed`, `drake_catkin_installed`, `drake_cmake_installed`, and
`drake_cmake_installed_apt` since these are the exemplary cases for lightweight,
open-source builds on public CI servers. Not all example projects are supported
on macOS.
