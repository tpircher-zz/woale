# Copyright (C) 2015-2016  Thomas Pircher <tehpeh@gmx.net>
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



# FindCgic
# --------
#
# Find libgit include dirs and libraries.
#
# Use this module by invoking find_package with the form:
#
#   find_package(Cgic
#     [REQUIRED]             # Fail with error if the library is not found
#     )
#
# This module finds headers and requested component libraries.  Results are
# reported in variables:
#
#   Cgic_FOUND            - True if headers and requested libraries were found
#   Cgic_INCLUDE_DIR      - include directories
#   Cgic_LIBRARY          - libgit2 component libraries to be linked
#

# Use pkg-config (if available) to find the directories of the componants.
find_package(PkgConfig QUIET)

if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_CGIC QUIET "libcgic")
    set(CGIC_DEFINITIONS ${PC_CGIC_CFLAGS_OTHER})
endif()

find_path(CGIC_INCLUDE_DIR
    NAMES cgic.h
    HINTS ${PC_CGIC_INCLUDEDIR} ${PC_CGIC_INCLUDE_DIRS}
    PATH_SUFFIXES git2)

find_library(CGIC_LIBRARY
    NAMES cgic
    HINTS ${PC_CGIC_LIBDIR} ${PC_CGIC_LIBRARY_DIRS})


set(CGIC_LIBRARIES ${CGIC_LIBRARY} )
set(CGIC_INCLUDE_DIRS ${CGIC_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Cgic DEFAULT_MSG
                                  CGIC_LIBRARY CGIC_INCLUDE_DIR)

mark_as_advanced(Cgic_FOUND CGIC_INCLUDE_DIR CGIC_LIBRARY)
