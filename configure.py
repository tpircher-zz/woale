#!/usr/bin/env python
from __future__ import print_function
import argparse
import sys
import os
import subprocess

progname = os.path.basename(__file__)
default_sysconfdir = 'etc'
default_localstatedir = 'var'
default_libdir = 'lib'
default_datarootdir = 'share'

parser = argparse.ArgumentParser(description='configures this build to adapt to many kinds of systems.')
parser.add_argument('--srcdir', dest='srcdir', metavar='DIR', default=os.path.dirname(__file__),
                   help='find the sources in DIR')
parser.add_argument('--prefix', dest='prefix', metavar='DIR',
                   help='install architecture-independent files in DIR')
#parser.add_argument('--sysconfdir', dest='sysconfdir', metavar='DIR',
#                   help='read-only configuration data [PREFIX/{:s}]'.format(default_sysconfdir))
#parser.add_argument('--localstatedir', dest='localstatedir', metavar='DIR',
#                   help='modifiable single-machine data [PREFIX/{:s}]'.format(default_localstatedir))
#parser.add_argument('--libdir', dest='libdir', metavar='DIR',
#                   help='object code libraries [PREFIX/{:s}]'.format(default_libdir))
#parser.add_argument('--datarootdir', dest='datarootdir', metavar='DIR',
#                   help='read-only arch.-independent data root [PREFIX/{:s}]'.format(default_datarootdir))
parser.add_argument('--disable-marked', dest='marked', default=True, action='store_false',
                   help='don\'t install the marked library')


args = parser.parse_args()
args.srcdir = os.path.abspath(args.srcdir)


def conf_error(error_str):
    print('{0:s}: error: {1:s}'.format(progname, error_str), file=sys.stderr)
    sys.exit(1)


builddir = 'build'
if not os.path.exists(builddir):
    os.makedirs(builddir)

cmake_cmd = ['cmake', args.srcdir, '-DCMAKE_BUILD_TYPE=RELEASE']

if args.prefix is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_PREFIX:PATH={0:s}'.format(os.path.abspath(args.prefix)))
#if args.sysconfdir is not None:
#    cmake_cmd.append('-DINSTALL_SYSCONFIG_DIR:PATH={0:s}'.format(args.sysconfdir))
#if args.localstatedir is not None:
#    cmake_cmd.append('-DINSTALL_LOCALSTATE_DIR:PATH={0:s}'.format(args.localstatedir))
#if args.libdir is not None:
#    cmake_cmd.append('-DINSTALL_LIB_DIR:PATH={0:s}'.format(args.libdir))
#if args.datarootdir is not None:
#    cmake_cmd.append('-DINSTALL_DATAROOT_DIR:PATH={0:s}'.format(args.datarootdir))
if not args.marked:
    cmake_cmd.append('-DINSTALL_MARKED_JS:BOOL=NO')
print(' '.join(cmake_cmd))
cmd = subprocess.Popen(cmake_cmd, cwd=builddir)
cmd.wait()

if cmd.returncode == 0:
    print('\nConfiguration successful.')
    print('Now cd to the directory "{0:s}" and type "make"'.format(builddir))
