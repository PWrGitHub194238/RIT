#!/bin/bash

BASE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Public function
# Parse command line arguments
#
#	-l --lowerBound	lower bound of cost interval
#	-u --upperBound	upper bound of cost interval
#	-i --input 	path to input file in .gr format (9th DIMACS Implementation Challenge)
#	-o --output 	path to output file
#	-s --seed	probability of edge cost to be changed
#
function pasreBash() {
	while [[ $# > 0 ]]
	do
		case $1 in
			-l|--lowerBound)
				LB=$2
				shift # past argument
			;;
			-u|--upperBound)
				UB=$2
				shift # past argument
			;;
			-i|--input)
				input_path=$(expandFilePath "$2")
				shift # past argument
			;;
			-o|--output)
				output_path=$(expandFilePath "$2")
				shift # past argument
			;;
			-s|--seed)
				seed=$2
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
	echo "Usage: [sudo] bash randDistance.bash [-l|--lowerBound <arg>] -u|--upperBound <arg> -i|--input <arg> -o|--output <arg> [-s|--seed <arg>]"
	echo "	-l --lowerBound	lower bound of cost interval"
	echo "	-u --upperBound	upper bound of cost interval"
	echo "	-i --input 	path to input file in .gr format (9th DIMACS Implementation Challenge)"
	echo "	-o --output 	path to output file"
	echo "	-s --seed	probability of edge cost to be changed"
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


####################################################################################################################

set -e

pasreBash $@

if [ -z ${UB+x} ] || [ -z ${input_path+x} ] || [ -z ${output_path+x} ]; then 
	printHelp
	exit;
fi

if [ -z ${LB+x} ]; then 
	LB=0
	echo "Lower bound has been set to default value (0)."
fi

if [ -z ${seed+x} ]; then 
	seed=0.5
	echo "Seed value has been set to default value (0.5)."
	echo "Every arc cost will be changed."
fi

perl randDistance.pl "$LB" "$UB" "$input_path" "$output_path" "$seed"
