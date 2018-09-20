#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'endian'
VERSION = '10.0.0'


def build(bld):

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_ENDIAN_VERSION="{}"'.format(
            VERSION))

    bld(name='endian_includes',
        includes='./src',
        export_includes='./src')

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse('test')
        bld.recurse('examples')
