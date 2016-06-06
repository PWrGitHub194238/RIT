/*
 * LogicExceptions.cpp
 *
 *  Created on: 16 mar 2016
 *      Author: tomasz
 */

#include "../../include/exp/LogicExceptions.hpp"

const unsigned short LogicExceptions::BUFFER_SIZE { 1024 };

const char* LogicExceptions::EdgeNullPointerException::what() const throw () {
	return "Cannot perform operation on null edge.";
}

const char* LogicExceptions::VertexNotFoundException::what() const throw () {
	return "Cannot find vertex with given properties.";
}

const char* LogicExceptions::EdgeNotFoundException::what() const throw () {
	return "Cannot find edge with given properties.";
}

const char* LogicExceptions::EmptyStructuredException::what() const throw () {
	return "Cannot perform operation on empty data structure.";
}

const char* LogicExceptions::EmptyIteratorException::what() const throw () {
	return "Cannot perform operation on iterator to empty structure.";
}
