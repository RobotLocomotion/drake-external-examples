# -*- mode: python -*-
# vi: set ft=python :

# Copyright (c) 2019, Toyota Research Institute.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
#
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
# * Neither the name of the copyright holder nor the names of its contributors
#   may be used to endorse or promote products derived from this software
#   without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

def _drake_impl(repo_ctx):
    root = repo_ctx.path(repo_ctx.attr.install_path)
    root.exists or fail("Missing " + root)

    # Symlink the data resources into the repository.
    share = root.get_child("share")
    share.exists or fail("Missing " + share)
    share_drake = share.get_child("drake")
    share_drake.exists or fail("Missing " + share_drake)
    data_files = []
    data_subdirs = []
    for resource_path in share_drake.readdir():
        basename = resource_path.basename
        is_file = basename.startswith(".")
        if is_file:
            data_files.append(basename)
        else:
            data_subdirs.append(basename)
        repo_ctx.symlink(resource_path, basename)

    # Symlink the binaries into the repository.
    lib = root.get_child("lib")
    lib.exists or fail("Missing " + lib)
    repo_ctx.symlink(lib, ".lib")

    # Add a pydrake package.
    resources_data = "{} + glob({})".format(data_files, [
        x + "/**" for x in data_subdirs
    ])
    repo_ctx.file(
        "BUILD.bazel",
        executable = False,
        content = """
filegroup(
    name = "resources",
    data = {resources_data},
)

_IMPORT = ".lib/python3.6/site-packages"
py_library(
    name = "pydrake",
    srcs = glob(include = [
        _IMPORT + "/**/*.py",
    ]),
    data = [":resources"] + glob(include = [
        _IMPORT + "/**/*.so",
        # TODO(jwnimmer-tri) Make a drake_shared_library label for these.
        ".lib/*.so",
        ".lib/*.so.*",
    ]),
    imports = [_IMPORT],
    visibility = ["//visibility:public"],
)
""".format(
        resources_data = resources_data,
    ))
    repo_ctx.file(
        "bindings/pydrake/BUILD.bazel",
        executable = False,
        content = """
alias(
    name = "pydrake",
    actual = "//:pydrake",
    visibility = ["//visibility:public"],
)
""")

_drake_repository = repository_rule(
    implementation = _drake_impl,
    attrs = {
         "install_path": attr.string(
             mandatory = True,
         )
    },
    local = True,
)


def drake_repository(install_path):
    _drake_repository(
        name = "drake",
        install_path = install_path,
    )
