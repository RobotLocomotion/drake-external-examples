# -*- python -*-
# This file contains rules for Bazel;
load("//tools:shambhala.bzl", "shambhala_cc_binary")

shambhala_cc_binary(
    name = "hello",
    srcs = [
        "hello.cpp",
    ],
    deps = [
        "//shambhala:hello",
    ],
)
