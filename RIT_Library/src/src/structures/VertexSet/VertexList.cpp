/*
 * VertexList.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/VertexSet/VertexList.hpp"

#include <log4cxx/logger.h>
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "../../../include/exp/LogicExceptions.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VertexList"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void VertexList::begin(std::list<VertexIF*>::const_iterator & iterator) {
	this->vertexIteratorBegin = this->vertices.begin();
	iterator = this->vertices.begin();
	this->vertexIteratorEnd = this->vertices.end();
}

void VertexList::end(std::list<VertexIF*>::const_iterator & iterator) {
	this->vertexIteratorBegin = this->vertices.begin();
	iterator = this->vertices.end();
	this->vertexIteratorEnd = this->vertices.end();
}

bool VertexList::hasNext(std::list<VertexIF*>::const_iterator & iterator) {
	return iterator != this->vertexIteratorEnd;
}

bool VertexList::hasNext(std::list<VertexIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasNext(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool VertexList::hasPrevious(std::list<VertexIF*>::const_iterator & iterator) {
	return iterator != this->vertexIteratorBegin;
}

bool VertexList::hasPrevious(std::list<VertexIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

VertexIF * VertexList::next(std::list<VertexIF*>::const_iterator & iterator) {
	return *(iterator++);
}

VertexIF * VertexList::current(
		std::list<VertexIF*>::const_iterator & iterator) {
	return *(iterator);
}

VertexIF * VertexList::previous(
		std::list<VertexIF*>::const_iterator & iterator) {
	return *(--iterator);
}

VertexIF * VertexList::peek(std::list<VertexIF*>::const_iterator & iterator,
		int moveIndex) throw (LogicExceptions::EmptyIteratorException) {
	VertexIF* item { };
	std::list<VertexIF*>::const_iterator tmpIterator = iterator;
	if (this->numberOfVertices > 0) {
		if (iterator == this->vertexIteratorEnd) {
			iterator = std::prev(iterator);
		}
		if (moveIndex > 0) {
			for (; moveIndex > 0; moveIndex -= 1) {
				if (std::next(iterator) != this->vertexIteratorEnd) {
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

void VertexList::createIteratorIfNotExists(IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::list<VertexIF*>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

VertexList::VertexList(VertexSetIF const & vertexList) :
		VertexSetIF(vertexList) {
	this->vertices = std::list<VertexIF*>(((VertexList) vertexList).vertices);
}

VertexList::VertexList() :
		VertexSetIF() {

}

VertexList::VertexList(VertexCount numberOfVertices) :
		VertexSetIF(numberOfVertices) {
	this->vertices = std::list<VertexIF*> { };
}

VertexList::~VertexList() {
	this->vertices.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void VertexList::push_back(VertexIF * const & vertex) {
	vertices.push_back(vertex);
}

VertexIF * VertexList::getElementAt(VertexIdx const vertexIdx) {
	return *(std::next(vertices.begin(), vertexIdx));
}

VertexCount VertexList::size() const {
	return (VertexCount) this->vertices.size();
}

void VertexList::begin() {
	begin(this->vertexIterator);
}

void VertexList::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void VertexList::end() {
	end(this->vertexIterator);
}

void VertexList::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool VertexList::hasNext() {
	return hasNext(this->vertexIterator);
}

bool VertexList::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool VertexList::hasNext(Visibility const visibility) {
	return hasNext(this->vertexIterator, visibility);
}

bool VertexList::hasNext(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), visibility);
}

bool VertexList::hasPrevious() {
	return hasPrevious(this->vertexIterator);
}

bool VertexList::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

bool VertexList::hasPrevious(Visibility const visibility) {
	return hasPrevious(this->vertexIterator, visibility);
}

bool VertexList::hasPrevious(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), visibility);
}

VertexIF * VertexList::next() {
	return next(this->vertexIterator);
}

VertexIF * VertexList::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

VertexIF * VertexList::current() {
	return current(this->vertexIterator);
}

VertexIF * VertexList::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

VertexIF * VertexList::previous() {
	return previous(this->vertexIterator);
}

VertexIF * VertexList::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

VertexIF * VertexList::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->vertexIterator, moveIndex);
}

VertexIF * VertexList::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId VertexList::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::list<VertexIF*>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::list<VertexIF*>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::list<VertexIF*>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId,
					std::list<VertexIF*>::const_iterator { }));
	return returnedId;
}

void VertexList::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//

