#!/bin/bash

##### Load auxiliar functions #####
. $ROOT_SCRIPTS_DIR/auxiliar_functions.sh

##### Settings #####
LIBRARY="openmpi-2.1.0"
LIBRARY_BUILD_DIR=$ROOT_BUILD_DIR/$LIBRARY
LIBRARY_INSTALL_DIR=$ROOT_INSTALL_DIR/$LIBRARY
CONFIGURE_FLAGS="--enable-binaries=yes --enable-static=yes --enable-cxx-exceptions=yes"
DOWNLOAD_LINK="https://www.open-mpi.org/software/ompi/v2.1/downloads/openmpi-2.1.0.tar.gz"

##### Build fuction #####

build_openmpi()
{

	define_default_derived_variables

	prepare_to_build_and_install

	get_library_as_source_dir

	message "Moving to source dir"
	run_command cd $SOURCE_DIR

	message  "Running configure"
	run_command ./configure $CONFIGURE_FLAGS --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR

	message  "Building"
	run_command make -j $NUMBER_OF_CORES all

	message  "Installing"
	run_command make install

	display_end_message
}

##### Build library #####

# Build
BUILD_TYPE=""
build_openmpi

##### Export name #####
export_name OPENMPI_DIR $LIBRARY_INSTALL_DIR
OPENMPI_DIR=$LIBRARY_INSTALL_DIR
export_name PATH "$OPENMPI_DIR/bin:$OPENMPI_DIR/lib:$OPENMPI_DIR/include:\$PATH"
