#!/bin/bash

##### Load auxiliar functions #####
. $ROOT_SCRIPTS_DIR/auxiliar_functions.sh

##### Settings #####
LIBRARY="cmake-3.8.2"
LIBRARY_BUILD_DIR=$ROOT_BUILD_DIR/$LIBRARY
LIBRARY_INSTALL_DIR=$ROOT_INSTALL_DIR/$LIBRARY
DOWNLOAD_LINK="https://cmake.org/files/v3.8/cmake-3.8.2.tar.gz"

##### Build fuction #####

build_cmake()
{

	define_default_derived_variables

	prepare_to_build_and_install

	get_library_as_source_dir

	message "Moving to source dir"
	run_command cd $SOURCE_DIR

	message  "Running configure"
	run_command ./bootstrap --parallel=$NUMBER_OF_CORES --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR 

	message  "Building"
	run_command make -j $NUMBER_OF_CORES

	message  "Installing"
	run_command make install

	display_end_message
}

##### Build library #####

BUILD_TYPE=""
build_cmake

##### Export name #####
export_name CMAKE_DIR $LIBRARY_INSTALL_DIR
CMAKE_DIR=$LIBRARY_INSTALL_DIR
export_name PATH "$CMAKE_DIR/bin:\$PATH"
