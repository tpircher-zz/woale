# Add your global cmake CACHE's configurations. You can mark the settings as
# advancedm if appropriate.


# Use an include guard to avoid processing this file multiple times. This will
# help speeding up the cmake stage.
if(local_cmake_config_included)
    return()
endif()
set(local_cmake_config_included true)


# Set default compiler flags.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -W -Wall -Werror -std=c11")
