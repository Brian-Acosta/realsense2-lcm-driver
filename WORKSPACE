# -*- mode: python -*-
# vi: set ft=python :

workspace(name = "realsense2_lcm_driver")

# Run a helper rule that brings in Drake.  To use a local checkout of Drake, or
# to change which git commit is downloaded, refer to the comments in drake.bzl.
load("//tools/workspace:drake.bzl", "drake_repository")

drake_repository()

# Run a helper rule that brings in all the rest of our dependencies.
load("//tools/workspace:default.bzl", "add_default_repositories")

add_default_repositories(os = ["macos"])
