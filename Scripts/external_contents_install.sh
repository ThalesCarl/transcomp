#!/bin/sh

############### Set some paths ###############
export SCRIPTS_DIR=`pwd`
export ROOT_INSTALL_DIR="$HOME/Install"
export ROOT_BUILD_DIR="$HOME/Build"

##### Load auxiliar functions #####
. $SCRIPTS_DIR/auxiliar_functions.sh

##### Get system number os processor cores #####
export NUMBER_OF_CORES=`nproc`

############### External packages ###############

 OS_NAME=`uname -a`
 
 if contains "$OS_NAME" "Debian"
 then
 	message "Debian detected"
 	auto_build_library debian_packages
 elif contains "$OS_NAME" "Ubuntu"
 then
 	message "Ubuntu detected."
 	auto_build_library debian_packages
 elif contains "$OS_NAME" "ARCH"
 then
 	message "ARCH linux detected."
 	auto_build_library arch_packages
 elif contains "$OS_NAME" "MANJARO"
 then
 	message "MANJARO detected."
 	auto_build_library arch_packages
 elif contains "$OS_NAME" "CYGWIN"
 then
 	message "Cygwin detected."
 	auto_build_library cygwin_packages
 fi
############### building libs ###############

auto_build_library boost-1.63.0
export BOOST_DIR=$ROOT_INSTALL_DIR/boost-1.63.0

auto_build_library petsc-3.7.6
export PETSC_DIR=$ROOT_INSTALL_DIR/petsc-3.7.6

# Process complete
echo -e "\n\n------------------ COMPLETE ------------------\n\n"
exit 0
