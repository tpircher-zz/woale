# Generate a header file based on the current CMake settings. All local
# variables and all CACHE'd variables can be referenced from the input file.
configure_file("config.h.in" "config.h")

# Add the source path to the compiler include path.
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


# Find libraries
find_package(LibGit2 REQUIRED)
find_package(Cgic REQUIRED)


set(target woale.cgi)
add_executable(${target} main.c)
#include_directories(${target} ${LIBGIT2_INCLUDE_DIR} ${CGIC_INCLUDE_DIR})
#target_link_libraries(${target} ${LIBGIT2_LIBRARY} ${CGIC_LIBRARY})
include_directories(${target} ${CGIC_INCLUDE_DIR})
target_link_libraries(${target} ${CGIC_LIBRARY})


install(TARGETS ${target}
        RUNTIME DESTINATION "bin")

install(FILES "${CMAKE_SOURCE_DIR}/etc/woale.conf"
        DESTINATION ${INSTALL_SYSCONFIG_DIR})
