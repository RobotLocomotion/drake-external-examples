# -*- python -*-

# The GCC_FLAGS will be enabled for all C++ rules in the project when
# building with gcc.
GCC_FLAGS = [
    "-Werror=all",
    "-Werror=extra",
    "-Werror=return-local-addr",
    "-Werror=non-virtual-dtor",
    "-Wno-unused-parameter",
    "-Wno-missing-field-initializers",
]

def shambhala_cc_library(
        name,
        hdrs=None,
        srcs=None,
        deps=None,
        copts=[],
        **kwargs):
    """
    Creates a rule to declare a C++ library.
    """
    native.cc_library(
        name=name,
        hdrs=hdrs,
        srcs=srcs,
        deps=deps,
        copts=copts,
        linkstatic=0,
        **kwargs)

def shambhala_cc_binary(
        name,
        srcs=None,
        deps=None,
        copts=[],
        **kwargs):
    """
    Creates a rule to declare a C++ binary.
    """
    native.cc_binary(
        name=name,
        srcs=srcs,
        deps=deps,
        copts=copts,
        testonly=0,
        linkstatic=0,
        **kwargs)
