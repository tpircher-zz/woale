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


# Set install directories
if ("${INSTALL_SYSCONFIG_DIR}" STREQUAL "")
    get_filename_component(INSTALL_SYSCONFIG_DIR "${CMAKE_INSTALL_PREFIX}/etc" ABSOLUTE)
else()
    get_filename_component(INSTALL_SYSCONFIG_DIR "${INSTALL_SYSCONFIG_DIR}" ABSOLUTE)
endif()
