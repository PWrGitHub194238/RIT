/*
 * VertexHeapItem.cpp
 *
 *  Created on: 11 sty 2016
 *      Author: tomasz
 */

#include "../../include/heap/VertexHeapItem.hpp"

#include <log4cxx/logger.h>
#include <limits>

#include "../../include/structures/VertexIF.hpp"
#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("heap.VertexHeapItem"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

VertexHeapItem::VertexHeapItem(VertexIF * vertex) :
		VertexHeapItem(vertex, std::numeric_limits<VertexKey>::max()) {
}

VertexHeapItem::VertexHeapItem(VertexIF * vertex, VertexKey const key) :
		VertexHeapItem(vertex, key, nullptr) {
}

VertexHeapItem::VertexHeapItem(VertexIF * vertex, VertexKey const key,
		VertexIF * predecessor) {
	this->key = key;
	this->value = vertex;
	this->predecessor = predecessor;
}

VertexHeapItem::~VertexHeapItem() {
	// TODO Auto-generated destructor stub
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void VertexHeapItem::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {

	jsonDoc.AddMember("key", key, allocator);
	jsonDoc.AddMember("value",
			JSONUtils::getDepthLimitedJSON(value, allocator, "VertexIF", depth),
			allocator);
}

//*************************************** GETTERS & SETTERS ****************************************//

VertexKey VertexHeapItem::getKey() const {
	return this->key;
}

void VertexHeapItem::setKey(VertexKey key) {
	this->key = key;
}

VertexIF * VertexHeapItem::getValue() const {
	return this->value;
}

VertexIF * VertexHeapItem::getPredecessor() const {
	return this->predecessor;
}

void VertexHeapItem::setPredecessor(VertexIF * predecessor) {
	this->predecessor = predecessor;
}
