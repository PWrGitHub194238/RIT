#!/bin/bash

function execute {
	read -p 'Enter full path to library project or leave empty (TMH_Library/):' LIBPATH

	if [ -z "$LIBPATH" ] 
	then
		LIBPATH="TMH_Library/"
	fi

	read -p 'Enter full path to sample project or leave empty (TMH_Examples/):' SAMPLEPATH

	if [ -z "$SAMPLEPATH" ] 
	then
		SAMPLEPATH="TMH_Examples/"
	fi

	read -p 'Enter full path for where generated library should be copy or leave empty (/usr/lib):' SOPATH

	if [ -z "$SOPATH" ] 
	then
		SOPATH="/usr/lib"
	fi

	read -p 'Enter name for generated program or leave empty (run):' EXECNAME

	if [ -z "$EXECNAME" ] 
	then
		EXECNAME="run"
	fi

	buildLib
	if [ -f "$LIBPATH/Debug/libTMH_Library.so" ] 
	then
		buildSample
		cp $LIBPATH/Debug/libTMH_Library.so $SOPATH
	else
		echo "Library build failure."
	fi

	echo "Done."
	echo "Type ./$EXECNAME to execute program or 'valgrind --leak-check=full --track-fds=yes --show-leak-kinds=all -v ./$EXECNAME' to scan for mamory leaks."
}


function buildLib {
	echo '----------------------------------------------------'
	echo "Go to $LIBPATH..."
	cd $LIBPATH
	echo 'Building library:'
	cd Debug
	rm "libTMH_Library.so"
	make -B all
	cd ../../
}

function buildSample {
	echo '----------------------------------------------------'
	echo "Go to $SAMPLEPATH..."
	cd $SAMPLEPATH
	echo 'Building sample:'
	cd Debug
	make -B all
	cp TMH_Examples ../../run
	cd ../../
	mv run $EXECNAME
}

echo '----------------------------------------------------'
cd ../
execute
