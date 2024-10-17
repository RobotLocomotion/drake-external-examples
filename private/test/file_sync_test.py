#!/usr/bin/env python3
# SPDX-License-Identifier: MIT-0

"""This regression test checks that files which are supposed to be copies of
each other are actually so.
"""

import logging
import os
import sys
from pathlib import Path

COPIES = (
    (
        "drake_ament_cmake_installed/.clang-format",
        "drake_bazel_download/.clang-format",
        "drake_bazel_external/.clang-format",
        "drake_catkin_installed/.clang-format",
        "drake_cmake_external/.clang-format",
        "drake_cmake_installed/.clang-format",
        "drake_cmake_installed_apt/.clang-format",
    ),
    (
        "drake_bazel_download/.github/ubuntu_setup",
        "drake_bazel_download/.github/ubuntu_setup",
        "drake_catkin_installed/.github/ubuntu_setup",
        "drake_cmake_installed/.github/ubuntu_setup",
        "drake_cmake_external/.github/ubuntu_setup",
    ),
    (
        "drake_ament_cmake_installed/CPPLINT.cfg",
        "drake_bazel_download/CPPLINT.cfg",
        "drake_bazel_external/CPPLINT.cfg",
        "drake_catkin_installed/CPPLINT.cfg",
        "drake_cmake_external/CPPLINT.cfg",
        "drake_cmake_installed/CPPLINT.cfg",
        "drake_cmake_installed_apt/CPPLINT.cfg",
    ),
    (
        "LICENSE",
        "drake_ament_cmake_installed/LICENSE",
        "drake_bazel_download/LICENSE",
        "drake_bazel_external/LICENSE",
        "drake_catkin_installed/LICENSE",
        "drake_cmake_external/LICENSE",
        "drake_cmake_installed/LICENSE",
        "drake_cmake_installed_apt/LICENSE",
    ),
    (
        "drake_bazel_external/.github/setup",
        "drake_cmake_external/.github/setup",
    ),
    (
        "drake_bazel_external/setup/install_prereqs",
        "drake_cmake_external/setup/install_prereqs",
    ),
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


def main():
    logging.basicConfig(format="%(levelname)s: %(message)s")
    os.chdir(Path(__file__).parent.parent.parent)
    for i, paths in enumerate(COPIES):
        check(i, paths)
    sys.exit(1 if found_errors else 0)


if __name__ == "__main__":
    main()
