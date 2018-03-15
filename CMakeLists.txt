CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

##################################################################
# BUILD FILES OUTPUT LOCATION
##################################################################
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/static_libs)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_libs)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##################################################################
# OPERATIONAL SYSTEMS CONFIGURATION
##################################################################
if (UNIX)
	message ("\n-- UNIX has been detected.")
	set (CMAKE_C_COMPILER "mpicc")
	set (CMAKE_CXX_COMPILER "mpicxx")

	set (CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-m64 -pedantic -std=c++14" CACHE STRING "Setting default compiler flags.")
	set (CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG} "-g" CACHE STRING "Setting default compiler flags.")
	set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-O3 -DNDEBUG" CACHE STRING "Setting default compiler flags")

	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set (BUILD_TYPE_OUTPUT_DIRECTORY "Debug")
	elseif (CMAKE_BUILD_TYPE MATCHES "Release")
		set (BUILD_TYPE_OUTPUT_DIRECTORY "Release")
	else ()
		message ("\n-- CMAKE_BUILD_TYPE must be set to Debug or Release.\n-- It will be set to Release.\n")
		set (CMAKE_BUILD_TYPE "Release")
		set (BUILD_TYPE_OUTPUT_DIRECTORY "Release")
	endif ()

	set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
	set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
	set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BUILD_TYPE_OUTPUT_DIRECTORY})
else ()
	message ("\n-- CMAKE HAS NOT BEEN CONFIGURED FOR THE CURRENT OPERATIONAL SYSTEM.\n")
endif ()

##################################################################
# ABOUT THE PROJECT
##################################################################
project (plain_wall)

##################################################################
# EXTERNAL LIBRARIES
##################################################################

# USER MODULES LOCATION
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

##############
# BOOST
##############
set (Components_Boost chrono context filesystem graph_parallel log mpi program_options regex serialization signals system thread timer wave date_time graph math_c99 random test_exec_monitor exception unit_test_framework serialization)
find_package (Boost COMPONENTS ${Components_Boost} REQUIRED)
if (Boost_FOUND)
    include_directories (${Boost_INCLUDE_DIRS})
endif ()

##############
# PETSC
##############
set (PETSC_VERSION "3.7.6" )
set (PETSC_DIR $ENV{PETSC_DIR}/${BUILD_TYPE_OUTPUT_DIRECTORY})
set (PETSC_ARCH)
find_package (PETSc ${PETSC_VERSION} REQUIRED)
if (PETSC_FOUND)
	include_directories (${PETSC_INCLUDES})
endif ()

##############
# MACROS
##############
macro (add_executable _target)
    _add_executable (${_target} ${ARGN})
    target_link_libraries (${_target} ${Boost_LIBRARIES}) 
    target_link_libraries (${_target} ${PETSC_LIBRARIES})
endmacro ()

macro (add_library _target)
    _add_library (${_target} ${ARGN})
    target_link_libraries (${_target} ${Boost_LIBRARIES}) 
    target_link_libraries (${_target} ${PETSC_LIBRARIES})
endmacro ()

##################################################################
# SUB PROJECTS
##################################################################
#add_subdirectory (UtilsLib)
add_subdirectory (lib)
add_subdirectory (libTest)
add_subdirectory (app_first_exercise)
add_subdirectory (app_second_exercise)
add_subdirectory (app_trabalho)



##################################################################
# DISPLAY INFORMATION
##################################################################
message ("\n-- Project: ${PROJECT_NAME} ${VERSION}")
message ("-- Build type: ${BUILD_TYPE_OUTPUT_DIRECTORY}")
message ("-- Install prefix: ${CMAKE_INSTALL_PREFIX}\n")
message ("-- C++ compiler: ${CMAKE_CXX_COMPILER}")
message ("-- Debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
message ("-- Release flags: ${CMAKE_CXX_FLAGS_RELEASE}\n")
