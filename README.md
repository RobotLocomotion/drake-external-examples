# Shambhala

Shambhala is a hidden kingdom (**Drake**) where Buddhist values and traditions rule. The
utopian realm is also home to the Great Warrior Gesar (**Russ Tedrake**), who leads hordes of the
righteous (**Drake Developers**) who will eventually ride into the human world to combat our demons (**intractable problems**).

More practically, this repository exemplifies various ways to open a pathway to using [Drake](https://github.com/RobotLocomotion/drake) in your own project.

# Continuous Integration

Scripts are provided for various CI instances in `scripts/continuous_integration`. The intended purpose of each is described below:

* `circle_ci` : exemplifies how to put a project depending on a Drake installation on Circle CI
* `jenkins` : provides complete coverage of shambhala for all subprojects and officially supported platforms

| **Subproject** | **Circle CI** | **Jenkins** |
|:---:|:---:|:---:|
| `drake_bazel_external` | - | o |
| `drake_cmake_external` | - | o |
| `drake_cmake_installed` | o | o |
||[![CircleCI](https://img.shields.io/circleci/project/github/RobotLocomotion/drake-shambhala/master.svg)](https://circleci.com/gh/RobotLocomotion/drake-shambhala)|[![Jenkins](https://img.shields.io/jenkins/s/https/drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-shambhala/job/master.svg)](https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-shambhala/)|

Note, the Circle CI jobs only build and test `drake_cmake_installed` since it is the exemplary case
for a lightweight, open-source build on a public CI server.
