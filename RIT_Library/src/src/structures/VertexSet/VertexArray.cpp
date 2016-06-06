/*
 * VertexArray.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/VertexSet/VertexArray.hpp"

#include <log4cxx/logger.h>
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "../../../include/exp/LogicExceptions.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VertexArray"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void VertexArray::begin(std::vector<VertexIF*>::const_iterator & iterator) {
	this->vertexIteratorBegin = this->vertices.begin();
	iterator = this->vertices.begin();
	this->vertexIteratorEnd = this->vertices.end();
}

void VertexArray::end(std::vector<VertexIF*>::const_iterator & iterator) {
	this->vertexIteratorBegin = this->vertices.begin();
	iterator = this->vertices.end();
	this->vertexIteratorEnd = this->vertices.end();
}

bool VertexArray::hasNext(std::vector<VertexIF*>::const_iterator & iterator) {
	return iterator != this->vertexIteratorEnd;
}

bool VertexArray::hasNext(std::vector<VertexIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasNext(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool VertexArray::hasPrevious(
		std::vector<VertexIF*>::const_iterator & iterator) {
	return iterator != this->vertexIteratorBegin;
}

bool VertexArray::hasPrevious(std::vector<VertexIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

VertexIF * VertexArray::next(
		std::vector<VertexIF*>::const_iterator & iterator) {
	return *(iterator++);
}

VertexIF * VertexArray::current(
		std::vector<VertexIF*>::const_iterator & iterator) {
	return *(iterator);
}

VertexIF * VertexArray::previous(
		std::vector<VertexIF*>::const_iterator & iterator) {
	return *(--iterator);
}

VertexIF * VertexArray::peek(std::vector<VertexIF*>::const_iterator & iterator,
		int moveIndex) throw (LogicExceptions::EmptyIteratorException) {
	VertexIF* item { };
	std::vector<VertexIF*>::const_iterator tmpIterator = iterator;
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

void VertexArray::createIteratorIfNotExists(IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::vector<VertexIF*>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

VertexArray::VertexArray(VertexSetIF const & vertexArray) :
		VertexSetIF(vertexArray) {
	this->vertices = std::vector<VertexIF*>(
			((VertexArray) vertexArray).vertices);
}

VertexArray::VertexArray() :
		VertexSetIF() {

}

VertexArray::VertexArray(VertexCount numberOfVertices) :
		VertexSetIF(numberOfVertices) {
	this->vertices = std::vector<VertexIF*> { };
	this->vertices.reserve(numberOfVertices);
}

VertexArray::~VertexArray() {
	this->vertices.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void VertexArray::push_back(VertexIF * const & vertex) {
	vertices.push_back(vertex);
}

VertexIF * VertexArray::getElementAt(VertexIdx const vertexIdx) {
	return *(std::next(vertices.begin(), vertexIdx));
}

VertexCount VertexArray::size() const {
	return (VertexCount) this->vertices.size();
}

void VertexArray::begin() {
	begin(this->vertexIterator);
}

void VertexArray::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void VertexArray::end() {
	end(this->vertexIterator);
}

void VertexArray::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool VertexArray::hasNext() {
	return hasNext(this->vertexIterator);
}

bool VertexArray::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool VertexArray::hasNext(Visibility const visibility) {
	return hasNext(this->vertexIterator, visibility);
}

bool VertexArray::hasNext(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), visibility);
}

bool VertexArray::hasPrevious() {
	return hasPrevious(this->vertexIterator);
}

bool VertexArray::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

bool VertexArray::hasPrevious(Visibility const visibility) {
	return hasPrevious(this->vertexIterator, visibility);
}

bool VertexArray::hasPrevious(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), visibility);
}

VertexIF * VertexArray::next() {
	return next(this->vertexIterator);
}

VertexIF * VertexArray::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

VertexIF * VertexArray::current() {
	return current(this->vertexIterator);
}

VertexIF * VertexArray::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

VertexIF * VertexArray::previous() {
	return previous(this->vertexIterator);
}

VertexIF * VertexArray::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

VertexIF * VertexArray::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->vertexIterator, moveIndex);
}

VertexIF * VertexArray::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId VertexArray::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::vector<VertexIF*>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::vector<VertexIF*>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::vector<VertexIF*>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId,
					std::vector<VertexIF*>::const_iterator { }));
	return returnedId;
}

void VertexArray::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//
