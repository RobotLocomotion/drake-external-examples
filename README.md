# Drake External Examples

Examples of how to use [Drake](https://github.com/RobotLocomotion/drake) in your
own project:

* [`drake_bazel_external`](./drake_bazel_external)
* [`drake_bazel_download`](./drake_bazel_download)
* [`drake_cmake_external`](./drake_cmake_external)
* [`drake_cmake_installed`](./drake_cmake_installed)
* [`drake_cmake_installed_apt`](./drake_cmake_installed_apt)
* [`drake_pip`](./drake_pip)
* [`drake_poetry`](./drake_poetry)

## Continuous Integration

Scripts are provided for Jenkins or GitHub Actions, depending on the example,
in each example's respective subdirectory under `.github/`.
See the `Jenkinsfile` and the top-level `.github/workflows/ci.yml`
for a complete overview of each respective pipeline.
The intended purpose of each is described below:

* GitHub Actions: exemplifies how to put a project depending on a Drake installation on GitHub Actions
* Jenkins: provides complete coverage of additional external example projects

| **Subproject** | **GitHub Actions** | **Jenkins** |
|:---:|:---:|:---:|
| `drake_bazel_external` | - | o |
| `drake_bazel_download` | o | - |
| `drake_cmake_external` | - | o |
| `drake_cmake_installed` | o | - |
| `drake_cmake_installed_apt` | o | - |
| `drake_pip` | o | - |
| `drake_poetry` | o | - |
|| ![GitHub Actions](https://img.shields.io/github/actions/workflow/status/RobotLocomotion/drake-external-examples/ci.yml?branch=main) | [![Jenkins](https://img.shields.io/jenkins/build.svg?jobUrl=https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/job/main)](https://drake-jenkins.csail.mit.edu/job/RobotLocomotion/job/drake-external-examples/) |


Note, the GitHub Actions jobs only build and test `drake_bazel_download`,
`drake_cmake_installed`, `drake_cmake_installed_apt`, `drake_pip`, and
`drake_poetry`, since these are the exemplary cases for lightweight,
open-source builds on public CI servers.
