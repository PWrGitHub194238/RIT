#!/bin/bash

TODELETEREGEXP='.*\(~\|aux\|bbl\|blg\|log\|nav\|out\|snm\|toc\|idx\|gz\|-converted-to.pdf\|run\)$'

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

execute

