# Do not forget to install wget

# Install apt-cyg in cygwin
install $EXTERNAL_PACKAGES_ROOT_SCRIPTS_DIR/apt-cyg /bin

# log file
mkdir -p $ROOT_BUILD_DIR
LOG_FILE="$ROOT_BUILD_DIR/cygwin_install_packages_log_file.txt"
echo > $LOG_FILE #initialize empty file

# Install package function
install_package()
{
	# usage: $ install_package PACKAGE
	echo "--- $1" | tee -a $LOG_FILE
	apt-cyg install $1 2>>$LOG_FILE >> $LOG_FILE
	echo "" >> $LOG_FILE
}

# Install packages
install_package "gcc-core"
install_package "gcc-fortran"
install_package "gcc-g++"
install_package "libgcc1"
install_package "colorgcc"
install_package "make"
install_package "cmake"
install_package "dos2unix"
install_package "bison"
install_package "diffutils"
install_package "flex"
install_package "python2"
install_package "python2-devel"
install_package "python3"
install_package "python3-devel"
install_package "wget"
install_package "libopenmpi-devel"
install_package "libopenmpi12"
install_package "libopenmpicxx1"
install_package "libopenmpifh12"
install_package "openmpi"
install_package "gdb"
install_package "git"
install_package "libncurses-devel"
