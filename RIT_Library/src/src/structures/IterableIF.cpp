/*
 * IterableIF.cpp
 *
 *  Created on: 11 lut 2016
 *      Author: tomasz
 */

#include "../../include/structures/IterableIF.hpp"

#include <log4cxx/logger.h>

#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/struct.hpp"

class VertexIF;
class EdgeIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.IterableIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

//*************************************** PUBLIC FUNCTIONS *****************************************//

template<typename Item>
bool Iterable<Item>::hasAny() {
	this->begin();
	return this->hasNext();
}

template<typename Item>
bool Iterable<Item>::hasAny(IteratorId const iteratorId) {
	this->begin(iteratorId);
	return this->hasNext(iteratorId);
}

template bool Iterable<EdgeByVertexIdxPair>::hasAny();
template bool Iterable<EdgeByVertexIdxPair>::hasAny(
		IteratorId const iteratorId);
template bool Iterable<VertexIF*>::hasAny();
template bool Iterable<VertexIF*>::hasAny(IteratorId const iteratorId);
template bool Iterable<EdgeIF*>::hasAny();
template bool Iterable<EdgeIF*>::hasAny(IteratorId const iteratorId);

//*************************************** GETTERS & SETTERS ****************************************//
