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


# This is the main CMakeLists.txt file.
#

# Require a minimum cmake version.
cmake_minimum_required(VERSION 3.0.0)

# Disallow in-source builds, i.e. require out-of-source builds.
set(CMAKE_DISABLE_SOURCE_CHANGES true)
set(CMAKE_DISABLE_IN_SOURCE_BUILD true)

# Add the custom cmake folder.
# This needs to be called *before* project() is called.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


# The name of the project
project(woale VERSION 0.0.0 LANGUAGES "C")

# Include some helper scripts. These scripts will also be included (and
# defined) in subdirectories.
include(LocalCmakeConfig)


# Add all subdirectories
add_subdirectory("src")
add_subdirectory("data")
add_subdirectory("etc")
add_subdirectory("doc")
