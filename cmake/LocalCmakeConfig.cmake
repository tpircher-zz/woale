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


# Add your global cmake CACHE's configurations. You can mark the settings as
# advanced, if appropriate.


# Use an include guard to avoid processing this file multiple times. This will
# help speeding up the cmake stage.
if(local_cmake_config_included)
    return()
endif()
set(local_cmake_config_included true)


# Set default compiler flags.
set(common_c_flags "-W -Wall -Werror -std=c11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${common_c_flags}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${common_c_flags}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${common_c_flags}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${common_c_flags}")
set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} ${common_c_flags}")


#macro(set_install_path var suffix)
#    if ("${${var}}" STREQUAL "")
#        get_filename_component(${var} "${CMAKE_INSTALL_PREFIX}/${suffix}" ABSOLUTE)
#    else()
#        get_filename_component(${var} "${${var}}" ABSOLUTE)
#    endif()
#endmacro()
#
## Set install directories
#set_install_path(INSTALL_SYSCONFIG_DIR "etc")
#set_install_path(INSTALL_LOCALSTATE_DIR "var")
#set_install_path(INSTALL_DATAROOT_DIR "share")

set(INSTALL_SYSCONFIG_DIR "etc" CACHE PATH "read-only configuration data")
set(INSTALL_LOCALSTATE_DIR "var" CACHE PATH "modifiable single-machine data")
set(INSTALL_LIB_DIR "usr/lib" CACHE PATH "object code libraries")
set(INSTALL_DATAROOT_DIR "usr/share" CACHE PATH "read-only arch.-independent data root")