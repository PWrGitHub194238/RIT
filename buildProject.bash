#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

exsName="BinaryIMST_Examples"
exsLibPath="$exsName/lib"
exsIncludePath="$exsName/include/IMST"
libName="BinaryIMST_Library"
libIncludePath="$libName/src/include"
staticLibName="lib$libName.a"

# Public function
# Parse command line arguments
#
#	autocompletion	add bash sugestions for all scripts to system
#	buildAll	will build static version of library, 
#			move it along with includes into appropriate locations,
#			link them to example project and build it.
#	buildStaticLib	only static library will be build 
#	buildExample	only example project will be build 
#	moveFiles	find static library in library project along with includes 
#			and move them into appropriate locations
#	clean		clean entire project
#	-f --force	force complite project rebuild
#
function pasreBash() {
	while [[ $# > 0 ]]
	do
		case $1 in
			autocompletion)
				ACTION='autocompletion'
			;;
			buildAll)
				ACTION='buildAll'
			;;
			buildStaticLib)
				ACTION='buildStaticLib'
			;;
			buildExample)
				ACTION='buildExample'
			;;
			moveFiles)
				ACTION='moveFiles'
			;;
			clean)
				ACTION='clean'
			;;
			-l|--without-log)
				LOG4CXX='n'
			;;
			-f|--force)
				FORCE='y'
			;;
			*)
				printHelp
				exit;
			;;
		esac
		shift # past argument or value
	done
}

# Public function
#
# Print help for script
#
function printHelp() {
	echo "Usage: [sudo] bash buildProject.bash autocompletion|buildAll|buildStaticLib|buildExample|moveFiles [-l|--without-log] [-f|--force]"
	echo "	autocompletion		add bash sugestions for all scripts to system"
	echo "	buildAll		will build static version of library,
				move it along with includes into appropriate locations,
				link them to example project and build it"
	echo "	buildStaticLib		only static library will be build"
	echo "	buildExample		only example project will be build"
	echo "	moveFiles		find static library in library project along with includes 
				and move them into appropriate locations"
	echo "	clean			clean entire project"
	echo "	-l --without-log	with this flag, project will be compiled without logging functionality"
	echo "	-f --force		force complite project rebuild"
}


####################################################################################################################



function buildExamples() {
	cd "$exsName"
	if [ "$FORCE" == 'y' ]; then
		./build.bash debug -f
	else
		./build.bash debug
	fi
	cd "../"
}

function moveFiles() {

	local staticLibPath=$(find "$BASE_DIR/$libName/" -type f -name "$staticLibName" -printf '%T@ %p\0' | sort -zk 1nr | sed -z 's/^[^ ]* //' | tr '\0' '\n' | head -1)
	if [ -z "$staticLibPath" ]; then
		echo "Cannot find any '$staticLibName' file in library project. You have to build it first."
	else
		echo "Moving static library '$staticLibName' from '$staticLibPath' to '$exsLibPath'."
		mv "$staticLibPath" "$exsLibPath"
		echo "Coping includes from '$libIncludePath' to '$exsIncludePath'."
		cp -r "$libIncludePath/"* "$exsIncludePath"
		echo "Done."
	fi
}

function buildLib() {
	cd "$libName"
	if [ "$LOG4CXX" == 'n' ]; then
		./toggleLogs.bash
	fi
	if [ "$FORCE" == 'y' ]; then
		./build.bash static -f
	else
		./build.bash static
	fi
	cd "../"
}


####################################################################################################################


function loadBashAutocompletion() {
	source "BinaryIMST_bac"
	sudo cp "./BinaryIMST_bac" "/etc/bash_completion.d/"
}


####################################################################################################################


set -e

pasreBash $@

if [[ "$ACTION" == 'autocompletion' ]]; then
	if [[ $UID != 0 ]]; then
	    echo "In order to successfully execute this commamd, extra privileges will be needed:"
	    sudo echo "You have granted this script root privileges."
	fi
	loadBashAutocompletion
	echo "Please, restart shell in order to changes take effect."
elif [[ "$ACTION" == 'buildAll' ]]; then
	buildLib
	moveFiles
	if [ "$LOG4CXX" == 'n' ]; then
		./$libName/toggleLogs.bash
	fi
	buildExamples
elif [[ "$ACTION" == 'buildStaticLib' ]]; then
	buildLib
	if [ "$LOG4CXX" == 'n' ]; then
		./$libName/toggleLogs.bash
	fi
elif [[ "$ACTION" == 'buildExample' ]]; then
	buildExamples
elif [[ "$ACTION" == 'moveFiles' ]]; then
	moveFiles
elif [[ "$ACTION" == 'clean' ]]; then
	cd "$exsName"
	./clean.bash all
	cd "../$libName"
	./clean.bash all
	cd "../"
	rm -fr *~
else
	printHelp
fi
