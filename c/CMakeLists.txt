cmake_minimum_required(VERSION 3.10)
project(cortex)

# Build shared library
add_library(cortex SHARED
    libcortex.c
)

# Output name
set_target_properties(cortex PROPERTIES
    OUTPUT_NAME "libcortex"
    PREFIX ""
)

# Include current directory
target_include_directories(cortex PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})