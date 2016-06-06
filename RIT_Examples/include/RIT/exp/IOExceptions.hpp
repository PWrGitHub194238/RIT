/*
 * InputExceptions.hpp
 *
 *  Created on: 7 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_EXP_IOEXCEPTIONS_HPP_
#define SRC_INCLUDE_EXP_IOEXCEPTIONS_HPP_

#include <exception>

namespace IOExceptions {

static const unsigned short BUFFER_SIZE { 1024 };

class FileNotFountException: public std::exception {
	char buffer[BUFFER_SIZE];
public:
	FileNotFountException(char const * pathToFile);
	virtual const char* what() const throw ();
};

class InvalidProblemRead: public std::exception {
	virtual const char* what() const throw ();
};

class InvalidProblemWrite: public std::exception {
	virtual const char* what() const throw ();
};

class InvalidArcRead: public std::exception {
	virtual const char* what() const throw ();
};

class InvalidArcWrite: public std::exception {
	virtual const char* what() const throw ();
};

}  // namespace IOExceptions

#endif /* SRC_INCLUDE_EXP_IOEXCEPTIONS_HPP_ */
