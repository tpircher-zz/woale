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


# Generate a header file based on the current CMake settings. All local
# variables and all CACHE'd variables can be referenced from the input file.
configure_file(
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h")

# Add the source path to the compiler include path.
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")


# Find libraries
#find_package(FFmpeg REQUIRED COMPONENTS avcodec avutil)


# Add all subdirectories
add_subdirectory(cli)
add_subdirectory(doc)
