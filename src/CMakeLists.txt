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


# Generate a header file based on the current CMake settings. All local
# variables and all CACHE'd variables can be referenced from the input file.
configure_file("config.h.in" "config.h")

# Add the source path to the compiler include path.
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# Find libraries
#find_package(LibGit2 REQUIRED)
find_package(Cgic REQUIRED)


set(target woale.cgi)
add_executable(${target} main.c fileparser.c)
#include_directories(${target} ${LIBGIT2_INCLUDE_DIR} ${CGIC_INCLUDE_DIR})
#target_link_libraries(${target} ${LIBGIT2_LIBRARY} ${CGIC_LIBRARY})
include_directories(${target} ${CGIC_INCLUDE_DIR})
target_link_libraries(${target} ${CGIC_LIBRARY})


install(TARGETS "${target}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_LIBDIR}/woale")

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/woale")
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/woale" DESTINATION "${CMAKE_INSTALL_LOCALSTATEDIR}/lib"
        DIRECTORY_PERMISSIONS
            OWNER_WRITE OWNER_READ OWNER_EXECUTE
            GROUP_WRITE GROUP_READ GROUP_EXECUTE
                        WORLD_READ WORLD_EXECUTE)
