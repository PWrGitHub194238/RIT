#!/bin/bash

firstDataArg=0
numberOfDataArgs=0
inputArray=( "$@" )
dataArray=()
styleArray=()
legendArray=()

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


####################################################################################################################


# Public function
# Parse command line arguments
#
#	-d --defaults			set script not to prompt about any value which has a default value specified
#	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution
#	-p --octave-path <arg>		path for Octave script: generatePlot.m
#	-i --input-data-path <arg>	path to files that contains data to plot
#	-o --output-data-path <arg>	path where Octave script will store generated plot
#	-t --plot-title <arg>		title of generated plot
#	-x --x-axis-title <arg>		name of label of X axis of generated plot
#	-y --y-axis-title <arg>		name of label of Y axis of generated plot
#	-n --file-output-name <arg>	name of an output file with generated plot
#	-e --file-extension <arg>	extension of an of output file with generated plot
#	-h --help			print hwlp message
#
function parseBash() {
	while [[ $# > 0 ]]
	do
		local arg=$(echo $1 | iconv -t utf8)
		case "$arg" in
			-d|--defaults)
				firstDataArg=$(($firstDataArg+1))
				DEFAULT='y'
			;;
			-y|--force-yes)
				firstDataArg=$(($firstDataArg+1))
				FORCE_YES='y'
			;;
			-p|--octave-path)
				firstDataArg=$(($firstDataArg+2))
				bash_octave_path=$(expandPath "$2")
				shift # past argument
			;;
			-i|--input-data-path)
				firstDataArg=$(($firstDataArg+2))
				bash_input_data_path=$(expandPath "$2")
				shift # past argument
			;;
			-o|--output-data-path)
				firstDataArg=$(($firstDataArg+2))
				bash_output_data_path=$(expandPath "$2")
				shift # past argument
			;;
			-t|--plot-title)
				firstDataArg=$(($firstDataArg+2))
				bash_plot_title="$2"
				shift # past argument
			;;
			-x|--x-axis-title)
				firstDataArg=$(($firstDataArg+2))
				bash_x_axis_title="$2"
				shift # past argument
			;;
			-y|--y-axis-title)
				firstDataArg=$(($firstDataArg+2))
				bash_y_axis_title="$2"
				shift # past argument
			;;
			-n|--file-output-name)
				firstDataArg=$(($firstDataArg+2))
				bash_file_output_name="$2"
				shift # past argument
			;;
			-e|--file-extension)
				firstDataArg=$(($firstDataArg+2))
				bash_ext_output_name="$2"
				shift # past argument
			;;
			-h|--help)
				usage
				exit
			;;
		esac
		shift # past argument or value
	done
}

function bash_flags_usage() {
	echo -e "\t\t-d --defaults			set script not to prompt about any value which has a default value specified"
	echo -e "\t\t-f --force-yes			force every prompt to be skiped with 'y' value - use with caution"
	echo -e "\t\t-p --octave-path <arg>		path for Octave script: generatePlot.m"
	echo -e "\t\t-i --input-data-path <arg>	path to files that contains data to plot"
	echo -e "\t\t-o --output-data-path <arg>	path where Octave script will store generated plot"
	echo -e "\t\t-t --plot-title <arg>		title of generated plot"
	echo -e "\t\t-x --x-axis-title <arg>		name of label of X axis of generated plot"
	echo -e "\t\t-y --y-axis-title <arg>		name of label of Y axis of generated plot"
	echo -e "\t\t-n --file-output-name <arg>	name of an output file with generated plot"
	echo -e "\t\t-e --file-extension <arg>		extension of an of output file with generated plot"
	echo -e "\t\t-h --help				prints thia message."
}

function usage() {
	echo -e "bash generatePlot.bash <bash flags> <file name> <octave line style> <function label> [...]\n"
	echo -e "\t<bash flags> - subset of given options:"
	bash_flags_usage
	echo -e "\n\t<file name> - name of the file with data in given format i.e.\n\t\t1\t1\n\t\t2\t4\n\t\t3\t9\n\t\t...\t...\n\tis an example of function f(x)=x^2\n"
	echo -e "\t<octave line style> - style of octave drawing in FMT format. To learn more see Octave's 'plot' documentation.\n"
	echo -e "\t<function label> - how given function will be named on a plot.\n"
}

