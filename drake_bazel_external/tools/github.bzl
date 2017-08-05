# -*- mode: python -*-
# vi: set ft=python :

# Copyright (c) 2017, Massachusetts Institute of Technology.
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
# * Neither the name of the Massachusetts Institute of Technology nor the names
#   of its contributors may be used to endorse or promote products derived from
#   this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE MASSACHUSETTS INSTITUTE OF TECHNOLOGY AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
# NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE MASSACHUSETTS
# INSTITUTE OF TECHNOLOGY OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

def github_archive(
        name,
        repository = None,
        commit = None,
        sha256 = "0" * 64,  # fallback to trigger a checksum misatch error that will print the real sha256 to screen
        build_file = None,
        local_repository_override = None,
        **kwargs):
    """A macro to be called in the WORKSPACE that adds an external from github
    using a workspace rule.

    The required name= is the rule name and so is used for @name//... labels
    when referring to this archive from BUILD files.

    The required commit= is the git hash to download.  (When the git project is
    also a git submodule in CMake, this should be kept in sync with the git
    submodule commit used there.)  This can also be a tag.

    The required sha256= is the checksum of the downloaded archive. If you are
    unsure, leave this unset as the default will trigger the checksum-mismatch error
    and the resulting message will offer a suggestion. If set to None, it will skip matching
    attempts on the checksum (useful if the commit string is a moving branch).

    The optional build_file= is the BUILD file label to use for building this
    external.  When omitted, the BUILD file(s) within the archive will be used.

    The optional local_repository_override= can be used for temporary local
    testing; instead of retrieving the code from github, the code is retrieved
    from the local filesystem path given in the argument.
    """
    if repository == None:
        fail("Missing repository=")
    if commit == None:
        fail("Missing commit=")

    urls = [
        "https://github.com/%s/archive/%s.tar.gz" % (repository, commit)
    ]

    repository_split = repository.split("/")
    if len(repository_split) != 2:
        fail("The repository= must be formatted as 'organization/project'")
    _, project = repository_split
    strip_commit = commit
    if commit[0] == 'v':
        # Github archives omit the "v" in version tags, for some reason.
        strip_commit = commit[1:]
    strip_prefix = project + "-" + strip_commit

    if local_repository_override != None:
        if build_file == None:
            native.local_repository(
                name = name,
                path = local_repository_override)
        else:
            native.new_local_repository(
                name = name,
                build_file = build_file,
                path = local_repository_override)
        return

    if build_file == None:
        native.http_archive(
            name = name,
            urls = urls,
            sha256 = sha256,
            strip_prefix = strip_prefix,
            **kwargs)
    else:
        native.new_http_archive(
            name = name,
            urls = urls,
            sha256 = sha256,
            build_file = build_file,
            strip_prefix = strip_prefix,
            **kwargs)
