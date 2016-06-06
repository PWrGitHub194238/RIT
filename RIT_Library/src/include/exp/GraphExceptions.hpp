/*
 * GraphExceptions.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_EXP_GRAPHEXCEPTIONS_HPP_
#define SRC_INCLUDE_EXP_GRAPHEXCEPTIONS_HPP_

#include <exception>

namespace GraphExceptions {

extern const unsigned short BUFFER_SIZE;

class DisconnectedGraphException: public std::exception {
public:
	virtual const char* what() const throw ();
};

}  // namespace GraphExceptions

#endif /* SRC_INCLUDE_EXP_GRAPHEXCEPTIONS_HPP_ */
