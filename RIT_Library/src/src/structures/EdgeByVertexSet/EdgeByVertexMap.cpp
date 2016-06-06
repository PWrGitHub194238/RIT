/*
 * EdgeByVertexMap.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/EdgeByVertexSet/EdgeByVertexMap.hpp"

#include <log4cxx/logger.h>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <utility>

#include "../../../include/exp/LogicExceptions.hpp"
#include "../../../include/structures/EdgeIF.hpp"
#include "../../../include/structures/VertexIF.hpp"

class EdgeByVertexSetIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.EdgeByVertexMap"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void EdgeByVertexMap::begin(EdgeByVertexIdxMap::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edgeMap.begin();
	iterator = this->edgeMap.begin();
	this->edgeIteratorEnd = this->edgeMap.end();
}

void EdgeByVertexMap::end(EdgeByVertexIdxMap::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edgeMap.begin();
	iterator = this->edgeMap.end();
	this->edgeIteratorEnd = this->edgeMap.end();
}

bool EdgeByVertexMap::hasNext(EdgeByVertexIdxMap::const_iterator & iterator) {
	return iterator != this->edgeIteratorEnd;
}

bool EdgeByVertexMap::hasNext(EdgeByVertexIdxMap::const_iterator & iterator,
		Visibility const visibility) {
	while (hasNext(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator).second->getVisibility() != visibility)) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeByVertexMap::hasPrevious(
		EdgeByVertexIdxMap::const_iterator & iterator) {
	return iterator != this->edgeIteratorBegin;
}

bool EdgeByVertexMap::hasPrevious(EdgeByVertexIdxMap::const_iterator & iterator,
		Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator).second->getVisibility() != visibility)) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::next(
		EdgeByVertexIdxMap::const_iterator & iterator) {
	return *(iterator++);
}

EdgeByVertexIdxPair EdgeByVertexMap::current(
		EdgeByVertexIdxMap::const_iterator & iterator) {
	return *(iterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::previous(
		EdgeByVertexIdxMap::const_iterator & iterator) {
	return *(--iterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::peek(
		EdgeByVertexIdxMap::const_iterator & iterator, int moveIndex)
				throw (LogicExceptions::EmptyIteratorException) {
	EdgeByVertexIdxPair item { };
	EdgeByVertexIdxMap::const_iterator tmpIterator = iterator;
	if (this->numberOfEdges > 0) {
		if (iterator == this->edgeIteratorEnd) {
			iterator = std::prev(iterator);
		}
		if (moveIndex > 0) {
			for (; moveIndex > 0; moveIndex -= 1) {
				if (std::next(iterator) != this->edgeIteratorEnd) {
					iterator++;
				}
			}
		} else {
			for (; moveIndex < 0; moveIndex += 1) {
				if (hasPrevious(iterator)) {
					--iterator;
				}
			}
		}
		item = *(iterator);
		iterator = tmpIterator;
		return item;
	}

	throw LogicExceptions::EmptyIteratorException();
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

void EdgeByVertexMap::createIteratorIfNotExists(IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						EdgeByVertexIdxMap::const_iterator { }));
	}
}

void EdgeByVertexMap::addUndirectedEdge(EdgeIF * const edge) {
	if (edge->getTargetVertex()->getVertexIdx() == this->vertexIdx) {
		edgeMap.insert(
				std::make_pair(edge->getSourceVertex()->getVertexIdx(), edge));
	} else if (edge->getSourceVertex()->getVertexIdx() == this->vertexIdx) {
		edgeMap.insert(
				std::make_pair(edge->getTargetVertex()->getVertexIdx(), edge));
	}
}

void EdgeByVertexMap::addForwardEdge(EdgeIF * const edge) {
	switch (this->key) {
	case EdgeByVertexKey::INCOMING_EDGES:
		if (edge->getTargetVertex()->getVertexIdx() == this->vertexIdx) {
			edgeMap.insert(
					std::make_pair(edge->getSourceVertex()->getVertexIdx(),
							edge));
			/*std::cout << "ADD In[" << this->vertexIdx << "] = "
			 << edge->getSourceVertex()->getVertexIdx() << std::endl;*/
		}
		break;
	case EdgeByVertexKey::OUTGOING_EDGES:
		if (edge->getSourceVertex()->getVertexIdx() == this->vertexIdx) {
			edgeMap.insert(
					std::make_pair(edge->getTargetVertex()->getVertexIdx(),
							edge));
			/*std::cout << "ADD Out[" << this->vertexIdx << "] = "
			 << edge->getTargetVertex()->getVertexIdx() << std::endl;*/

		}
		break;
	default:
		break;
	}
}

