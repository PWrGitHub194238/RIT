/*
 * VertexIF.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../include/structures/VertexIF.hpp"

#include <log4cxx/logger.h>
#include <iostream>
#include <sstream>

#include "../../include/enums/EdgeByVertexKey.hpp"
#include "../../include/exp/LogicExceptions.hpp"
#include "../../include/structures/EdgeByVertexSetInclude.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VertexIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

VertexIF::VertexIF(VertexIF * vertex) :
		VertexIF::VertexIF(vertex->getVertexIdx(), vertex->getVisibility()) {
}

VertexIF::VertexIF(VertexIdx vertexIdx, Visibility const visibility) {
	this->vertexIdx = vertexIdx;
	this->visibility = visibility;
	this->inputEdges = new EdgeByVertexSetImpl { this,
			EdgeByVertexKey::INCOMING_EDGES };
	this->outputEdges = new EdgeByVertexSetImpl { this,
			EdgeByVertexKey::OUTGOING_EDGES };
}

VertexIF::VertexIF(VertexIdx vertexIdx) :
		VertexIF::VertexIF(vertexIdx, Visibility::VISIBLE) {
}

VertexIF::~VertexIF() {
	delete this->inputEdges;
	delete this->outputEdges;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void VertexIF::addInputEdge(EdgeIF * inputEdge) {
	try {
		this->inputEdges->addEdge(inputEdge);
	} catch (const LogicExceptions::EdgeNullPointerException& e) {
		std::cout << e.what() << std::endl;
	}
}

void VertexIF::addOutputEdge(EdgeIF * outputEdge) {
	this->outputEdges->addEdge(outputEdge);
}

EdgeIF * VertexIF::findInputEdge(VertexIdx const vertexId,
		bool searchOutputIfNoResult) {
	EdgeIF * edge { };
	try {
		edge = this->inputEdges->findEdge(vertexId);
	} catch (std::out_of_range& e) {
		if (searchOutputIfNoResult) {
			edge = this->outputEdges->findEdge(vertexId);
		}
	}
	return edge;
}

EdgeIF * VertexIF::findInputEdge(VertexIdx const vertexId) {
	return this->inputEdges->findEdge(vertexId);
}

EdgeIF * VertexIF::findInputEdge(VertexIF * vertex,
		bool searchOutputIfNoResult) {
	EdgeIF * edge { };
	try {
		edge = this->inputEdges->findEdge(vertex);
	} catch (std::out_of_range& e) {
		if (searchOutputIfNoResult) {
			edge = this->outputEdges->findEdge(vertex);
		}
	}
	return edge;
}

EdgeIF * VertexIF::findInputEdge(VertexIF * vertex) {
	return this->inputEdges->findEdge(vertex);
}

EdgeIF * VertexIF::findOutputEdge(VertexIdx const vertexId,
		bool searchInputIfNoResult) {
	EdgeIF * edge { };
	try {
		edge = this->outputEdges->findEdge(vertexId);
	} catch (std::out_of_range& e) {
		if (searchInputIfNoResult) {
			edge = this->inputEdges->findEdge(vertexId);
		}
	}
	return edge;
}

EdgeIF * VertexIF::findOutputEdge(VertexIdx const vertexId) {
	return this->outputEdges->findEdge(vertexId);
}

EdgeIF * VertexIF::findOutputEdge(VertexIF * vertex,
		bool searchInputIfNoResult) {
	EdgeIF * edge { };
	try {
		edge = this->outputEdges->findEdge(vertex);
	} catch (std::out_of_range& e) {
		if (searchInputIfNoResult) {
			edge = this->inputEdges->findEdge(vertex);
		}
	}
	return edge;
}

EdgeIF * VertexIF::findOutputEdge(VertexIF * vertex) {
	return this->outputEdges->findEdge(vertex);
}

void VertexIF::removeInputEdge(EdgeIF * const inputEdge) {
	this->inputEdges->removeEdge(inputEdge);
}

void VertexIF::removeInputEdge(VertexIdx const vertexIdx) {
	this->inputEdges->removeEdge(vertexIdx);
}

void VertexIF::removeInputEdge(VertexIF * const vertex) {
	this->inputEdges->removeEdge(vertex);
}

void VertexIF::removeOutputEdge(EdgeIF * const outputEdge) {
	this->outputEdges->removeEdge(outputEdge);
}

void VertexIF::removeOutputEdge(VertexIdx const vertexIdx) {
	this->outputEdges->removeEdge(vertexIdx);
}

void VertexIF::removeOutputEdge(VertexIF * const vertex) {
	this->outputEdges->removeEdge(vertex);
}

VertexIF* VertexIF::getRandomSuccessor() {
	EdgeIF* edge = this->outputEdges->getRandomEdge();
	return edge->getOtherVertex(this->vertexIdx);
}

void VertexIF::beginInputEdges() {
	inputEdges->begin();
}

bool VertexIF::hasNextInputEdge() {
	return inputEdges->hasNext();
}

bool VertexIF::hasNextInputEdge(Visibility const visibility) {
	return inputEdges->hasNext(visibility);
}

bool VertexIF::hasAnyInputEdge() {
	return inputEdges->Iterable::hasAny();
}

bool VertexIF::hasAnyInputEdge(Visibility const visibility) {
	return inputEdges->hasAny(visibility);
}

EdgeIF * VertexIF::nextInputEdge() {
	return inputEdges->nextEdge();
}

VertexIF * VertexIF::nextInputEdgeSource() {
	return inputEdges->nextVertex();
}

void VertexIF::beginOutputEdges() {
	outputEdges->begin();
}

bool VertexIF::hasNextOutputEdge() {
	return outputEdges->hasNext();
}

bool VertexIF::hasNextOutputEdge(Visibility const visibility) {
	return outputEdges->hasNext(visibility);
}

bool VertexIF::hasAnyOutputEdge() {
	return outputEdges->Iterable::hasAny();
}

bool VertexIF::hasAnyOutputEdge(Visibility const visibility) {
	return outputEdges->hasAny(visibility);
}

EdgeIF * VertexIF::nextOutputEdge() {
	return outputEdges->nextEdge();
}

EdgeIF * VertexIF::currentOutputEdge() {
	return outputEdges->currentEdge();
}

VertexIF * VertexIF::nextOutputEdgeTarget() {
	return outputEdges->nextVertex();
}

VertexIF * VertexIF::currentOutputEdgeTarget() {
	return outputEdges->currentVertex();
}

void VertexIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	jsonDoc.AddMember("Vertex ID", vertexIdx, allocator);
	jsonDoc.AddMember("Incoming edges",
			JSONUtils::getDepthLimitedJSON(inputEdges, allocator,
					"EdgeByVertexSetIF", depth), allocator);
	jsonDoc.AddMember("Outgoing edges",
			JSONUtils::getDepthLimitedJSON(outputEdges, allocator,
					"EdgeByVertexSetIF", depth), allocator);
}

std::string VertexIF::toString() {
	std::ostringstream oss { };
	oss << "(" << this->getVertexIdx() << ")" << std::flush;
	return oss.str();
}

std::string VertexIF::inputEdgesToString() {
	return this->inputEdges->toString();
}

std::string VertexIF::outputEdgesToString() {
	return this->outputEdges->toString();
}

//*************************************** GETTERS & SETTERS ****************************************//

VertexIdx VertexIF::getVertexIdx() const {
	return this->vertexIdx;
}

VertexCount VertexIF::getNumberOfInputEdges() const {
	return this->inputEdges->size();
}

VertexCount VertexIF::getNumberOfInputEdges(Visibility const visibility) const {
	return this->inputEdges->size(visibility);
}

VertexCount VertexIF::getNumberOfOutputEdges() const {
	return this->outputEdges->size();
}

VertexCount VertexIF::getNumberOfOutputEdges(
		Visibility const visibility) const {
	return this->outputEdges->size(visibility);
}
