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



# FindSqlite
# ---------
#
# Find libcgicc include dirs and libraries.
#
# Use this module by invoking find_package with the form:
#
#   find_package(Sqlite
#     [REQUIRED]             # Fail with error if the library is not found
#     )
#
# This module finds headers and requested component libraries.  Results are
# reported in variables:
#
#   Sqlite_FOUND            - True if headers and requested libraries were found
#   Sqlite_INCLUDE_DIRS     - include directories
#   Sqlite_LIBRARIES        - libcgicc component libraries to be linked
#

# Use pkg-config (if available) to find the directories of the componants.
find_package(PkgConfig QUIET)
pkg_check_modules(PKG_SQLITE QUIET "libcgicc")
set(SQLITE_DEFINITIONS ${PKG_SQLITE_CFLAGS_OTHER})

find_path(SQLITE_INCLUDE_DIR
        NAMES sqlite3.h
        HINTS ${PKG_SQLITE_INCLUDEDIR} ${PKG_SQLITE_INCLUDE_DIRS}
        PATH_SUFFIXES cgicc)

find_library(SQLITE_LIBRARY
        NAMES sqlite3
        HINTS ${PKG_SQLITE_LIBDIR} ${PKG_SQLITE_LIBRARY_DIRS})


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Sqlite DEFAULT_MSG
        SQLITE_LIBRARY SQLITE_INCLUDE_DIR)

mark_as_advanced(Sqlite_FOUND SQLITE_INCLUDE_DIR SQLITE_LIBRARY)

set(SQLITE_LIBRARIES ${SQLITE_LIBRARY})
set(SQLITE_INCLUDE_DIRS ${SQLITE_INCLUDE_DIR})
