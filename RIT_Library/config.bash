#!/bin/bash

sharedLibConfigDir='/etc/ld.so.conf.d/'

rapidJSONURL='https://github.com/miloyip/rapidjson.git'

# Public function
# Parse command line arguments
#
#	autocompletion			add bash sugestions for all scripts to system
#	help				print this message
#	install	[arg]			run script with default settings that will install all dependencies required 
#					if no argument was specify.
#		log4cxx
#		graphviz
#		rapidJSON
#	initBuild			run Eclipse's headless build menager to recreate all makefiles and clean the project
#	-p --path			path to Eclipse's workspace
#	-d --defaults			set script not to prompt about any value which has a default one specified
#	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution
#	--rapidjson-path <arg>		path where RapidJSON project will be downloaded
#	--rapidjson-include-path <arg>	path where RapidJSON header files will be moved
#
function pasreBash() {
	while [[ $# > 0 ]]
	do
		case $1 in
			autocompletion)
				ACTION='autocompletion'
			;;
			help)
				ACTION='help'
			;;
			install)
				ACTION='installAll'
			;;
			log4cxx)
				ACTION='install'
				LOG4CXX='y'
			;;
			graphviz)
				ACTION='install'
				GRAPHVIZ='y'
			;;
			rapidJSON)
				ACTION='install'
				RAPIDJSON='y'
			;;
			initBuild)
				ACTION='initBuild'
			;;
			-p|--path)
				WORKSPACE_PATH=$(expandPath "$2")
				shift # past argument
			;;
			-d|--defaults)
				DEFAULT='y'
			;;
			-f|--force-yes)
				FORCE_YES='y'
			;;
			--rapidjson-path)
				bash_rapidjson_path=$(expandPath "$2")
				shift # past argument
			;;
			--rapidjson-include-path)
				bash_rapidjson_include_path=$(expandPath "$2")
				shift # past argument
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
	echo "Usage: [sudo] bash install.bash autocompletion|help|install|initBuild [log4cxx graphviz rapidJSON] [-d|--defaults] [-f|--force-yes] [--rapidjson-path <arg>] [--rapidjson-include-path <arg>]"
	echo "	autocompletion			add bash sugestions for all scripts to system"
	echo "	help				print this message"
	echo "	install				run script with default settings that will install all dependencies required 
					if no argument was specify.
			log4cxx
			graphviz
			rapidJSON"
	echo "	initBuild			run Eclipse's headless build menager to recreate all makefiles and clean the project"
	echo "	-p --path			path to Eclipse's workspace"
	echo "	-d --defaults			set script not to prompt about any value which has a default one specified"
	echo "	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution"
	echo "	--rapidjson-path <arg>		path where RapidJSON project will be downloaded"
	echo "	--rapidjson-include-path <arg>	path where RapidJSON header files will be moved"
}


