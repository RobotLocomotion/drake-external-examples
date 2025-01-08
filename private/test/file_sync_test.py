#!/usr/bin/env python3
# SPDX-License-Identifier: MIT-0

"""This regression test checks that files which are supposed to be copies of
each other are actually so.
"""

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
    ),
    (
        "drake_bazel_download/.github/ci_build_test",
        "drake_bazel_external/.github/ci_build_test",
    ),
    (
        "drake_bazel_external/.github/ubuntu_setup",
        "drake_cmake_external/.github/ubuntu_setup",
    ),
    (
        "drake_bazel_external/setup/install_prereqs",
        "drake_cmake_external/setup/install_prereqs",
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
        ".bazelrc",
        ".clang-format",
        ".github/ci_build_test",
        ".github/setup",
        ".github/ubuntu_setup",
        ".gitignore",
        "BUILD.bazel",
        "CPPLINT.cfg",
        "LICENSE",
        "WORKSPACE",
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
        "environ.bzl",
        "setup/install_prereqs",
    ]
])

GITHUB_WORKFLOWS = (
    "bazel_download",
    "cmake_installed",
    "cmake_installed_apt",
)

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
    """Enforces the subdir_ci to have the contents of root_ci up until 
    it reaches the jobs: line plus the content in the subdir_ workflow
    after jobs: is mentioned.
    """
    root_ci_path = ".github/workflows/ci.yml"
    subdir_workflow_path = f".github/workflows/{workflow_name}.yml"
    subdir_ci_path = f"drake_{workflow_name}/.github/workflows/ci.yml"
    sep = "\njobs:\n"

    # Read all files into memory and check sep occurs once in each file.
    content = {}
    for path in [root_ci_path, subdir_workflow_path, subdir_ci_path]:
        try:
            with open(path, "r", encoding="utf-8") as f:
                content[path] = f.read()
                if len(re.findall(sep, content[path])) != 1:
                    error(f"{workflow_name}'s {path} contents are invalid")
        except IOError:
            error(f"Missing {workflow_name} file {path}")

    # Workflow check.
    events = content[root_ci_path].split(sep)[0]
    ex_jobs = sep + content[subdir_workflow_path].split(sep)[1]

    if events + ex_jobs != content[subdir_ci_path]:
        error(f"{workflow_name} subdir CI does not match")


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
