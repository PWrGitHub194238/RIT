#!/bin/bash

TODELETEREGEXP='.*\(~\|aux\|bbl\|blg\|[.]log\|nav\|out\|snm\|toc\|idx\|gz\|-converted-to.pdf\|run\)$'
PATHS="./ EmptyTeX/ Presentations/ References/ RIT_Examples/ RIT_Library/ Scripts/ Thesis/"

function execute {
	echo -e "List of files to be deleted in $(pwd) and subdirs:\n"
	find $PATHS -regex "$TODELETEREGEXP"
	read -p 'Do you realy want to DELETE this files? (y/n): ' deleteFiles
	if [ $deleteFiles == "y" ] 
	then
		find $PATHS -regex "$TODELETEREGEXP" -exec rm {} \;
	fi
}

execute

bash ./RIT_Library/clean.bash all
bash ./RIT_Examples/clean.bash all
