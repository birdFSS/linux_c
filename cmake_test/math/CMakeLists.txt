cmake_minimum_required (VERSION 2.8)

# projectname is the same as the main-executable
project(math)

add_definitions('-g')
add_definitions('-Wall')
include_directories(.)

add_library(${PROJECT_NAME} add.c)
target_include_directories(math INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

