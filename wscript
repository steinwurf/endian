#! /usr/bin/env python
# encoding: utf-8

import os

APPNAME = "endian"
VERSION = "13.0.0"


def configure(conf):
    conf.set_cxx_std(11)


def build(bld):
    bld(name="endian_includes", includes="./src", export_includes="./src")

    if bld.is_toplevel():
        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse("test")
        bld.recurse("examples")


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
