#!/usr/bin/env python3
# SPDX-License-Identifier: MIT-0

"""This regression test checks that files which are supposed to be copies of
each other are actually so.
"""

from itertools import chain
import logging
import os
import re
import sys
from pathlib import Path

COPIES = (
    (
        "drake_bazel_download/.clang-format",
        "drake_bazel_external/.clang-format",
        "drake_cmake_external/.clang-format",
        "drake_cmake_installed/.clang-format",
        "drake_cmake_installed_apt/.clang-format",
    ),
    (
        "drake_bazel_download/.github/ubuntu_setup",
        "drake_cmake_installed/.github/ubuntu_setup",
    ),
    (
        "drake_bazel_download/CPPLINT.cfg",
        "drake_bazel_external/CPPLINT.cfg",
        "drake_cmake_external/CPPLINT.cfg",
        "drake_cmake_installed/CPPLINT.cfg",
        "drake_cmake_installed_apt/CPPLINT.cfg",
    ),
    (
        "LICENSE",
        "drake_bazel_download/LICENSE",
        "drake_bazel_external/LICENSE",
        "drake_cmake_external/LICENSE",
        "drake_cmake_installed/LICENSE",
        "drake_cmake_installed_apt/LICENSE",
        "drake_pip/LICENSE",
        "drake_poetry/LICENSE",
    ),
    # TODO(tyankee): These aren't synced right now because
    # drake_bazel_download runs in GHA while drake_bazel_external
    # runs in Jenkins, and only GHA has been updated so far.
    # Perhaps this can be brought back once the Jenkins changes
    # are introduced.
    # (
    #     "drake_bazel_download/.github/ci_build_test",
    #     "drake_bazel_external/.github/ci_build_test",
    # ),
    (
        "drake_bazel_external/.github/ubuntu_setup",
        "drake_cmake_external/.github/ubuntu_setup",
    ),
    (
        "drake_bazel_download/.bazelversion",
        "drake_bazel_external/.bazelversion",
    ),
) + tuple([
    (
        f"drake_bazel_external/{path}",
        f"drake_bazel_external_legacy/{path}",
    )
    for path in [
        ".bazelignore",
        ".bazelproject",
        ".clang-format",
        ".github/ci_build_test",
        ".github/setup",
        ".github/ubuntu_setup",
        "BUILD.bazel",
        "CPPLINT.cfg",
        "LICENSE",
        "apps/BUILD.bazel",
        "apps/exec.sh",
        "apps/find_resource_test.py",
        "apps/import_all_test.py",
        "apps/simple_adder-inl.h",
        "apps/simple_adder.cc",
        "apps/simple_adder.h",
        "apps/simple_adder_py.cc",
        "apps/simple_adder_py_test.py",
        "apps/simple_adder_test.cc",
        "apps/simple_continuous_time_system.cc",
        "apps/simple_logging_example.py",
        "setup/install_prereqs",
    ]
]) + tuple([
    (
        f"drake_cmake_installed/src/particle/{path}",
        f"drake_cmake_installed_apt/src/{path}",
    )
    for path in [
        "CMakeLists.txt",
        "particle.cc",
        "particle.h",
        "particle_test.cc",
    ]
]) + tuple([
    (
        f"drake_cmake_installed/src/particle/{path}",
        f"drake_cmake_installed_apt/src/{path}",
        f"drake_pip/src/{path}",
        f"drake_poetry/src/{path}",
    )
    for path in [
        "particle.py",
        "particle_test.py",
    ]
])

GITHUB_WORKFLOWS = (
    "bazel_download",
    "cmake_installed",
    "cmake_installed_apt",
    "pip",
    "poetry"
)

GITHUB_WORKFLOW_OPTS = {
    "bazel_download": (
        "linux_jammy_package_tar",
    ),
    "cmake_installed": (
        "linux_jammy_package_tar",
        "mac_arm_sonoma_package_tar"
    ),
    "cmake_installed_apt": (
        "linux_jammy_package_deb",
    ),
    **dict.fromkeys([
        "pip",
        "poetry"
    ],
    (
        "linux_jammy_wheel",
        "mac_arm_sonoma_wheel"
    ))
}

found_errors = False


def _ordinalize(number: int) -> str:
    if (number % 100) // 10 == 1:
        return f"{number}th"

    SUFFIXES = {1: "st", 2: "nd", 3: "rd"}
    return f"{number}{SUFFIXES.get(number % 10, 'th')}"


def error(message: str):
    logging.error(message)
    global found_errors
    found_errors = True


def check(index: int, paths: tuple[str]):
    # For readability, enforce that the lists are sorted.
    first_name = Path(paths[0]).name
    prologue = (f"The {_ordinalize(index + 1)} list of files"
                f" (containing {first_name})")
    if list(paths) != sorted(paths):
        error(f"{prologue} is not alpha-sorted; fix the file_sync_test code.")

    # Read all of the files into memory.
    content = {}
    for path in paths:
        try:
            with open(path, "rb") as f:
                content[path] = f.read()
        except IOError:
            error(f"{prologue} refers to a missing file {path}")
    paths = list(content.keys())

    # Check for matching.
    all_match = all(
        content[a] == content[b]
        for a, b in zip(paths, paths[1:])
    )
    if not all_match:
        error(f"{prologue} do not all match")

