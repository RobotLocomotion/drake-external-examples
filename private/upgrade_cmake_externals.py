"""
Automates the upgrade process for Drake's `WITH_USER_*` upstream dependencies
for drake_cmake_external. For each external package, finds the current version
from the example's `CMakeLists.txt` file and compares it against what's being
used in Drake's `MODULE.bazel` file.

To check for and print any possible upgrades, run

    python3 upgrade_cmake_externals.py

To automatically perform (and optionally commit) upgrades, run

    python3 upgrade_cmake_externals.py [--commit] <package> [<package> ...]

"""

import argparse
import ast
from dataclasses import dataclass
import hashlib
from pathlib import Path
import re
import shlex
import subprocess
import sys
from typing import Any, Dict, Optional, Tuple
from urllib.parse import urljoin
from urllib.request import urlopen


PACKAGES_TO_UPGRADE = [
    "eigen",
    "fmt",
    "spdlog",
]


RE_VERSION: re.Pattern = re.compile(r"v?\d+(?:\.\d+)+")
"""A regular expression matching a version string, as `(v)MAJOR.MINOR(.PATCH)`."""


@dataclass
class PackageVersionInfo:
    current_version: str
    drake_version: str
    needs_upgrade: Optional[bool] = None

    def __post_init__(self):
        self.needs_upgrade = self.current_version != self.drake_version


def _get_sha(url: str) -> str:
    """
    Given a url, downloads it (in memory) and returns the sha256 checksum.
    """
    hasher = hashlib.sha256()
    with urlopen(url) as response:
        while True:
            data = response.read(4096)
            if not data:
                break
            hasher.update(data)
    return hasher.hexdigest()


def _get_raw_github_file(
    file_path: str,
    org: Optional[str] = "RobotLocomotion",
    repo: Optional[str] = "drake",
    default_branch: Optional[str] = "master",
) -> str:
    """
    Given a path to a file within a GitHub repo, returns its contents on the
    latest master.
    """
    url = urljoin(
        f"https://raw.githubusercontent.com/{org}/{repo}/{default_branch}/",
        file_path,
    )
    with urlopen(url) as response:
        if response.status != 200:
            raise RuntimeError(
                f"The file '{file_path}' was not found on branch"
                f"'{default_branch}' of the '{org}/{repo}' repository."
            )
        return response.read().decode("utf-8")


def _extract_dependencies(module_content: str) -> Dict[str,Dict[str,Any]]:
    """
    Extracts the `bazel_dep`s from the given module file into a dictionary
    where the keys are the names of the dependencies and all other data is
    stored in the values.
    """
    deps = {}

    class Visitor(ast.NodeVisitor):
        def visit_Call(self, node):
            if isinstance(node.func, ast.Name):
                dep = {}
                if node.func.id == "bazel_dep":
                    for arg in node.keywords:
                        if isinstance(arg.value, ast.Constant):
                            dep[arg.arg] = arg.value.value

                if len(dep) and "name" in dep:
                    deps[dep["name"]] = dep
                    del dep["name"]

            self.generic_visit(node)

    tree = ast.parse(module_content)
    Visitor().visit(tree)
    return deps


def _re_externalproject(package: str) -> re.Pattern:
    """
    Returns a regex pattern matching a call to CMake:

    `ExternalProject_Add(<package> ... URL <url> URL_HASH SHA256=<sha>)`

    and capturing <url> and <sha>.
    """
    return re.compile(
        r"ExternalProject_Add\s*\(\s*"
        rf"{package}\b.*?"
        r"\bURL\s+(\S*)\s+"
        r"\bURL_HASH\s+SHA256=(\w{64})"
        r"[^\)]*\)",
        flags=re.DOTALL,
    )


def _canonicalize_bazel_version(raw_version: str, bazel_version: str) -> str:
    """
    Canonicalizes the version number used in Bazel by:
    * stripping any Bazel central registry bits
    * prepending a "v" if the raw version from GitHub tags also has one
    """
    result = bazel_version.partition(".bcr")[0]
    if raw_version.startswith("v") and not result.startswith("v"):
        result = f"v{result}"
    return result


def _find_externalproject_args(package: str, cmake_file: Path) -> Tuple[str, str]:
    """
    Returns the arguments to `URL` and `URL_HASH` from `ExternalProject_Add`.
    """
    text = cmake_file.read_text()
    match = _re_externalproject(package).search(text)
    if not match:
        raise RuntimeError(
            f"Unable to locate call to ExternalProject_Add({package} ...)"
        )
    return match.group(1), match.group(2)


