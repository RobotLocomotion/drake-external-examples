# Drake External Examples

Examples of how to use [Drake](https://github.com/RobotLocomotion/drake) in your
own project:

* [`drake_bazel_external`](./drake_bazel_external)
* [`drake_cmake_external`](./drake_cmake_external)
* [`drake_cmake_installed`](./drake_cmake_installed)

## Continuous Integration

Scripts are provided for various CI instances in `scripts/continuous_integration`. The intended purpose of each is described below:

* `circle_ci` : exemplifies how to put a project depending on a Drake installation on Circle CI
* `jenkins` : provides complete coverage of all example projects
* `travis_ci` : examplifies how to put a project depending on a Drake installation on Travis CI

| **Subproject** | **Circle CI** | **Jenkins** | **Travis CI** |
|:---:|:---:|:---:|:---:|
| `drake_bazel_external` | - | o | - |
| `drake_cmake_external` | - | o | - |
| `drake_cmake_installed` | o | o | o |
||[![CircleCI](https://img.shields.io/circleci/project/github/RobotLocomotion/drake-shambhala/master.svg)](https://circleci.com/gh/RobotLocomotion/drake-shambhala)|[![Jenkins](https://img.shields.io/jenkins/s/https/drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-shambhala/job/master.svg)](https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-shambhala/)|[![Travis CI](https://img.shields.io/travis/com/RobotLocomotion/drake-shambhala/master.svg)](https://travis-ci.com/RobotLocomotion/drake-shambhala)

Note, the Circle CI and Travis CI jobs only build and test `drake_cmake_installed`
since it is the exemplary case for a lightweight, open-source build on a public
CI server.
