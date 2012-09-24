#!/usr/bin/env python
import sys, os

def options(opt):
    pass

def configure(cfg):
    cfg.env.CXXFLAGS_RANGER = ['-std=c++0x',]

def build(bld):
    bld(
        target          = 'ranger',
        export_includes = '.',
        use             = ['RANGER'],
    )