function fillArrays() {
	for (( i=$firstDataArg; i<$1; ))
		do
		dataArray+=(${inputArray[$i]})
		echo "Add to dataArray: " ${inputArray[$i]}
		i=$(($i+1))

		styleArray+=(${inputArray[$i]})
		echo "Add to styleArray: " ${inputArray[$i]}
		i=$(($i+1))

		legendArray+=(${inputArray[$i]})
		echo "Add to legendArray: " ${inputArray[$i]}
		i=$(($i+1))
	done
}

function generatePlot() {
	local octave_path=$(bash_readDir 'Enter path to generatePlot.m Octave function' './octave' "$bash_octave_path")

	local input_data_path=$(bash_readDir 'Enter path to folder with Data Input Files' './plots/inData' "$bash_input_data_path")

	local output_data_path=$(bash_readDir 'Enter path to output folder for generated plot' './plots/outData' "$bash_output_data_path")

	local plot_title=$(bash_readData 'Enter title of generated plot' '{EMPTY TITLE}' "$bash_plot_title")

	local x_axis_title=$(bash_readData 'Enter title of label for axis X' '{X LABEL}' "$bash_x_axis_title")

	local y_axis_title=$(bash_readData 'Enter title of label for axis Y' '{Y LABEL}' "$bash_y_axis_title")

	local file_output_name=$(bash_readData 'Enter name of output file' 'out' "$bash_file_output_name")

	local ext_output_name=$(bash_readData 'Enter extension of output file' 'epsc' "$bash_ext_output_name")

	echo -e "Plot configuration summary: 
	Octave function path\t:\t"$octave_path"
	Input data path\t\t:\t"$input_data_path"
	Output plot path\t:\t"$output_data_path"
	Plot's title\t\t:\t"$plot_title"
	Plot X label\t\t:\t"$x_axis_title"
	Plot Y label\t\t:\t"$y_axis_title"
	Output file\t\t:\t"$file_output_name"."$ext_output_name"\n\n"

	if ! [ $(bash_confirm 'Do you want to create this plot?') == 'n' ]; then
		echo "Generating plot..."

		# Replace colons
		echo "Formating input files..."
		sed -i 's/,/./g' $input_data_path/*

		#generateOctaveString
		echo "Generating input Octave command..."
		octaveString="addpath(\"$octave_path\")"

		octaveString="$octaveString; dataArray = {"
		for (( i=0; i<$((${#dataArray[@]}-1)); i+=1 ))
		do
			octaveString="$octaveString \"$input_data_path${dataArray[$i]}\","
		done
		octaveString="$octaveString \"$input_data_path${dataArray[$i]}\""
		octaveString="$octaveString}"

		octaveString="$octaveString; styleArray = {"
		for (( i=0; i<$((${#styleArray[@]}-1)); i+=1 ))
		do
			octaveString="$octaveString \"${styleArray[$i]}\","
		done
		octaveString="$octaveString \"${styleArray[$i]}\""
		octaveString="$octaveString}"

		octaveString="$octaveString; legendArray = {"
		for (( i=0; i<$((${#legendArray[@]}-1)); i+=1 ))
		do
			octaveString="$octaveString \"${legendArray[$i]}\","
		done
		octaveString="$octaveString \"${legendArray[$i]}\""
		octaveString="$octaveString}"

		octaveString="$octaveString; generatePlot(\"$plot_title\",\"$x_axis_title\",\"$y_axis_title\",dataArray,styleArray,legendArray,\"$output_data_path$file_output_name\",\"$ext_output_name\",$(($numberOfDataArgs/3)))"

		echo -e "Execute Octave commands:\n'$octaveString'"
		octave --eval "$octaveString"
		if [ $? -eq 0 ]; then
			echo -e "Plot has been saved to: \"$output_data_path\" as \"$file_output_name.$ext_output_name\"\n"
		else
			echo -e "Something goes wrong."
		fi;
	fi;
}



echo '---------------------------------------------------------------------------------'

parseBash "$@"

numberOfDataArgs=$(($#-$firstDataArg))

if [ $numberOfDataArgs != 0 -a $(($numberOfDataArgs%3)) = 0 ];
then
	fillArrays "$#"
	generatePlot
else
	usage
fi
