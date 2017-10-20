#!/bin/sh

if $INSTALL_EXTERNAL_PACKAGES
then

	############### External packages ###############
	if $SYSTEM_PACKAGES
	then
		install_system_packages
	fi

	############### building libs ###############
	if $OPENMPI
	then
		# Build openMPI on Linux, not on cygwin
		if $LINUX
		then
			echo "" >> $HOME/.bashrc #prevent error when export name
			auto_build_library openmpi-2.1.0
			export OPENMPI_DIR="$ROOT_INSTALL_DIR/openmpi-2.1.0"
			export PATH="$OPENMPI_DIR/bin:$OPENMPI_DIR/lib:$OPENMPI_DIR/include:$PATH"
		fi

		# Set some MPI environmet variables
		export CC=mpicc
		export CXX=mpicxx
		export FC=mpifort
		export F77=mpifort
		export F90=mpifort
	fi

	if $CMAKE
	then
		auto_build_library cmake-3.8.2
		export CMAKE_DIR=$ROOT_INSTALL_DIR/cmake-3.8.2
		export PATH="$CMAKE_DIR/bin:$PATH"
	fi

	if $BOOST
	then
		auto_build_library boost-1.63.0
		export BOOST_DIR=$ROOT_INSTALL_DIR/boost-1.63.0
	fi

	if $PETSC
	then
		auto_build_library petsc-3.7.6
		export PETSC_DIR=$ROOT_INSTALL_DIR/petsc-3.7.6
	fi