# Public function
# Return file extension from given path
# Example:
#	getFileExt '/usr/foo.bar'
#	returns 'bar'
#
function getFileExt() {
	echo ${1##*.}
}

# Public function
# Return file name with its extension from given path
# Example:
#	getFileName '/usr/foo.bar'
#	returns 'foo.bar'
#
function getFileName() {
	echo ${1##*/}
}

# Public function
# Return directory name from given path
# Example:
#	getDir '/usr/foo.bar'
#	returns '/usr'
#
function getDir() {
	echo ${1%/*}
}

# Public function
# Return base directory name from given path
# Example:
#	getDir '/usr/foo/foo.bar'
#	returns '/usr'
#
function getBaseDir() {
	echo ${1%%/*}
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

# Public function
# Read user input and return inputed value or default one (if user left field blank)
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(readData 'Text#1' 'Text#2')
#	returns 'Text#2' or user no-empty input
#
function readData() {
	read -rep "$1 (\"$2\" if left blank): "

	if [ -z "$REPLY" ]; then
		echo "$2"
	else
		echo "$REPLY"
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	result=$(bash_readData 'Text#1' 'Text#2' "$var")
#	returns
#		- Default value ('Text#2') if $DEFAULT is set to 'y'
#		- Value given by the command line if specified
#		- readData() returned value
#
function bash_readData() {
	if [ "$DEFAULT" == 'y' ]; then
		echo "$2"
	elif ! [ -z "$3" ]; then
		echo "$3"
	else
		echo $(readData "$1" "$2")
	fi
}

# Public function
# Read user input and return inputed value or default one (if user left field blank).
# In both cases special character '~', if occurred, will be expanded to home directory.
# Only first '~' will be expanded.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(readDir 'Text#1' $(pwd)'/Text#2/')
#	returns absolute path to folder 'Text#2' in current directory or user no-empty input
#
function readDir() {
	local var=$(readData "$1" "$2")

	echo $(expandPath "$var")
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	result=$(bash_readDir 'Text#1' $(pwd)'/Text#2/' "$var")
#	returns bash_readData() expanded value
#
function bash_readDir() {
	local var=$(bash_readData "$1" "$2" "$3")

	echo $(expandPath "$var")
}


# Public function
# Read user input and return inputed value or default one (if user left field blank).
# In both cases special character '~', if occurred, will be expanded to home directory.
# Only first '~' will be expanded.
# If given directory exists then function will ask to delete it first.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(remakeDir 'Text#1' $(pwd)'/Text#2/')
#	returns absolute path to folder 'Text#2' in current directory or user no-empty input
#
function remakeDir() {
	local var=$(readDir "$1" "$2")
	
	removeIfExists "\"$var\" already exists. Remove this directory?" "$var"
	makeDirIfNotExists "$var"

	echo "$var"
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	var=$(bash_remakeDir 'Text#1' $(pwd)'/Text#2/' "$var")
#	returns bash_readDir() value
#
function bash_remakeDir() {
	local var=$(bash_readDir "$1" "$2" "$3")

	bash_removeIfExists "\"$var\" already exists. Remove this directory?" "$var"
	bash_makeDirIfNotExists "$var"

	echo "$var"
}

# Public function
# Remove file/directory if it exists.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#	$2 - file name/path to directory
#
function removeIfExists() {
	if [ -d "$2" ]; then
		if [ $(confirm "$1") == "y" ]; then
			sudo rm -fr "$2"
		fi;
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#	$2 - file name/path to directory
#
function bash_removeIfExists() {
	if [ -d "$2" ]; then
		if [ $(bash_confirm "$1") == "y" ]; then
			sudo rm -fr "$2"
		fi;
	fi
}

# Public function
# Create given directory if it  exists.
#
# Parameters:
#	$1 - path to directory 
#
function makeDirIfNotExists() {
	if ! [ -d "$1" ]; then
		sudo mkdir -p "$1"
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - path to directory 
#
function bash_makeDirIfNotExists() {
	makeDirIfNotExists "$1"
}

# Public function
# Show prompt and force user to confirm or cancel.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#
function confirm() {
	read -e -p "$1 (y/n): "
	echo "$REPLY"
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#
function bash_confirm() {
	if [ "$FORCE_YES" == 'y' ]; then
		echo 'y'
	else
		echo $(confirm "$1")
	fi
}

# Public function
# Install additional package and dependencies
#
# Parameters:
#	$1 - package name
#
function installPackage() {
	if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
	then
		if [ $(confirm "Confirm installation of package \"$1\"?") == "y" ]; then
			sudo apt-get -yf install "$1"
			sudo apt-get -yf install
		fi;
	else
		echo "Package \"$1\" already has been installed."
	fi;
}

# Public function
# Install additional package and dependencies
#
# Parameters:
#	$1 - package name
#
function bash_installPackage() {
	if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
	then
		if [ $(bash_confirm "Confirm installation of package \"$1"\"?) == "y" ]; then
			sudo apt-get -yf install "$1"
			sudo apt-get -yf install
		fi;
	else
		echo "Package '$1' is already installed. Skipping..."
	fi;
}

# Public function
# Updates the cache of the linker with given shared objects
#
# Parameters:
#	$1 - directory of shared libraries to be added
#	$2 - name of a config file to be readed by the linker
#	$3 - directory of configurations for the linker
# Example:
#	addSharedLibToLinker '/usr/local/lib/X' '/etc/ld.so.conf.d/' 'X.conf'
#	will add config file with location of shared libraries in directory '/usr/local/lib/X'
#
function addSharedLibToLinker() {
	echo "Create \"$3\" file in directory \"$2\"..."

	sudo sh -c "echo \"$1\" > \"$2$3\""

	echo 'Reload system configuration...'
	sudo ldconfig

	echo 'Done.'
}

####################################################################################################################

# Public function
# Checkout given RapidJSON project stable version into given path and move all files to given directory.
#
function executeRapidJSONCreation() {
	echo -e "Installing 'RapidJSON' library...\n\n"
	local rapidJSON_path=$(bash_remakeDir 'Enter where to checkout RapidJSON project' $(pwd)'/RapidJSON/' "$bash_rapidjson_path")
	local rapidJSON_include_path=$(bash_remakeDir 'Enter where to move RapidJSON header files' '/usr/local/include/rapidjson/' "$bash_rapidjson_include_path")
	getRapidJSON "$rapidJSON_path"
	
	sudo mv "$rapidJSON_path"'include/rapidjson/'* "$rapidJSON_include_path"

	echo "RapidJSON header files have been moved to \"$rapidJSON_include_path\"."

	bash_removeIfExists "Delete RapidJSON project sources?" "$rapidJSON_path"
	echo "Done"
}

# Private function
# Checkout RapidJSON project into given path
#
# Parameters:
#	$1 - path where RapidJSON project will be checked out.
# Example:
#	getRapidJSON '~/git/RapidJSON'
#	will download to '~/git/RapidJSON'
#
function getRapidJSON() {
	local thisPath=$(pwd)
	echo "\"git\" is required to checkout GTest project's sources."
	bash_installPackage "git"
	echo "\"pthread\" library will be needed to build GTest form sources."
	bash_installPackage "libpthread-stubs0-dev"

	echo 'Checking out the RapidJSON project...'
	sudo git clone "$rapidJSONURL" "$1"

	echo "RapidJSON project has been saved into \"$1\"."
	cd "$1"
	
	#sudo git submodule update --init

	makeRapidJSON
	cd "$thisPath"
}

function makeRapidJSON() {
	makeDirIfNotExists "build"
	cd "build"
	sudo cmake ..
	sudo make
	#sudo make test
}


####################################################################################################################


function executeLog4cxxCreation() {
	echo -e "Installing 'Log4cxx' library...\n\n"
	bash_installPackage "liblog4cxx10v5"
	bash_installPackage "liblog4cxx10-dev"
	echo "Done"
}


function executeGraphvizCreation() {
	echo -e "Installing 'GraphViz' library...\n\n"
	sudo add-apt-repository -y ppa:gviz-adm/graphviz-dev
	set +e
	sudo apt-get -y update
	sudo apt-get -y autoremove graphviz
	sudo apt-get -y remove libcdt4
	sudo apt-get -y remove libpathplan4
	set -e
	sudo apt-get -y install graphviz graphviz-dev
}


####################################################################################################################


function loadBashAutocompletion() {
	source "RIT_Library_bac"
	sudo cp "./RIT_Library_bac" "/etc/bash_completion.d/"
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
elif [[ "$ACTION" == 'installAll' || "$ACTION" == 'install' ]]; then
	if [[ $UID != 0 ]]; then
	    echo "In order to successfully install all dependencies, apt-get will be called, this extra privileges will be needed:"
	    sudo echo "You have granted this script root privileges."
	fi
	if [[ "$ACTION" == 'installAll' ]]; then
		echo -e "Following dependencies will be installed:\n\tliblog4cxx10v5\n\tliblog4cxx10-dev\n\tgraphviz\n\tgraphviz-dev\n\tRapidJSON."
		if [ $(confirm "Confirm installation of this packages?") == "y" ]; then
			executeLog4cxxCreation
			executeGraphvizCreation
			executeRapidJSONCreation
			sudo apt-get -y autoremove
		else
			echo "Process aborted."
		fi
	else
		if [ "$LOG4CXX" == 'y' ]; then
			executeLog4cxxCreation
			sudo apt-get -y autoremove
		fi
		if [ "$GRAPHVIZ" == 'y' ]; then
			executeGraphvizCreation
			sudo apt-get -y autoremove
		fi
		if [ "$RAPIDJSON" == 'y' ]; then
			executeRapidJSONCreation
			sudo apt-get -y autoremove
		fi
	fi
	loadBashAutocompletion
elif [[ "$ACTION" == 'initBuild' ]]; then
	if [[ -n ${WORKSPACE_PATH+x} ]]; then
		echo "WARNING: In order to Eclipse successfully generate project's makefiles, You have to run this script as Eclipse's owner."
		eclipse -nosplash -application 'org.eclipse.cdt.managedbuilder.core.headlessbuild' -data "$WORKSPACE_PATH" -cleanBuild 'RIT_Library'
		bash clean.bash all
	else
		echo "You need to specify a path to Eclipse's workspace that contains RIT_Library project (-p|--path)."
	fi;
elif [[ "$ACTION" == 'help' ]]; then
	printHelp
else
	echo -e "You have to specify at least one action from these:\n\tautocompletion\n\thelp\n\tinstall\n\tinitBuild -p|--path <arg>"
fi
