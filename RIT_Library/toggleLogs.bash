#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Public function
# Return directory name from given path
# Example:
#	getDir '/usr/foo.bar'
#	returns '/usr'
#
function getParentDir() {
	echo ${1%/*/*}
}

# Public function
# Expand special character '~'/'.', if occurred, to home/current directory.
# Only first '~'/'.' will be expanded.
# Add '/' as a last character if needed
#
function expandPath() {
	# Expand of ~ character
	local var="${1/#\~/$HOME}"

	# Expand of . character
	local var="${var/#\./$PWD}"

	# Add '/' as a last character if needed
	if ! [[ "$var" =~ '/'$ ]]; then 
		var=$var'/'

	fi

	echo "$var"
}

function move() {
	echo "Move '$1' > '$2'."
	mv "$1" "$2"
}

function makeDir() {
	echo "Create directory '$1'."
	mkdir -p "$1"
}

# Script control flag
# 0 - uncomments every log-like line in project and moves back all necessary files,
# 1 - comments every log-like line in project and moves all logging related files out.
toggleLogs=0

# Regexp of lines with logging macro (in 1 line).
INCLUDE_LOGGER='#include <(log4cxx\/.*)>'

INCLUDE_LOG_BUNDLE='#include ".*?\/log\/bundle\/.*"'
INCLUDE_LOG_UTILS='#include ".*?\/log\/utils\/.*"'

LOG_POINTER='(const |const static |static const )?log4cxx::LoggerPtr \w*?\(\s*log4cxx::Logger::getLogger\(\s*".*?"\)\)\s*?;'
CONFIG_LOG='LogUtils::.+?\(.+?\)\s*?;'
	
LOG_LINES='((TRACE|DEBUG|INFO|WARN|ERROR|FATAL)(_NOARG)?\(\s*.+?,(.|\s)*?\))\s*?;'
COMMENT_START='\/\*'
COMMENT_END='\*\/'

MAKEFILE_LOG='^#?-include src\/((src\/log)|(test\/log4cxx)).*?'
SOURCES_LOG='^#?src\/(src\/log|test\/log4cxx).*?'
MAKE_COMMENT='#'

OBJECTS_LOG='LIBS :=( -l\w+)*'
LOG4CXX_LIB='log4cxx'

projectPath=$BASE_DIR
includeProjectPath="$projectPath/src/include"
srcProjectPath="$projectPath/src/src"

# Path to folder where all logging-related files will be stored when they are moved from project
extractLogForder="$projectPath/extracted_log"

# All files and directories to be moved from project in order to delete logging funcionality
logIncludePath="$includeProjectPath/log"
logSrcPath="$srcProjectPath/log"

includeExtractPath="$extractLogForder/src/include"
srcExtractPath="$extractLogForder/src/src"

logIncludeExtractPath="$includeExtractPath/log"
logSrcExtractPath="$srcExtractPath/log"

function getToggleState() {
	
	# If no extract folder exists create one and assume to comment all logs
	if ! [ -d "$1" ]; then
		toggleLogs=1
		makeDir "$includeExtractPath"
		makeDir "$srcExtractPath"
	fi

}

function uncommentMakefile() {
	cd "$projectPath/$1"
	make clean

	echo "Changing $1 makefile..."
	sed -E -i "/$MAKEFILE_LOG/s/$MAKE_COMMENT//" 'makefile'

	echo "Changing $1 sources.mk..."
	sed -E -i "/$SOURCES_LOG/s/$MAKE_COMMENT//" 'sources.mk'

	if [[ "$1" != "Static" ]]; then
		echo "Changing $1 objects.mk..."
		sed -E -i "s/$OBJECTS_LOG/& -l$LOG4CXX_LIB/" 'objects.mk'
	fi

	cd "$projectPath"
}


function uncommentLogs() {
	echo "Add logs back to project..."

	echo "Move logging-related sources into project..."
	move "$logSrcExtractPath" "$logSrcPath"

	local srcFiles=$(find "$srcProjectPath" -name '*.cpp' -or -name '*.c')

	echo -e "\nUncomment logging-related code in source files...\n"
	for f in $srcFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/$COMMENT_START\s*($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)\s*$COMMENT_END/\$1/g" "$f"
	done;
	echo -e "\nAll logging-related code in source files has been uncommended."

	echo -e "\nMove logging-related headers into project..."
	move "$logIncludeExtractPath" "$logIncludePath"

	local includeFiles=$(find "$includeProjectPath" -name '*.hpp' -or -name '*.h')

	echo -e "\nUncomment logging-related code in header files...\n"
	for f in $includeFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/$COMMENT_START\s*($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)\s*$COMMENT_END/\$1/g" "$f"
	done;
	echo -e "\nAll logging-related code in headers files has been uncommended."

	echo -e "\nChanging makefile and related files..."

	uncommentMakefile "Debug"
	uncommentMakefile "Release"
	uncommentMakefile "Shared"
	uncommentMakefile "Static"

	rm -rf "$extractLogForder"
	echo "Done."

}

function commentMakefile() {
	cd "$projectPath/$1"
	make clean

	echo "Changing $1 makefile..."
	sed -E -i "s/$MAKEFILE_LOG/$MAKE_COMMENT\0/" 'makefile'

	echo "Changing $1 sources.mk..."
	sed -E -i "s/$SOURCES_LOG/$MAKE_COMMENT\0/" 'sources.mk'

	echo "Changing $1 objects.mk..."
	sed -E -i "/$OBJECTS_LOG/s/ -l$LOG4CXX_LIB//" 'objects.mk'

	cd "$projectPath"
}

function commentLogs() {
	echo "Removing logs from project..."
	
	echo "Move logging-related sources out from project..."
	move "$logSrcPath" "$srcExtractPath"
	
	local srcFiles=$(find "$srcProjectPath" -name '*.cpp' -or -name '*.c')

	echo -e "\nComment logging-related code in source files...\n"
	for f in $srcFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)/$COMMENT_START\t\$1\t$COMMENT_END/g" "$f"
	done;
	echo -e "\nAll logging-related code in source files has been commended."

	echo -e "\nMove logging-related headers out from project..."
	move "$logIncludePath" "$includeExtractPath"

	local includeFiles=$(find "$includeProjectPath" -name '*.hpp' -or -name '*.h')

	echo -e "\nComment logging-related code in header files...\n"
	for f in $includeFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)/$COMMENT_START\t\$1\t$COMMENT_END/g" "$f"
	done;
	echo -e "\nAll logging-related code in headers files has been commended."

	echo -e "\nChanging makefile and related files..."

	commentMakefile "Debug"
	commentMakefile "Release"
	commentMakefile "Shared"
	commentMakefile "Static"

	echo "Done."
	
}

getToggleState $extractLogForder

echo '--------------------------------------------------------------'
if [ $toggleLogs -eq 0 ]; then
	uncommentLogs
else
	commentLogs
fi
echo '--------------------------------------------------------------'
