# Script make to automatically install
# packages neede by Linux operating
# systems based on Debian (debian, ubuntu,
# kubuntu, mint, etc.)


# fix unmet dependencies
sudo apt-get install -f # fix unmet dependencies

# log file
mkdir -p $ROOT_BUILD_DIR
LOG_FILE="$ROOT_BUILD_DIR/debian_install_packages_log_file.txt"
echo > $LOG_FILE #initialize empty file

# Install package function
install_package()
{
	# usage: $ install_package PACKAGE
	echo "--- $1" | tee -a $LOG_FILE
	sudo apt install -y $1 2>>$LOG_FILE >> $LOG_FILE
	echo "" >> $LOG_FILE
}


install_package python3
install_package python3-dev
install_package python3-numpy
install_package python3-matplotlib
install_package gfortran
