cmake_minimum_required (VERSION 2.8)

# projectname is the same as the main-executable
project(cmake_test)
add_subdirectory(math)

add_definitions('-g')
add_definitions('-Wall')
#add_definitions('-std=c++11')

aux_source_directory(src DIR_SRC)
add_executable(${PROJECT_NAME} ${DIR_SRC})
target_link_libraries(${PROJECT_NAME} math)
