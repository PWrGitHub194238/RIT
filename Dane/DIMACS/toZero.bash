#!/bin/bash

# Bellow command will subtract 1 from each vertex label in each USA*.gr file
#find ./ -name "USA*.gr" -exec bash toZero.bash -f -i {} -o {} -s 1 \;

# Public function
# Parse command line arguments
#
#	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution
#	-i --input <arg>		path to input file
#	-o --output <arg>		path to output file
#	-a -add	<arg>			add given number to each vertex label
#	-s --subtract <arg>		subtract given number to each vertex label
#
function pasreBash() {
	while [[ $# > 0 ]]
	do
		case $1 in
			-f|--force-yes)
				FORCE_YES='y'
			;;
			-i|--input)
				input_path=$(expandFilePath "$2")
				shift # past argument
			;;
			-o|--output)
				output_path=$(expandFilePath "$2")
				shift # past argument
			;;
			-a|--add)
				num=$2
				shift # past argument
			;;
			-s|--subtract)
				num=$((-$2))
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
	echo "Usage: [sudo] bash toZero.bash -i|--input <arg> -a|--add|-s|--subtract <arg> [-o|--output <arg>] [-f|--force_yes]"
	echo "	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution"
	echo "	-i --input <arg>		path to input file"
	echo "	-o --output <arg>		path to output file"
	echo "	-a -add	<arg>			add given number to each vertex label"
	echo "	-s --subtract <arg>		subtract given number to each vertex label"
}

# Public function
# Expand special character '~'/'.', if occurred, to home/current directory.
# Only first '~'/'.' will be expanded.
#
function expandFilePath() {
	# Expand of ~ character
	local var="${1/#\~/$HOME}"

	# Expand of . character
	local var="${var/#\./$PWD}"

	echo "$var"
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

#################################################################################################################

pasreBash $@

if [ -z "$input_path" ] || [ -z "$num" ]; then
	printHelp
	exit;
elif [ -n "$output_path" ]; then
	if [ "$input_path" == "$output_path" ]; then
		if [ $(confirm "Are you sure that you want to overwrite this file (\"$input_path\")?") == "y" ]; then
			perl -pi -e 's/^a\s+?\K(\d+)/($1+'$num')/ge' "$output_path"
			perl -pi -e 's/^a\s+?\d+\s+?\K(\d+)/($1+'$num')/ge' "$output_path"
		else
			echo "File \"$input_path\" was not overwritten."
		fi
	else
		perl -pe 's/^a\s+?\K(\d+)/($1+'$num')/ge' "$input_path" > "$output_path"
		perl -pi -e 's/^a\s+?\d+\s+?\K(\d+)/($1+'$num')/ge' "$output_path"
	fi
else
	perl -pe 's/^a\s+?\K(\d+)/($1+'$num')/ge' "$input_path" | perl -pi -e 's/^a\s+?\d+\s+?\K(\d+)/($1+'$num')/ge'
fi

