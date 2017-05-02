# -*- python -*-
# This file contains rules for Bazel;

load("//tools:shambhala.bzl", "shambhala_cc_library")

package(default_visibility = ["//visibility:public"])

# A library of things that EVERYONE should want and MUST EAT.
# Be appropriately hesitant when adding new things here.
shambhala_cc_library(
    name = "hello",
    srcs = [
        "hello.cpp",
    ],
    hdrs = [
        "hello.hpp",
    ],
    deps = [
        #        "@eigen//:eigen",
        #        "@spdlog//:spdlog",
    ],
)