def gha_workflow_check(workflow_name: str):
    """Enforces the following contents of root_ci and subdir_ci:
        1. subdir_ci and root_ci match until 'workflow_dispatch:'
        2. root_ci and subdir_ workflow match from 'workflow_dispatch:' until 'concurrency:',
           for only the options corresponding to the given workflow
        3. subdir_ci and root_ci match from 'concurrency:' until 'jobs:'
        4. subdir_ci and subdir_ workflow match after 'jobs:'
    """
    root_ci_path = ".github/workflows/ci.yml"
    subdir_workflow_path = f".github/workflows/{workflow_name}.yml"
    subdir_ci_path = f"drake_{workflow_name}/.github/workflows/ci.yml"
    paths = [ root_ci_path, subdir_workflow_path, subdir_ci_path ]
    seps = [
        "\n  workflow_dispatch:\n    inputs:\n",
        "\nconcurrency:\n",
        "\njobs:\n"
    ]

    # Read all files into memory.
    content = {}
    for path in paths:
        try:
            with open(path, "r", encoding="utf-8") as f:
                content[path] = f.read()
        except IOError:
            error(f"Missing {workflow_name} file {path}")
            exit(1)

    # Check that seps occur once in each file.
    def check_sep(paths, sep):
        for path in paths:
            if len(re.findall(sep, content[path])) != 1:
                error(f"{workflow_name}'s {path} contents are invalid: does not include {sep}")
                exit(1)
    check_sep([root_ci_path, subdir_ci_path], seps[0])
    check_sep([root_ci_path, subdir_ci_path], seps[1])
    check_sep(paths, seps[2])

    # Workflow check. See above for steps.
    root_events = content[root_ci_path].split(seps[0])[0]
    subdir_events = content[subdir_ci_path].split(seps[0])[0]
    if root_events != subdir_events:
        error(f"{workflow_name} subdir CI events do not match")

    # Custom logic for workflow dispatch based on options defined above.
    root_dispatch = (content[root_ci_path].split(seps[0]))[1].split(seps[1])[0].splitlines()
    subdir_dispatch = (content[subdir_ci_path].split(seps[0]))[1].split(seps[1])[0].splitlines()
    def get_option_definition(dispatch, option_line):
        dispatch_option = [option_line]
        l = dispatch.index(option_line) + 1
        while l < len(dispatch):
            if len(dispatch[l]) - len(dispatch[l].lstrip()) == 8:
                dispatch_option.append(dispatch[l])
                l += 1
            else:
                break
        return dispatch_option

    for dispatch_option in GITHUB_WORKFLOW_OPTS[workflow_name]:
        dispatch_option_line = f"      {dispatch_option}:"

        # Check that all options occur in both files.
        if (dispatch_option_line not in root_dispatch or
            dispatch_option_line not in subdir_dispatch):
            error(f"{workflow_name} subdir workflow_dispatch does not match: missing {dispatch_option}")
            exit(1)

        # Find the next line which is not indented to the sub-level,
        # representing the next option (or end).
        root_dispatch_option = get_option_definition(root_dispatch, dispatch_option_line)
        subdir_dispatch_option = get_option_definition(subdir_dispatch, dispatch_option_line)
        if root_dispatch_option != subdir_dispatch_option:
            error(f"{workflow_name} subdir CI workflow_dispatch option {dispatch_option} does not match")

    # Check that the subdir and root workflows define no extra options
    # beyond what is configured here.
    subdir_dispatch_options = sum([1 for line in subdir_dispatch if len(line) - len(line.lstrip()) == 6])
    if subdir_dispatch_options != len(GITHUB_WORKFLOW_OPTS[workflow_name]):
        error(f"{workflow_name} subdir CI defines additional options than expected. Please add them to the file_sync_test.")
    all_dispatch_options = set(chain(*GITHUB_WORKFLOW_OPTS.values()))
    root_dispatch_options = sum([1 for line in root_dispatch if len(line) - len(line.lstrip()) == 6])
    if root_dispatch_options != len(all_dispatch_options):
        error(f"Root CI workflow_dispatch defines additional options than expected. Please add them to the file_sync_test.")
    # End workflow dispatch custom logic.

    root_conc = (content[root_ci_path].split(seps[1]))[1].split(seps[2])[0]
    subdir_conc = (content[subdir_ci_path].split(seps[1]))[1].split(seps[2])[0]
    if root_conc != subdir_conc:
        error(f"{workflow_name} subdir CI concurrency does not match")

    subdir_jobs = content[subdir_ci_path].split(seps[2])[1]
    subdir_workflow_jobs = content[subdir_workflow_path].split(seps[2])[1]
    if subdir_jobs != subdir_workflow_jobs:
        error(f"{workflow_name} subdir CI jobs do not match")


def main():
    logging.basicConfig(format="%(levelname)s: %(message)s")
    os.chdir(Path(__file__).parent.parent.parent)
    for i, paths in enumerate(COPIES):
        check(i, paths)
    for workflow in GITHUB_WORKFLOWS:
        gha_workflow_check(workflow)
    sys.exit(1 if found_errors else 0)


if __name__ == "__main__":
    main()
