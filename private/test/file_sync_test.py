#!/usr/bin/env python3
# SPDX-License-Identifier: MIT-0

"""This regression test checks that files which are supposed to be copies of
each other are actually so.
"""

from io import StringIO
from itertools import chain
import logging
import os
import re
import sys
from pathlib import Path
import ruamel.yaml

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
        "linux_noble_package_tar",
    ),
    "cmake_installed": (
        "linux_noble_package_tar",
        "mac_arm_sonoma_package_tar"
    ),
    "cmake_installed_apt": (
        "linux_noble_package_deb",
    ),
    "pip": (
        "linux_noble_wheel",
        "mac_arm_sonoma_wheel"
    ),
    "poetry": (
        "linux_noble_wheel",
        "mac_arm_sonoma_wheel"
    )
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
    seps = {
        "dispatch": "\n  workflow_dispatch:\n    inputs:\n",
        "conc": "\nconcurrency:\n",
        "jobs": "\njobs:\n"
    }

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
    check_sep([root_ci_path, subdir_ci_path], seps["dispatch"])
    check_sep([root_ci_path, subdir_ci_path], seps["conc"])
    check_sep(paths, seps["jobs"])

    # Workflow check. See above for steps.
    root_events = content[root_ci_path].split(seps["dispatch"])[0]
    subdir_events = content[subdir_ci_path].split(seps["dispatch"])[0]
    if root_events != subdir_events:
        error(f"{workflow_name} subdir CI events do not match")

    root_dispatch = content[root_ci_path].split(seps["dispatch"])[1].split(seps["conc"])[0]
    subdir_dispatch = content[subdir_ci_path].split(seps["dispatch"])[1].split(seps["conc"])[0]
    subdir_dispatch = re.sub("[^\S\r\n]{6}", "", subdir_dispatch) # remove leading indendation

    # Check that the root workflow defines no extra options
    # beyond what is configured here.
    all_dispatch_options = set(chain(*GITHUB_WORKFLOW_OPTS.values()))
    root_dispatch_options = set([line.lstrip().rstrip(':') for line in root_dispatch.splitlines() if len(line) - len(line.lstrip()) == 6])
    if root_dispatch_options != all_dispatch_options:
        error(f"Root CI workflow_dispatch does not define the expected options. Please add them to the file_sync_test.")
        exit(1)

    # Load root workflow as yaml, delete the unused options, and check
    # dumped string for equality to subdir workflow.
    yaml = ruamel.yaml.YAML()
    yaml.preserve_quotes = True # Enforce use of '' for quoted strings.
    root_dispatch_yaml = yaml.load(root_dispatch)
    unused_options = set.difference(all_dispatch_options, GITHUB_WORKFLOW_OPTS[workflow_name])
    for unused in unused_options:
        del root_dispatch_yaml[unused]
    with StringIO() as string_stream:
        yaml.dump(root_dispatch_yaml, string_stream)
        root_dispatch_parsed = string_stream.getvalue().rstrip('\n')
    if root_dispatch_parsed != subdir_dispatch:
        error(f"{workflow_name} subdir CI workflow_dispatch does not match")

    root_conc = content[root_ci_path].split(seps["conc"])[1].split(seps["jobs"])[0]
    subdir_conc = content[subdir_ci_path].split(seps["conc"])[1].split(seps["jobs"])[0]
    if root_conc != subdir_conc:
        error(f"{workflow_name} subdir CI concurrency does not match")

    subdir_jobs = content[subdir_ci_path].split(seps["jobs"])[1]
    subdir_workflow_jobs = content[subdir_workflow_path].split(seps["jobs"])[1]
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
