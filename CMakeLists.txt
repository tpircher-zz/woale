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
add_subdirectory("doc")
