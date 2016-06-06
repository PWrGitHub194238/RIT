/*
 * VertexSetIF.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../include/structures/VertexSetIF.hpp"

#include <log4cxx/logger.h>
#include <rapidjson/rapidjson.h>
#include <sstream>

#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VertexSetIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

VertexSetIF::VertexSetIF() :
		numberOfVertices { 0 } {
}

VertexSetIF::VertexSetIF(VertexCount numberOfVertices) {
	this->numberOfVertices = numberOfVertices;
}

// Empty virtual destructor for proper cleanup
VertexSetIF::~VertexSetIF() {

}

//*************************************** PUBLIC FUNCTIONS *****************************************//

VertexCount VertexSetIF::size(Visibility const visibility) {
	VertexCount setSize { 0 };
	begin();
	while (hasNext(visibility)) {
		setSize += 1;
		next();
	}
	return setSize;
}

VertexCount VertexSetIF::size(IteratorId const iteratorId,
		Visibility const visibility) {
	VertexCount setSize { 0 };
	begin(iteratorId);
	while (hasNext(iteratorId, visibility)) {
		setSize += 1;
		next(iteratorId);
	}
	return setSize;
}

void VertexSetIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	rapidjson::Value vertexSet(rapidjson::kArrayType);

	jsonDoc.AddMember("Number of vertices", numberOfVertices, allocator);

	begin();
	while (hasNext()) {
		vertexSet.PushBack(
				JSONUtils::getDepthLimitedJSON(next(), allocator, "VertexIF",
						depth), allocator);
	}

	jsonDoc.AddMember("Vertex set", vertexSet, allocator);
}

std::string VertexSetIF::toString() {
	std::ostringstream oss { };
	oss << "Set of vertices have " << numberOfVertices << " vertices:"
			<< std::endl;

	begin();
	oss << "\t";
	while (hasNext()) {
		oss << next()->toString() << std::flush;
		oss << (hasNext() ? ", " : "") << std::flush;
	}
	begin();
	return oss.str();
}

//*************************************** GETTERS & SETTERS ****************************************//

