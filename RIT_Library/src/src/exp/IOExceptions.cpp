/*
 * IOExceptions.cpp
 *
 *  Created on: 16 mar 2016
 *      Author: tomasz
 */

#include "../../include/exp/IOExceptions.hpp"

#include <cstdio>

IOExceptions::FileNotFountException::FileNotFountException(
		char const * pathToFile) {
	sprintf(buffer, "Cannot open given file: '%s'. File cannot be found.",
			pathToFile);
}

const char* IOExceptions::FileNotFountException::what() const throw () {
	return buffer;
}

const char* IOExceptions::InvalidProblemRead::what() const throw () {
	return "Problem occurred while reading problem line in file with graph data.";
}

const char* IOExceptions::InvalidProblemWrite::what() const throw () {
	return "Problem occurred while writing problem line to graph data file.";
}

const char* IOExceptions::InvalidArcRead::what() const throw () {
	return "Problem occurred while reading arc line in file with graph data.";
}

const char* IOExceptions::InvalidArcWrite::what() const throw () {
	return "Problem occurred while writing arc line to graph data file.";
}
