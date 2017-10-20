# Project name
project (app)

# Add a directory to teh list of directories where to look for header files
include_directories (${CMAKE_SOURCE_DIR}/lib/include)

# Locate all app '.cpp' files
file (GLOB ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

# Command to create a app using the '.cpp' file (where 'main' is located)
add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_sources})

# Instruct the compiler to link app against a library named 'lib'
target_link_libraries (${PROJECT_NAME} lib)