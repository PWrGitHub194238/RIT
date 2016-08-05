#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

appName="RIT_Examples"
libName="RIT_Library"
libPath="lib/lib$libName.a"
exsIncludePath="include/IMST"



# Public function
# Parse command line arguments
#
#	all		clean entire project
#	debug		clean only 'Debug' configuration
#	shared		clean only 'Shared' configuration
#	static		clean only 'Static' configuration
#	doc		clean documentation only
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
				cleanAll
			;;
			debug)
				cleanProject 'Debug' "$appName"
			;;
			shared)
				cleanProject 'Shared' "lib$appName.so"
			;;
			static)
				cleanProject 'Static' "lib$appName.a"
				;;
			doc)
				cleanDoc
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
	echo "Usage: [sudo] bash clean.bash [all|debug|shared|static|doc]"
	echo "	all		clean entire project"
	echo "	debug		clean only 'Debug' configuration"
	echo "	shared		clean only 'Shared' configuration"
	echo "	static		clean only 'Static' configuration"
	echo "	doc		clean documentation only"
}

###########################################################################################

function clean() {
	rm "$BASE_DIR/"*.log
	rm -fr "$BASE_DIR/"*~
	rm "$BASE_DIR/$libPath"
	rm -r "$BASE_DIR/$exsIncludePath/"*
}

function cleanDoc() {
	rm -fr "$BASE_DIR/html/"
	rm -fr "$BASE_DIR/latex/"
	rm -f "$BASE_DIR/doc.html"
}

function cleanProject() {
	cd "$BASE_DIR/$1"
	make clean
	cd ../
	rm "$2"
}

function cleanAll() {
	cleanDoc
	cleanProject 'Debug' "$appName"
	cleanProject 'Shared' "lib$appName.so"
	cleanProject 'Static' "lib$appName.a"
}

###########################################################################################

pasreBash $@
clean
