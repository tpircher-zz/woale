# Copyright (C) 2015-2016  Thomas Pircher <tehpeh@gmx.net>
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



# FindLibGit2
# -----------
#
# Find libgit include dirs and libraries.
#
# Use this module by invoking find_package with the form:
#
#   find_package(LibGit2
#     [REQUIRED]             # Fail with error if the library is not found
#     )
#
# This module finds headers and requested component libraries.  Results are
# reported in variables:
#
#   LibGit2_FOUND            - True if headers and requested libraries were found
#   LibGit2_INCLUDE_DIR      - include directories
#   LibGit2_LIBRARY          - libgit2 component libraries to be linked
#

# Use pkg-config (if available) to find the directories of the componants.
find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIBGIT2 QUIET "libgit2")
    set(LIBGIT2_DEFINITIONS ${PC_LIBGIT2_CFLAGS_OTHER})
endif()

find_path(LIBGIT2_INCLUDE_DIR
    NAMES git2.h
    HINTS ${PC_LIBGIT2_INCLUDEDIR} ${PC_LIBGIT2_INCLUDE_DIRS}
    PATH_SUFFIXES git2)

find_library(LIBGIT2_LIBRARY
    NAMES git2
    HINTS ${PC_LIBGIT2_LIBDIR} ${PC_LIBGIT2_LIBRARY_DIRS})


set(LIBGIT2_LIBRARIES ${LIBGIT2_LIBRARY} )
set(LIBGIT2_INCLUDE_DIRS ${LIBGIT2_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibGit2 DEFAULT_MSG
                                  LIBGIT2_LIBRARY LIBGIT2_INCLUDE_DIR)

mark_as_advanced(LibGit2_FOUND LIBGIT2_INCLUDE_DIR LIBGIT2_LIBRARY)
