#!/usr/bin/env python
import sys, os

def options(opt):
    pass

def configure(cfg):
    # This non-standard installation behaviour is a HACK for some transitional
    # time. Note that, if you want to install the package at the system level
    # you can still explicitly specify the PREFIX var. For most unix style
    # systems the prefix should be '/usr/local'.
    cfg.env.PREFIX = cfg.env.PREFIX if os.getenv('PREFIX') else '.'

def build(bld):
    # only export include path, header only lib.
    bld(target          = 'rant',
        export_includes = '.')

    # install headers
    for header in bld.path.ant_glob('rant/**/*.(h)'):
        bld.install_as('${PREFIX}/include/%s' % header.path_from(bld.path), header)
