#!/usr/bin/env python
from __future__ import print_function
import argparse
import sys
import os
import subprocess

progname = os.path.basename(__file__)

parser = argparse.ArgumentParser(description='configures this build to adapt to many kinds of systems.')
#parser.add_argument('-q', '--quiet', dest='quiel', action='store_true',
#                   help='do not print "checking ..." messages')
parser.add_argument('--prefix', dest='prefix', metavar='DIR',
                   help='install architecture-independent files in DIR')
parser.add_argument('--srcdir', dest='srcdir', metavar='DIR', default=os.path.dirname(__file__),
                   help='find the sources in DIR')
parser.add_argument('--sysconfdir', dest='sysconfdir', metavar='DIR',
                   help='read-only configuration data')

args = parser.parse_args()
args.srcdir = os.path.abspath(args.srcdir)


def conf_error(error_str):
    print('{0:s}: error: {1:s}'.format(progname, error_str), file=sys.stderr)
    sys.exit(1)


builddir = 'build'
if os.path.exists(builddir):
    conf_error('build directory "{0:s}" exists'.format(builddir))

os.makedirs(builddir)
cmake_cmd = ['cmake', args.srcdir, '-DCMAKE_BUILD_TYPE=RELEASE']

if args.prefix is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_PREFIX:PATH={0:s}'.format(os.path.abspath(args.prefix)))
if args.sysconfdir is not None:
    cmake_cmd.append('-DINSTALL_SYSCONFIG_DIR:PATH={0:s}'.format(args.sysconfdir))
print(' '.join(cmake_cmd))
cmd = subprocess.Popen(cmake_cmd, cwd=builddir)
cmd.wait()

if cmd.returncode == 0:
    print('\nConfiguration successful.')
    print('Now cd to the directory "{0:s}" and type "make"'.format(builddir))
