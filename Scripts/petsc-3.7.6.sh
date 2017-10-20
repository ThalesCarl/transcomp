#!/bin/bash

##### Load auxiliar functions #####
. $SCRIPTS_DIR/auxiliar_functions.sh

##### Settings #####
LIBRARY="petsc-3.7.6"
LIBRARY_BUILD_DIR=$ROOT_BUILD_DIR/$LIBRARY
LIBRARY_INSTALL_DIR=$ROOT_INSTALL_DIR/$LIBRARY
CONFIGURE_FLAGS="--with-mpi=0 --with-shared-libraries=0 --download-fblaslapack=yes"
CONFIGURE_FLAGS_DEBUG="$CONFIGURE_FLAGS --with-debugging=1"
CONFIGURE_FLAGS_RELEASE="$CONFIGURE_FLAGS --with-debugging=0"
DOWNLOAD_LINK="http://ftp.mcs.anl.gov/pub/petsc/release-snapshots/petsc-lite-3.7.6.tar.gz"

##### Build fuction #####

build_petsc()
{

	define_default_derived_variables

	prepare_to_build_and_install

	get_library_as_source_dir

	message "Moving to source dir"
	run_command cd $SOURCE_DIR

	export PETSC_ARCH=$BUILD_TYPE
	export PETSC_DIR=$SOURCE_DIR

	message  "Running configure"

	OS_NAME=`uname -a`
	if contains "$OS_NAME" "ARCH" || contains "$OS_NAME" "MANJARO"
	then
		run_command python2 ./configure --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR $CONFIGURE_FLAGS
	else
		run_command ./configure --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR $CONFIGURE_FLAGS
	fi

	message  "Building"
	USE_NUMBER_OF_CORES=`expr $NUMBER_OF_CORES - 1`
	USE_NUMBER_OF_CORES=$(( 1 < $USE_NUMBER_OF_CORES ? $USE_NUMBER_OF_CORES : 1 )) # number greater or equal to 1
	run_command make MAKE_NP=$USE_NUMBER_OF_CORES all

	message  "Installing"
	run_command make install

	message  "Testing"
	run_command make test

	display_end_message
}

##### Build library #####

# Debug
BUILD_TYPE="Debug"
CONFIGURE_FLAGS=$CONFIGURE_FLAGS_DEBUG
build_petsc

# Release
BUILD_TYPE="Release"
CONFIGURE_FLAGS="$CONFIGURE_FLAGS_RELEASE COPTFLAGS=-O3 CXXOPTFLAGS=-O3 FOPTFLAGS=-O3 "
build_petsc

##### Export name #####
export_name PETSC_DIR $LIBRARY_INSTALL_DIR
