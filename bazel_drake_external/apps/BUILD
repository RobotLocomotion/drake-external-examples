# -*- python -*-
# This file contains rules for Bazel;
load("//tools:shambhala.bzl", "shambhala_cc_binary")

cc_binary(
    name = "hello-dudes",
    srcs = [
        "hello.cpp",
    ],
    deps = [
        "@eigen",
        "@drake//drake/automotive:curve2",
        "//shambhala:hello",
    ],
)
