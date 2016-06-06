/*
 * LogicExceptions.cpp
 *
 *  Created on: 16 mar 2016
 *      Author: tomasz
 */

#include "../../include/exp/ModelExceptions.hpp"

const unsigned short ModelExceptions::BUFFER_SIZE { 1024 };

const char* ModelExceptions::GeneralConcertException::what() const throw () {
	return "Concert exception.";
}
