/*
 * MODELEXCEPtions.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_EXP_MODELEXCEPTIONS_HPP_
#define SRC_INCLUDE_EXP_MODELEXCEPTIONS_HPP_

#include <exception>

namespace ModelExceptions {

extern const unsigned short BUFFER_SIZE;

class GeneralConcertException: public std::exception {
public:
	virtual const char* what() const throw ();
};

}  // namespace ModelExceptions

#endif /* SRC_INCLUDE_EXP_MODELEXCEPTIONS_HPP_ */
