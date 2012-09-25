#!/usr/bin/env python
import sys, os

def options(opt):
    pass

def configure(cfg):
    cfg.env.CXXFLAGS_RANT = ['-std=c++0x',]

def build(bld):
    bld(
        target          = 'rant',
        export_includes = '.',
        use             = ['RANT'],
    )
