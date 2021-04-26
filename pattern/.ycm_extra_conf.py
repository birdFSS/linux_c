#! /usr/bin/env python3
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2021 birdfss <ffhbird@gmail.com>
#
# Distributed under terms of the MIT license.

"""

"""
from distutils.sysconfig import get_python_inc
import platform
import os
import subprocess
import ycm_core

DIR_OF_THIS_SCRIPT = os.path.abspath( os.path.dirname( __file__ ) )
DIR_OF_THIRD_PARTY = os.path.join( DIR_OF_THIS_SCRIPT, 'third_party' )
SOURCE_EXTENSIONS = ['.c', '.cpp', '.cxx', '.cc']

flags = [
    '-Wall',
    '-Werror',
    '-std=c99',
    '-Wextra',
    '-x',
    'c',
    '-I',
    "math"
]

def FlagsForFile(filename, **kwargs):
    return {
        'flags': flags,
        'do_cache': True
    }
