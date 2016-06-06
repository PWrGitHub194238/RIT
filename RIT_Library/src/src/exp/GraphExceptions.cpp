/*
 * GraphExceptions.cpp
 *
 *  Created on: 16 mar 2016
 *      Author: tomasz
 */

#include "../../include/exp/GraphExceptions.hpp"

const unsigned short GraphExceptions::BUFFER_SIZE { 1024 };

const char* GraphExceptions::DisconnectedGraphException::what() const throw () {
	return "Cannot perform operation on non-connected graph.";
}