void EdgeByVertexMap::addBackwardEdge(EdgeIF * const edge) {
	return addForwardEdge(edge);
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

EdgeByVertexMap::EdgeByVertexMap(VertexIF const * const vertex,
		EdgeByVertexKey const key) :
		EdgeByVertexSetIF(vertex, key) {
	// TODO Auto-generated constructor stub
}

EdgeByVertexMap::~EdgeByVertexMap() {
	this->edgeMap.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeIF * EdgeByVertexMap::findEdge(VertexIdx const vertexId) {
	return edgeMap.at(vertexId);
}

EdgeIF * EdgeByVertexMap::findEdge(VertexIF * const vertex) {
	return findEdge((vertex->getVertexIdx()));
}

void EdgeByVertexMap::removeEdge(EdgeIF * const edge) {
	edgeMap.erase(
			((this->vertexIdx == edge->getTargetVertex()->getVertexIdx()) ?
					edge->getSourceVertex()->getVertexIdx() :
					edge->getTargetVertex()->getVertexIdx()));
}

void EdgeByVertexMap::removeEdge(VertexIdx const vertexIdx) {
	edgeMap.erase(vertexIdx);
}

void EdgeByVertexMap::removeEdge(VertexIF * const vertex) {
	removeEdge(vertex->getVertexIdx());
}

EdgeCount EdgeByVertexMap::size() const {
	return (EdgeCount) this->edgeMap.size();
}

EdgeIF* EdgeByVertexMap::getRandomEdge() {
	EdgeByVertexIdxMap::const_iterator tmpIterator = this->edgeMap.begin();
	std::advance(tmpIterator, std::rand() % this->edgeMap.size());
	return (*tmpIterator).second;
}

EdgeIF* EdgeByVertexMap::getRandomEdge(std::set<VertexIdx> excludedList) {
	EdgeByVertexIdxMap::const_iterator tmpIterator = this->edgeMap.begin();
	VertexIdx idx = std::rand() % this->edgeMap.size();
	while (excludedList.count(idx) > 0) {
		idx = std::rand() % this->edgeMap.size();
	}
	std::advance(tmpIterator, idx);
	return (*tmpIterator).second;
}

void EdgeByVertexMap::begin() {
	begin(this->edgeIterator);
}

void EdgeByVertexMap::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void EdgeByVertexMap::end() {
	end(this->edgeIterator);
}

void EdgeByVertexMap::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool EdgeByVertexMap::hasNext() {
	return hasNext(this->edgeIterator);
}

bool EdgeByVertexMap::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool EdgeByVertexMap::hasNext(Visibility const visibility) {
	return hasNext(this->edgeIterator, visibility);
}

bool EdgeByVertexMap::hasNext(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), visibility);
}

bool EdgeByVertexMap::hasPrevious() {
	return hasPrevious(this->edgeIterator);
}

bool EdgeByVertexMap::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

bool EdgeByVertexMap::hasPrevious(Visibility const visibility) {
	return hasPrevious(this->edgeIterator, visibility);
}

bool EdgeByVertexMap::hasPrevious(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), visibility);
}

EdgeByVertexIdxPair EdgeByVertexMap::next() {
	return next(this->edgeIterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

EdgeByVertexIdxPair EdgeByVertexMap::current() {
	return current(this->edgeIterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

EdgeByVertexIdxPair EdgeByVertexMap::previous() {
	return previous(this->edgeIterator);
}

EdgeByVertexIdxPair EdgeByVertexMap::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

EdgeByVertexIdxPair EdgeByVertexMap::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->edgeIterator, moveIndex);
}

EdgeByVertexIdxPair EdgeByVertexMap::peek(IteratorId const iteratorId,
		int moveIndex) throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId EdgeByVertexMap::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, EdgeByVertexIdxMap::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, EdgeByVertexIdxMap::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							EdgeByVertexIdxMap::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId, EdgeByVertexIdxMap::const_iterator { }));
	return returnedId;
}

void EdgeByVertexMap::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//

