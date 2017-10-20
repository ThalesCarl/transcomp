# Project name
project (libTest)

# Add a directory to teh list of directories where to look for header files
include_directories (${CMAKE_SOURCE_DIR}/lib/include)
include_directories (${CMAKE_SOURCE_DIR}/UtilsLib/include)

# Locate all library '.cpp' and '.h' files
file (GLOB ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

# Command to create a app using the '.cpp' file (where 'main' is located)
add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_sources})

#target_link_libraries(${PROJECT_NAME} UtilsLib)
target_link_libraries(${PROJECT_NAME} lib)
