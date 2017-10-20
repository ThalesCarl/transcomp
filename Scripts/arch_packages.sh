# log file
mkdir -p $ROOT_BUILD_DIR
LOG_FILE="$ROOT_BUILD_DIR/debian_install_packages_log_file.txt"
echo > $LOG_FILE #initialize empty file

install_package()
{
	# usage: $ install_package PACKAGE
	echo "--- $1" | tee -a $LOG_FILE
	sudo pacman -Syu --noconfirm $1 2>>$LOG_FILE >> $LOG_FILE
	echo "" >> $LOG_FILE
}

install_package gcc-multilib
install_package gcc-fortran-multilib
install_package python

install_package make
install_package cmake
install_package gdb
install_package git

install_package bison
install_package flex
install_package diffutils

install_package vim
install_package wget
install_package dos2unix