def _check_for_package_upgrade(
    package: str, cmake_file: Path, bazel_deps: Dict[str,Dict[str,Any]]
) -> PackageVersionInfo:
    """
    Determines if the given package should be upgraded according to Drake's
    version upstream.
    """
    # Find the current version.
    url, _ = _find_externalproject_args(package, cmake_file)
    match = RE_VERSION.search(url)
    if not match:
        raise RuntimeError(f"Unable to extract version number from URL: {url}")
    version = match.group(0)

    # Find the version that Drake is using.
    if package not in bazel_deps:
        raise RuntimeError(
            f"Unable to extract {package} from Drake's MODULE.bazel file."
        )
    if "version" not in bazel_deps[package]:
        raise RuntimeError(
            f"Unable to extract version for {package} "
            "from Drake's MODULE.bazel file."
        )
    drake_version = bazel_deps[package]["version"]

    # Match up the two version strings.
    drake_version = _canonicalize_bazel_version(version, drake_version)

    return PackageVersionInfo(current_version=version, drake_version=drake_version)


def _do_package_upgrade(package: str, version: str, cmake_file: Path) -> None:
    """
    Upgrades the given package to the given version by automatically replacing
    the `URL` and `URL_HASH` arguments to `ExternalProject_Add` in the given
    `CMakeLists.txt` file.
    """
    print(f"Upgrading {package} to {version}.")

    text = cmake_file.read_text()

    # Locate the URL and SHA in CMakeLists.txt.
    old_url, old_sha = _find_externalproject_args(package, cmake_file)

    # Determine the new URL and SHA.
    new_url = RE_VERSION.sub(version, old_url)
    new_sha = _get_sha(url=new_url)

    # Replace the URL and SHA in CMakeLists.txt.
    new_text = text.replace(old_url, new_url).replace(old_sha, new_sha)
    cmake_file.write_text(new_text)


def _do_commit(
    package: str, version: str, cmake_file: Path, actually_commit: bool,
) -> None:
    """
    Commits the changes to the given CMakeLists.txt file for the given
    package, or prints what would have been committed.
    """
    message = f"[drake_cmake_external] Upgrade {package} to {version}"
    if actually_commit:
        subprocess.check_call(
            [
                "git",
                "add",
                "-A",
                str(cmake_file),
            ],
            cwd=Path(__file__).parent,
        )
        subprocess.check_call(
            [
                "git",
                "commit",
                "-o",
                str(cmake_file),
                "-m",
                message,
            ],
            cwd=Path(__file__).parent,
        )
        print(f"Done. Changes for {package} were committed.")
        print("Be sure to review the changes and amend the commit if needed.")
    else:
        print("Done. Be sure to review and commit the changes:")
        print(f"  git add {cmake_file}")
        print(f"  git commit -m{shlex.quote(message)}")


def main(args):
    # Parse our arguments.
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--commit",
        action="store_true",
        default=False,
        help="When upgrading packages, automatically commit the changes.",
    )
    parser.add_argument(
        "package",
        nargs="*",
        type=str,
        help="(Optional) Instead of reporting on possible upgrades, "
        "update the CMakeLists.txt with the new URL(s) and checksum(s) "
        "for the specified packages."
    )
    options = parser.parse_args(args)

    if options.commit and not len(options.package):
        raise ValueError(
            "If --commit is specified, there must be also be at least one "
            "package to be upgraded, specified as a positional argument."
        )

    # Find the CMakeLists.txt file for drake_cmake_external.
    cmake_file = (
        Path(__file__).parent.parent / "drake_cmake_external" / "CMakeLists.txt"
    )
    if not cmake_file.exists():
        raise RuntimeError("Could not find drake_cmake_external CMakeLists.txt.")

    # Parse Drake's MODULE.bazel for its packages.
    module_bazel = _get_raw_github_file("MODULE.bazel")
    bazel_deps = _extract_dependencies(module_bazel)

    if len(options.package):
        # We are doing upgrade(s).
        for package in options.package:
            if package not in PACKAGES_TO_UPGRADE:
                raise ValueError(f"Invalid package {package}.")
            upgrade = _check_for_package_upgrade(package, cmake_file, bazel_deps)
            if upgrade.needs_upgrade:
                _do_package_upgrade(package, upgrade.drake_version, cmake_file)
                _do_commit(
                    package,
                    upgrade.drake_version,
                    cmake_file,
                    options.commit,
                )
    else:
        # We are only checking for possible upgrades.
        for package in PACKAGES_TO_UPGRADE:
            upgrade = _check_for_package_upgrade(package, cmake_file, bazel_deps)
            if upgrade.needs_upgrade:
                print(
                    f"{package} needs upgrade from {upgrade.current_version} "
                    f"to {upgrade.drake_version}"
                )


if __name__ == "__main__":
    main(sys.argv[1:])
