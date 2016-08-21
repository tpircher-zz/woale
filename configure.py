#!/usr/bin/env python

# Copyright (C) 2015-2016  Thomas Pircher <tehpeh-web@tty1.net>
#
# woale -- a wiki engine.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from __future__ import print_function
import argparse
import sys
import os
import subprocess

progname = os.path.basename(__file__)

parser = argparse.ArgumentParser(description='configures this build to adapt to many kinds of systems.')
parser.add_argument('--srcdir', dest='srcdir', metavar='DIR', default=os.path.dirname(__file__),
                   help='find the sources in DIR.')
parser.add_argument('--prefix', dest='prefix', metavar='DIR',
                   help='install files in DIR.')
parser.add_argument('--bindir', dest='bindir', metavar='DIR',
                   help='install executable program files files in DIR.')
parser.add_argument('--datadir', dest='datadir', metavar='DIR',
                   help='install read-only architecture-independent data files in DIR.')
parser.add_argument('--sysconfdir', dest='sysconfdir', metavar='DIR',
                   help='install read-only configuration files in DIR.')
parser.add_argument('--localstatedir', dest='localstatedir', metavar='DIR',
                   help='install data files which the programs modify while they run in DIR.')
parser.add_argument('--libdir', dest='libdir', metavar='DIR',
                   help='install object files and libraries of object code in DIR.')
parser.add_argument('--docdir', dest='docdir', metavar='DIR',
                   help='install documentation files for this package in DIR.')


args = parser.parse_args()
args.srcdir = os.path.abspath(args.srcdir)


def conf_error(error_str):
    print('{0:s}: error: {1:s}'.format(progname, error_str), file=sys.stderr)
    sys.exit(1)


builddir = 'build'
if not os.path.exists(builddir):
    os.makedirs(builddir)

cmake_cmd = ['cmake', args.srcdir, '-DCMAKE_BUILD_TYPE=Release']

if args.prefix is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_PREFIX={0:s}'.format(os.path.abspath(args.prefix)))
if args.bindir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_BINDIR={0:s}'.format(os.path.abspath(args.bindir)))
if args.datadir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_DATADIR={0:s}'.format(os.path.abspath(args.datadir)))
if args.sysconfdir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_SYSCONFDIR={0:s}'.format(os.path.abspath(args.sysconfdir)))
if args.localstatedir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_LOCALSTATEDIR={0:s}'.format(os.path.abspath(args.localstatedir)))
if args.libdir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_LIBDIR={0:s}'.format(os.path.abspath(args.libdir)))
if args.docdir is not None:
    cmake_cmd.append('-DCMAKE_INSTALL_DOCDIR={0:s}'.format(os.path.abspath(args.docdir)))



print(' '.join(cmake_cmd))
cmd = subprocess.Popen(cmake_cmd, cwd=builddir)
cmd.wait()

if cmd.returncode == 0:
    print('\nConfiguration successful.')
    print('Now cd to the directory "{0:s}" and type "make"'.format(builddir))
