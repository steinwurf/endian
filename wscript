#! /usr/bin/env python
# encoding: utf-8

import os
import hashlib
import shutil
import platform
import waflib
import pathlib
import subprocess

from waflib.Build import BuildContext

APPNAME = "endian"
VERSION = "13.0.0"


def options(opt):
    opt.load("cmake")


def configure(conf):
    conf.load("cmake")

    if conf.is_toplevel():
        conf.cmake_configure()


def build(bld):
    bld.load("cmake")

    if bld.is_toplevel():
        bld.cmake_build()


def docs(ctx):
    """Build the documentation in a virtualenv"""

    with ctx.create_virtualenv() as venv:
        # To update the requirements.txt just delete it - a fresh one
        # will be generated from test/requirements.in
        if not os.path.isfile("docs/requirements.txt"):
            venv.run("python -m pip install pip-tools")
            venv.run("pip-compile docs/requirements.in")

        venv.run("python -m pip install -r docs/requirements.txt")

        build_path = os.path.join(ctx.path.abspath(), "build", "site", "docs")

        venv.run(
            "giit clean . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )
        venv.run(
            "giit sphinx . --build_path {}".format(build_path), cwd=ctx.path.abspath()
        )
