"""
Automates the upgrade process for Drake's `WITH_USER_*` upstream dependencies
for drake_cmake_external.

Parses `CMakeLists.txt` to find calls to `ExternalProject_Add(<package>)`,
downloads the requested version of `<package>` and obtains its sha256sum, and
replaces the `URL` and `URL_HASH` arguments with the values for the new version.

Usage:
    python3 upgrade_cmake_externals.py --package <package> --version <version>

where `<version>` is of the form MAJOR.MINOR(.PATCH).
"""

import argparse
import hashlib
from pathlib import Path
import re
import sys
from urllib.request import urlopen


def _get_sha(url):
    """Given a url, downloads it (in memory) and returns the sha256 checksum."""
    hasher = hashlib.sha256()
    with urlopen(url) as response:
        while True:
            data = response.read(4096)
            if not data:
                break
            hasher.update(data)
    return hasher.hexdigest()


def re_externalproject(package):
    """
    Returns a regex pattern matching a call to CMake:

    `ExternalProject_Add(package ... URL <url> URL_HASH SHA256=<sha>)`

    and capturing <url> and <sha>.
    """
    return re.compile(
        r"ExternalProject_Add\s*\(\s*"
        rf"{package}\b[\s\S]*?"
        r"\bURL\s+(\S*)\s+"
        r"\bURL_HASH\s+SHA256=(\w{64})"
        r"[^\)]*\)"
    )


def main(args):
    # Parse our arguments.
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--package",
        type=str,
        required=True,
        help="Package to be upgraded.",
    )
    parser.add_argument(
        "--version",
        type=str,
        required=True,
        help="Version to upgrade the package to.",
    )
    options = parser.parse_args(args)

    # Find the CMakeLists.txt file for drake_cmake_external.
    cmake_file = (
        Path(__file__).parent.parent / "drake_cmake_external" / "CMakeLists.txt"
    )
    if not cmake_file.exists():
        raise RuntimeError("Could not find drake_cmake_external CMakeLists.txt.")
    text = cmake_file.read_text()

    # Locate the URL and SHA in CMakeLists.txt.
    match = re.search(re_externalproject(options.package), text)
    if not match:
        raise RuntimeError(
            f"Unable to locate call to ExternalProject_Add({options.package} ...)"
        )
    old_url = match.group(1)
    old_sha = match.group(2)
    print(f"+ Old URL: {old_url}")
    print(f"+ Old SHA: {old_sha}")

    # Determine the new URL.
    new_url = re.sub(r"\d+\.\d+(?:\.\d+)*", options.version, old_url)
    print(f"+ New URL: {new_url}")

    # Obtain the SHA.
    new_sha = _get_sha(url=new_url)
    print(f"+ New SHA: {new_sha}")

    # Replace the URL and SHA in CMakeLists.txt.
    new_text = text.replace(old_url, new_url).replace(old_sha, new_sha)
    cmake_file.write_text(new_text)


if __name__ == "__main__":
    main(sys.argv[1:])
