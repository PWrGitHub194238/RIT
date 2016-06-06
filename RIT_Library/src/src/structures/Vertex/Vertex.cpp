/*
 * Vertex.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/Vertex/Vertex.hpp"

#include <log4cxx/logger.h>

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.Vertex"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

Vertex::Vertex(VertexIF* vertex) : VertexIF(vertex) {

}

Vertex::Vertex(VertexIdx vertexIdx, Visibility visibility) :
		VertexIF(vertexIdx, visibility) {
}

Vertex::Vertex(VertexIdx vertexIdx) :
		VertexIF(vertexIdx) {
}

Vertex::~Vertex() {
	// TODO Auto-generated destructor stub
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

