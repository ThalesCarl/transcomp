#!/bin/sh

############### DEFINE AUXILIAR FUNCTIONS ###############

##############
# MESSAGE
##############

message()
{
	# $1 - first argument : message to be displayed
	echo -e "\n$1\n" | tee -a $LOG_FILE
}

##############
# RUN COMMAND
##############

display_command_to_be_executed()
{
	# $1 : command to be executed
	echo -e "\n`pwd`\$ $1\n" >> $LOG_FILE
}

display_execution_error_message()
{
	# $1 : command that returned error
	message "ERROR"
	message "`pwd`\$ $1 \n"
	echo -e "See program error message in $LOG_FILE"
}

verify_error_in_command()
{
	# $1 : return code
	# $2 : command executed
	if [ $1 != 0 ]
	then
		display_execution_error_message $2
		exit 1
	fi
}

run_command()
{
	# $@ - all arguments passed to function.
	# 		In this case, the command to be
	#		executed.
	display_command_to_be_executed $@
	$@ 2>>$LOG_FILE >> $LOG_FILE #execute command
	verify_error_in_command $? $@
}

##############
# SYSTEM IDENTIFICATION
##############

contains() {
	string="$1"
	substring="$2"
	if test "${string#*$substring}" != "$string"
	then
		return 0    # $substring is in $string
	else
		return 1    # $substring is not in $string
	fi
}

# Get system number os processor cores
export NUMBER_OF_CORES=`nproc`

OPERATING_SYSTEM_NAME=`uname -a`

LINUX=false
DEBIAN=false
UBUNTU=false
ARCH=false
MANJARO=false
CYGWIN=false

# Detect if is a linux system
if contains "$OPERATING_SYSTEM_NAME" "Linux"
then
	LINUX=true
fi

# Detect linux system type
if contains "$OPERATING_SYSTEM_NAME" "Debian"
then
	DEBIAN=true
elif contains "$OPERATING_SYSTEM_NAME" "Ubuntu"
then
	UBUNTU=true
elif contains "$OPERATING_SYSTEM_NAME" "ARCH"
then
	ARCH=true
elif contains "$OPERATING_SYSTEM_NAME" "MANJARO"
then
	MANJARO=true
elif contains "$OPERATING_SYSTEM_NAME" "CYGWIN"
then
	CYGWIN=true
else
	message "Script not tested in your system!"
fi


##############
# AUTO EXECUTION
##############

auto_install_package()
{
	# $1 : first argument, package name
	echo -e "\n\n------------------ $1 ------------------\n\n"
	. $EXTERNAL_PACKAGES_ROOT_SCRIPTS_DIR/${1}.sh
	cd $ROOT_SCRIPTS_DIR # Avoid location error
}

auto_build_library()
{
	# $1 : first argument, lib name
	echo -e "\n\n------------------ $1 ------------------\n\n"
	. $LIBRARIES_ROOT_SCRIPTS_DIR/${1}.sh
	cd $ROOT_SCRIPTS_DIR # Avoid location error
}

auto_build_sinmec_library()
{
	# $1 : first argument, lib name
	echo -e "\n\n------------------ $1 ------------------\n\n"
	. $SINMEC_LIBRARIES_ROOT_SCRIPTS_DIR/${1}.sh
	cd $ROOT_SCRIPTS_DIR # Avoid location error
}

##############
# SYSTEM PACKAGES INSTALLATION
##############
install_system_packages()
{
	if $DEBIAN
	then
		auto_install_package debian_packages
	elif $UBUNTU
	then
		auto_install_package debian_packages
	elif $ARCH
	then
		auto_install_package arch_packages
	elif $MANJARO
	then
		auto_install_package arch_packages
	elif $CYGWIN
	then
		auto_install_package cygwin_packages
	fi
}

##############
# DOWNLOAD LIBRARY
##############

exist_library()
{
if test -f $COMPRESSED_LIBRARY
	then
		return 0
	else
		return 1
	fi
}

get_library_online()
{
	message "Downloading $LIBRARY"
	run_command wget $DOWNLOAD_LINK -O $COMPRESSED_LIBRARY
}

