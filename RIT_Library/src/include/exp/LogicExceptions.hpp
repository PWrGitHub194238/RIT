/*
 * LogicExceptions.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_EXP_LOGICEXCEPTIONS_HPP_
#define SRC_INCLUDE_EXP_LOGICEXCEPTIONS_HPP_

#include <exception>

namespace LogicExceptions {

extern const unsigned short BUFFER_SIZE;

class EdgeNullPointerException: public std::exception {
public:
	virtual const char* what() const throw ();
};

class VertexNotFoundException: public std::exception {
public:
	virtual const char* what() const throw ();
};

class EdgeNotFoundException: public std::exception {
public:
	virtual const char* what() const throw ();
};

class EmptyStructuredException: public std::exception {
public:
	virtual const char* what() const throw ();
};

class EmptyIteratorException: public std::exception {
public:
	virtual const char* what() const throw ();
};

}  // namespace LogicExceptions

#endif /* SRC_INCLUDE_EXP_LOGICEXCEPTIONS_HPP_ */
