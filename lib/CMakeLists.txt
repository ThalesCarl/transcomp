# Project name
project (lib)

# Add a directory to teh list of directories where to look for header files
include_directories (${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/include)

# Locate all library '.cpp' and '.h' files
file (GLOB ${PROJECT_NAME}_headers ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/include/*.h)
file (GLOB ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.cpp)

# Command to create a library using the library '.cpp' and '.h' files
add_library (${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources} ${${PROJECT_NAME}_headers})