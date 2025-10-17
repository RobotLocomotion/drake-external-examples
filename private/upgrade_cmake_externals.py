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
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile


def _run(argv, check=True, shell=False, **kwargs):
    """Runs as per `subprocess.run`, but with added command logging."""
    if shell:
        assert isinstance(argv, str)
        cmd = argv
    else:
        assert isinstance(argv, list)
        cmd = shlex.join(argv)
    print(f"+ {cmd}", file=sys.stderr)
    return subprocess.run(argv, check=check, shell=shell, **kwargs)


def _download_with_sha(*, base_url, filename):
    """Downloads `{base_url}/{filename}` into a temporary directory and returns
    its sha256sum.
    """
    with tempfile.TemporaryDirectory(
        prefix="drake-external-examples-upgrade-tmp"
    ) as scratch_dir:
        os.chdir(scratch_dir)
        _run(["wget", f"{base_url}/{filename}"])
        sha256_file = f"{filename}.sha256"
        _run(f"sha256sum {filename} > {sha256_file}", shell=True)
        _run(["sha256sum", "-c", sha256_file])
        return Path(sha256_file).read_text().split()[0]


def _check_url(*, url):
    """Checks the url exists."""
    assert url
    print(f"+ Verify URL: {url}", file=sys.stderr)
    proc = subprocess.run(["wget", "--spider", url], capture_output=True)
    if proc.returncode != 0:
        raise RuntimeError(
            f"Cannot find URL {url}:\n    {proc.stderr.decode('utf-8')}"
        )


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


if __name__ == "__main__":
    # Parse our arguments.
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--package",
        type=str,
        required=True,
        help="Package to be upgraded",
    )
    parser.add_argument(
        "--version",
        type=str,
        required=True,
        help="Version to upgrade to",
    )
    args = parser.parse_args()

    # Find the CMakeLists.txt file for drake_cmake_external.
    cmake_file = (
        Path(__file__).parent.parent / "drake_cmake_external" / "CMakeLists.txt"
    )
    if not cmake_file.exists():
        raise RuntimeError("Could not find drake_cmake_external CMakeLists.txt.")
    text = cmake_file.read_text()

    # Locate the URL and SHA in CMakeLists.txt.
    match = re.search(re_externalproject(args.package), text)
    if not match:
        raise RuntimeError(
            f"Unable to locate call to ExternalProject_Add({args.package} ...)"
        )
    old_url = match.group(1)
    old_sha = match.group(2)
    print(f"+ URL: {old_url}")
    print(f"+ SHA256: {old_sha}")

    # Determine the new URL.
    new_url = re.sub(r"\d+\.\d+(?:\.\d+)*", args.version, old_url)
    print(f"+ New URL: {new_url}")

    # Make sure the URL can be downloaded (fail-fast).
    _check_url(url=new_url)

    # Obtain the SHA.
    new_sha = _download_with_sha(
        base_url=new_url.rsplit("/", 1)[0], filename=new_url.rsplit("/", 1)[-1]
    )
    print(f"+ New SHA256: {new_sha}")

    # Replace the URL and SHA in CMakeLists.txt.
    new_text = re.sub(old_url, new_url, text)
    new_text = re.sub(old_sha, new_sha, new_text)
    cmake_file.write_text(new_text)
