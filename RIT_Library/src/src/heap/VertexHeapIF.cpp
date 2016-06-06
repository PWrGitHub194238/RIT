/*
 * VertexHeapIF.cpp
 *
 *  Created on: 13 sty 2016
 *      Author: tomasz
 */

#include "../../include/heap/VertexHeapIF.hpp"

#include <log4cxx/logger.h>

#include "../../include/heap/VertexHeapItem.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("heap.VertexHeapIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

// Empty virtual destructor for proper cleanup
VertexHeapIF::~VertexHeapIF() {
	// TODO Auto-generated destructor stub
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

bool VertexHeapIF::compare(VertexHeapItem * const vertexU,
		VertexHeapItem * const vertexV) {
	return vertexU->getKey() > vertexV->getKey();
}

//*************************************** GETTERS & SETTERS ****************************************//

