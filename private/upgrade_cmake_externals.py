"""
Automates the upgrade process for Drake's `WITH_USER_*` upstream dependencies
for drake_cmake_external. For each external package, finds the current version
from the example's `CMakeLists.txt` file and compares it against what's being
used in Drake's `MODULE.bazel` file.

To check for and print any possible upgrades, run

    python3 upgrade_cmake_externals.py

To automatically perform (and optionally commit) upgrades, run

    python3 upgrade_cmake_externals.py [--commit] [<package>]

"""

import argparse
from dataclasses import dataclass
import hashlib
from pathlib import Path
import re
import shlex
import subprocess
import sys
from typing import Optional, Tuple
from urllib.parse import urljoin
from urllib.request import urlopen


PACKAGES_TO_UPGRADE = [
    "eigen",
    "fmt",
    "spdlog",
]


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


def _re_externalproject(package: str) -> re.Pattern:
    """
    Returns a regex pattern matching a call to CMake:

    `ExternalProject_Add(package ... URL <url> URL_HASH SHA256=<sha>)`

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


def _re_version() -> re.Pattern:
    """
    Returns a regular expression matching a version string, as
    `(v)MAJOR.MINOR(.PATCH)`.
    """
    return re.compile(r"v?\d+\.\d+(?:\.\d+)*")


def _re_bazeldep(package: str) -> re.Pattern:
    """
    Returns a regex pattern matching a call to Bazel:

    `bazel_dep(name = "<package>", version = "<version>" ...)`

    and capturing <version>.
    """
    return re.compile(
        rf"bazel_dep\s*\(\s*name\s*=\s*\"{package}\"\s*,"
        r"\s*version\s*=\s*\"([0-9|b|c|r|\\.]+)"
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
    match = re.search(_re_externalproject(package), text)
    if not match:
        raise RuntimeError(
            f"Unable to locate call to ExternalProject_Add({package} ...)"
        )
    return match.group(1), match.group(2)


def _extract_versions(package: str, cmake_file: Path) -> Tuple[str, str]:
    """
    Returns the version of the given package used by the given `CMakeLists.txt`
    file, and the version used by Drake's `MODULE.bazel` file.
    """
    # Find the current version.
    url, _ = _find_externalproject_args(package, cmake_file)
    match = re.search(_re_version(), url)
    if not match:
        raise RuntimeError(f"Unable to parse version number from URL: {url}")
    version = match.group(0)

    # Find the version that Drake is using.
    module_bazel = _get_raw_github_file("MODULE.bazel")
    match = re.search(_re_bazeldep(package), module_bazel)
    if not match:
        raise RuntimeError(
            f"Unable to parse version number for {package} from Drake's "
            "MODULE.bazel file."
        )

    # Match up the two version strings.
    drake_version = _canonicalize_bazel_version(version, match.group(1))

    return version, drake_version


def _check_for_package_upgrade(package: str, cmake_file: Path) -> PackageVersionInfo:
    """
    Determine if the given package should be upgraded according to Drake's
    version upstream.
    """
    version, drake_version = _extract_versions(package, cmake_file)
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
    new_url = re.sub(_re_version(), version, old_url)
    new_sha = _get_sha(url=new_url)

    # Replace the URL and SHA in CMakeLists.txt.
    new_text = text.replace(old_url, new_url).replace(old_sha, new_sha)
    cmake_file.write_text(new_text)


def _do_commit(package: str, version: str, cmake_file: Path, do_commit: bool) -> None:
    """
    Commits the changes to the given CMakeLists.txt file for the given
    package, or print what would have been committed.
    """
    message = f"[drake_cmake_external] Upgrade {package} to {version}"
    if do_commit:
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
        print(f"Done. Changes were committed for {package}.")
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
        "download new archives for the given externals "
        "and edit the CMakeLists.txt file to match.",
    )
    options = parser.parse_args(args)

    # Find the CMakeLists.txt file for drake_cmake_external.
    cmake_file = (
        Path(__file__).parent.parent / "drake_cmake_external" / "CMakeLists.txt"
    )
    if not cmake_file.exists():
        raise RuntimeError("Could not find drake_cmake_external CMakeLists.txt.")

    if len(options.package):
        # We are doing upgrade(s).
        for package in options.package:
            if package not in PACKAGES_TO_UPGRADE:
                raise RuntimeError(f"Invalid package {package}.")
            upgrade = _check_for_package_upgrade(package, cmake_file)
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
            upgrade = _check_for_package_upgrade(package, cmake_file)
            if upgrade.needs_upgrade:
                print(
                    f"{package} needs upgrade from {upgrade.current_version} "
                    f"to {upgrade.drake_version}"
                )
            else:
                print(f"No upgrades possible for {package}.")


if __name__ == "__main__":
    main(sys.argv[1:])
