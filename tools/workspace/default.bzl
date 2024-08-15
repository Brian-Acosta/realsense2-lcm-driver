# -*- python -*-
# vi: set ft=python :

# Reference external software libraries and tools per Drake's defaults.  Some
# software will come from the host system (Ubuntu or macOS); other software
# will be downloaded in source or binary form from github or other sites.
load(
    "@drake//tools/workspace:default.bzl",
    drake_add_default_workspace = "add_default_workspace",
)
load("//tools/workspace:mirrors.bzl", "DEFAULT_MIRRORS")
load("//tools/workspace/boost:repository.bzl", "boost_repository")
load("//tools/workspace/libjpeg:repository.bzl", "libjpeg_repository")
load("//tools/workspace/libtiff:repository.bzl", "libtiff_repository")
load("//tools/workspace/opencv:repository.bzl", "opencv_repository")
load("//tools/workspace/realsense2:repository.bzl", "realsense2_repository")
load("//tools/workspace/tbb:repository.bzl", "tbb_repository")
load("//tools/workspace/opencv_pkgconfig:repository.bzl", "opencv_pkgconfig_repository")
load("//tools/workspace/realsense2_pkgconfig:repository.bzl", "realsense2_pkgconfig_repository")

def add_default_repositories(excludes = [], os = [], mirrors = DEFAULT_MIRRORS):
    # N.B. We do *not* pass `mirrors = ` into the drake_add_default_... call.
    # We want to use its (wider) set of defaults for the packages it knows
    # about; we only use Anzu's mirrors list for Anzu-specific packages.
    drake_add_default_workspace(repository_excludes = [
        "boost",
        "godotengine",  # TODO(jeremy-nimmer) Push our patches back into Drake.
        "snopt",
    ])
    if "boost" not in excludes:
        boost_repository(name = "boost")
    if "libjpeg" not in excludes:
        libjpeg_repository(name = "libjpeg")
    if "libtiff" not in excludes:
        libtiff_repository(name = "libtiff")
    if "opencv" not in excludes and "macos" not in os:
        opencv_repository(name = "opencv", mirrors = mirrors)
    if "opencv" not in excludes and "macos" in os:
        opencv_pkgconfig_repository(name = "opencv")
    if "realsense2" not in excludes and "macos" not in os:
        realsense2_repository(name = "realsense2")
    if "realsense2" not in excludes and "macos" in os:
        realsense2_pkgconfig_repository(name = "realsense2")
    if "tbb" not in excludes:
        tbb_repository(name = "tbb")