download_library()
{
	if ! exist_library
	then
		get_library_online
	fi
}

##############
# EXTRACT LIBRARY
##############

select_extraction_method()
{
	if [[ $1 == *.tar.gz ]]
	then
		DECOMPRESS_ARGUMENT="z"
	elif [[ $1 == *.tar.xz ]]
	then
		DECOMPRESS_ARGUMENT="J"
	elif [[ $1 == *.tar.bz2 ]]
	then
		DECOMPRESS_ARGUMENT="j"
	else
		message "Unknown method to decompress: $1"
		exit 1
	fi

	DECOMPRESS_ARGUMENT="-x -$DECOMPRESS_ARGUMENT -f"
	# example: -x -z -f FILE_NAME
}

exist_decompressed_library()
{
	if test -f $SOURCE_DIR
	then
		return 0
	else
		return 1
	fi
}

extract_library_as_source_dir()
{
	# do not use 'run_command'
	message "Extracting $COMPRESSED_LIBRARY"
	select_extraction_method $COMPRESSED_LIBRARY
	tar $DECOMPRESS_ARGUMENT $COMPRESSED_LIBRARY -C $LIBRARY_BUILD_TYPE_BUILD_DIR 2>>$LOG_FILE >> $LOG_FILE
}

extract_library()
{
	if ! exist_decompressed_library
	then
		extract_library_as_source_dir
	fi
}

##############
# GET LIBRARY
##############

extraction_fails()
{
	if [ $? == 0 ]
	then
		return 1
	else
		return 0
	fi
}

clean_library_build_type_build_dir()
{
	rm -R $COMPRESSED_LIBRARY
	rm -R $SOURCE_DIR
}

get_library_as_source_dir()
{
	download_library
	extract_library

	if extraction_fails
	then
		message "Extraction failed, retrying ..."
		clean_library_build_type_build_dir
		download_library
		extract_library
	fi
}

##############
# INITIALIZE
##############

create_build_directory()
{
	mkdir -p $LIBRARY_BUILD_TYPE_BUILD_DIR
}

create_install_directory()
{
	message "Creating install directory"
	run_command mkdir -p $LIBRARY_BUILD_TYPE_INSTALL_DIR
}

initialize_log_file()
{
	message "Initializing log file"
	run_command echo " " > $LOG_FILE
}

display_initial_message()
{
	message "Building $LIBRARY in $BUILD_TYPE mode"
}

define_default_derived_variables()
{
	LIBRARY_BUILD_TYPE_INSTALL_DIR=$LIBRARY_INSTALL_DIR/$BUILD_TYPE
	LIBRARY_BUILD_TYPE_BUILD_DIR=$LIBRARY_BUILD_DIR/$BUILD_TYPE
	LOG_FILE=$LIBRARY_BUILD_TYPE_BUILD_DIR/${LIBRARY}_$BUILD_TYPE.log
	COMPRESSED_LIBRARY=$LIBRARY_BUILD_TYPE_BUILD_DIR/$LIBRARY.tar.gz
	SOURCE_DIR=$LIBRARY_BUILD_TYPE_BUILD_DIR/$LIBRARY
}

prepare_to_build_and_install()
{
	create_build_directory
	initialize_log_file
	display_initial_message
	create_install_directory
}

##############
# FINAL STEPS
##############

display_end_message()
{
	# Build directory location
	message "Build directory: $LIBRARY_BUILD_TYPE_BUILD_DIR"

	# Compressed library location
	message "Compressed library: $COMPRESSED_LIBRARY"

	# Intall directory location
	message "Install directory: $LIBRARY_BUILD_TYPE_INSTALL_DIR"

	# log file location
	message "log file: $LOG_FILE"

	# Install directory status (files and
	#	folders created).
	message "`ls -lA $LIBRARY_BUILD_TYPE_INSTALL_DIR/**`"
}

export_name()
{
	# $1 - first argument : name to be exported to environment
	# $2 - second argument : value to be exported
	# example: export BOOST_DIR=/home/guesser/boost

	echo "export $1=$2" >> ~/.bashrc 
	# append in ~/.bashrc file
}
