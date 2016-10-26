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


# Add your global cmake CACHE's configurations. You can mark the settings as
# advanced, if appropriate.


# Use an include guard to avoid processing this file multiple times. This will
# help speeding up the cmake stage.
if(local_cmake_config_included)
    return()
endif()
set(local_cmake_config_included true)


# Set default compiler flags.
set(cxx_warnings "-W -Wall -Werror -Wextra -Wstrict-aliasing -Weffc++ -std=c++14")
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
set(cxx_flags "-ftree-vectorize -ftree-vectorizer-verbose=2 -ffast-math -fstrict-aliasing")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
set(cxx_flags "-ftree-vectorize -ffast-math -fstrict-aliasing")
endif()
set(common_cxx_flags "${cxx_warnings} ${cxx_flags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${common_cxx_flags}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${common_cxx_flags}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${common_cxx_flags}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${common_cxx_flags}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${common_cxx_flags}")
