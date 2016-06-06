#!/bin/bash

DIRECTORY="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SUBDIR="Thesis"

TODELETEREGEXP='.*\(~\|aux\|bbl\|blg\|log\|nav\|out\|snm\|toc\|idx\|gz\|-converted-to.pdf\|run\)$'

function invalidFileStruct {
	echo -e "No directory: \"$SUBDIR\". Abort."
}

function execute {
	echo -e "List of files to be deleted in $(pwd) and subdirs:\n"
	find . -regex "$TODELETEREGEXP"
	read -p 'Do you realy want to DELETE this files? (y/n): ' deleteFiles
	if [ $deleteFiles == "y" ] 
	then
		find . -regex "$TODELETEREGEXP" -exec rm {} \;
	fi
}

echo '----------------------------------------------------'
if [ -d "$DIRECTORY$SUBDIR" ]; then
	invalidFileStruct $SUBDIR
else
	cd $DIRECTORY"/../"$SUBDIR
	execute
fi

