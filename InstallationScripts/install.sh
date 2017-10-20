#!/bin/bash

############### Set some paths ###############
export ROOT_SCRIPTS_DIR=`pwd`
export EXTERNAL_PACKAGES_ROOT_SCRIPTS_DIR="$ROOT_SCRIPTS_DIR/external_packages_scripts"
export LIBRARIES_ROOT_SCRIPTS_DIR="$ROOT_SCRIPTS_DIR/libraries_scripts"

##### Load settings #####
. $ROOT_SCRIPTS_DIR/settings.sh

##### Load auxiliar functions #####
. $ROOT_SCRIPTS_DIR/auxiliar_functions.sh

##### install external packages #####
. $ROOT_SCRIPTS_DIR/external_contents_install.sh

# Process complete
echo -e "\n\n------------------ COMPLETE ------------------\n\n"
exit 0
