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

def bitbucket_archive(
        name,
        repository = None,
        commit = None,
        sha256 = None,
        strip_prefix = None,
        build_file = None,
        **kwargs):
    """A macro to be called in the WORKSPACE that adds an external from
    bitbucket using a workspace rule.

    The required name= is the rule name and so is used for @name//... labels
    when referring to this archive from BUILD files.

    The required commit= is the hash to download.  (When a git project is
    also a git submodule in CMake, this should be kept in sync with the git
    submodule commit used there.)  This can also be a tag.

    The required sha256= is the checksum of the downloaded archive.  When
    unsure, you can omit this argument (or comment it out) and then the
    checksum-mismatch error message message will offer a suggestion.

    The required build_prefix= is the directory prefix to strip from the
    extracted files.

    The optional build_file= is the BUILD file label to use for building this
    external.  When omitted, the BUILD file(s) within the archive will be used.
    """
    if repository == None:
        fail("Missing repository=")
    if commit == None:
        fail("Missing commit=")
    if sha256 == None:
        # This is mostly-required, but we fallback to a wrong-default value to
        # allow the first attempt to fail and print the correct sha256.
        sha256 = "0" * 64
    if strip_prefix == None:
        fail("Missing strip_prefix=")

    urls = [
        "https://bitbucket.org/%s/get/%s.tar.gz" % (repository, commit)
    ]

    repository_split = repository.split("/")
    if len(repository_split) != 2:
        fail("The repository= must be formatted as 'organization/project'")

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
