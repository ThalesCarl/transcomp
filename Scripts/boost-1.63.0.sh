#!/bin/bash

##### Load auxiliar functions #####
. $SCRIPTS_DIR/auxiliar_functions.sh

##### Settings #####
LIBRARY="boost-1.63.0"
LIBRARY_BUILD_DIR=$ROOT_BUILD_DIR/$LIBRARY
LIBRARY_INSTALL_DIR=$ROOT_INSTALL_DIR/$LIBRARY
CONFIGURE_FLAGS="link=static runtime-link=static threading=single --layout=versioned"

# System specific setting
OS_NAME=`uname -a`
if contains "$OS_NAME" "Debian" || contains "$OS_NAME" "Ubuntu"
	then
	PYTHON_CONFIGURE_FLAGS="--with-python=/usr/bin/python3 -with-python-root=/usr"
elif contains "$OS_NAME" "ARCH" || contains "$OS_NAME" "MANJARO"
	then
	PYTHON_CONFIGURE_FLAGS="--with-python=/usr/bin/python2.7 -with-python-root=/usr --with-python-version=2.7"
elif contains "$OS_NAME" "CYGWIN"
	then
	CONFIGURE_FLAGS="$CONFIGURE_FLAGS define=BOOST_LOG_WITHOUT_IPC"
	# prevent error in cygwin
	PYTHON_CONFIGURE_FLAGS=""
fi
DOWNLOAD_LINK="https://downloads.sourceforge.net/project/boost/boost/1.63.0/boost_1_63_0.tar.gz"

##### Build fuction #####

build_boost()
{

	define_default_derived_variables

	prepare_to_build_and_install

	get_library_as_source_dir
	mv $LIBRARY_BUILD_TYPE_BUILD_DIR/boost_1_63_0 $SOURCE_DIR
	# correct extracted lib name. If it has already been
	# changed, ignore the command.

	message "Moving to source dir"
	run_command cd $SOURCE_DIR

	message  "Running configure"
	run_command ./bootstrap.sh $PYTHON_CONFIGURE_FLAGS --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR

	message  "Building and installing"
	VARIANT=`echo ${BUILD_TYPE,,}` # all characters in lowercase
	run_command ./b2 variant=$VARIANT $CONFIGURE_FLAGS --prefix=$LIBRARY_BUILD_TYPE_INSTALL_DIR -j$NUMBER_OF_CORES install

	display_end_message
}

##### Build library #####

# Release
BUILD_TYPE="Release"
build_boost

# Debug
BUILD_TYPE="Debug"
build_boost

##### Export name #####
export_name BOOST_DIR $LIBRARY_INSTALL_DIR
