#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

appName="RIT_Library"

# Public function
# Parse command line arguments
#
#	all		build entire project
#	debug		build only 'Debug' configuration
#	shared		build only 'Shared' configuration
#	static		build only 'Static' configuration
#	doc		build documentation only
#	-f --force	force complite project rebuild
#
function pasreBash() {
	if [[ $# -eq 0 ]]; then
		printHelp
		exit;
	fi
	while [[ $# > 0 ]]
	do
		case $1 in
			all)
				ACTION='all'
			;;
			debug)
				ACTION='debug'
			;;
			shared)
				ACTION='shared'
			;;
			static)
				ACTION='static'
			;;
			doc)
				ACTION='doc'
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
	echo "Usage: [sudo] bash build.bash all|debug|shared|static|doc [-f|--force]"
	echo "	all		build entire project"
	echo "	debug		build only 'Debug' configuration"
	echo "	shared		build only 'Shared' configuration"
	echo "	static		build only 'Static' configuration"
	echo "	doc		build documentation only"
	echo "	-f --force	force complite project rebuild"
}

###########################################################################################

function buildDoc() {
	doxygen "$1"
}

function buildProject() {
	cd "$BASE_DIR/$1"
	if [ "$FORCE" == 'y' ]; then
		make clean
		make -B all	
	else
		make all
	fi
	if [ $? -eq 0 ]; then
		mv "./$2" "../$2"
		echo "Done..."
	fi
	cd ../
}

###########################################################################################

set -e

pasreBash $@

if [ "$ACTION" == 'all' ]; then
	buildDoc "$appName.Doxyfile"
	buildProject 'Debug' "$appName"
	buildProject 'Shared' "lib$appName.so"
	buildProject 'Static' "lib$appName.a"
elif [ "$ACTION" == 'debug' ]; then
	buildProject 'Debug' "$appName"
elif [ "$ACTION" == 'shared' ]; then
	buildProject 'Shared' "lib$appName.so"
elif [ "$ACTION" == 'static' ]; then
	buildProject 'Static' "lib$appName.a"
elif [ "$ACTION" == 'doc' ]; then
	buildDoc "$appName.Doxyfile"
else
	printHelp
fi
