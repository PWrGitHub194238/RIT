/*
 * EdgeArray.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/EdgeSet/EdgeArray.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <algorithm>
#include <iterator>
#include <memory>
#include <set>
#include <utility>

#include "../../../include/exp/LogicExceptions.hpp"
#include "../../../include/log/bundle/Bundle.hpp"
#include "../../../include/log/utils/LogUtils.hpp"
#include "../../../include/structures/EdgeInclude.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.EdgeArray"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void EdgeArray::begin(std::vector<EdgeIF*>::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edges.begin();
	iterator = this->edges.begin();
	this->edgeIteratorEnd = this->edges.end();
}

void EdgeArray::end(std::vector<EdgeIF*>::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edges.begin();
	iterator = this->edges.end();
	this->edgeIteratorEnd = this->edges.end();
}

bool EdgeArray::hasNext(std::vector<EdgeIF*>::const_iterator & iterator) {
	return iterator != this->edgeIteratorEnd;
}

bool EdgeArray::hasNext(std::vector<EdgeIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasNext(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeArray::hasNext(std::vector<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity) {
	while (hasNext(iterator)
			&& (connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity))) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeArray::hasNext(std::vector<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity, Visibility const visibility) {
	while (hasNext(iterator)
			&& (((connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity)))
					|| (visibility != Visibility::BOTH
							&& current(iterator)->getVisibility() != visibility))) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeArray::hasPrevious(std::vector<EdgeIF*>::const_iterator & iterator) {
	return iterator != this->edgeIteratorBegin;
}

bool EdgeArray::hasPrevious(std::vector<EdgeIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

bool EdgeArray::hasPrevious(std::vector<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity) {
	while (hasPrevious(iterator)
			&& (connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity))) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

bool EdgeArray::hasPrevious(std::vector<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity, Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (((connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity)))
					|| (visibility != Visibility::BOTH
							&& current(iterator)->getVisibility() != visibility))) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

EdgeIF * EdgeArray::next(std::vector<EdgeIF*>::const_iterator & iterator) {
	return *(iterator++);
}

EdgeIF * EdgeArray::current(std::vector<EdgeIF*>::const_iterator & iterator) {
	return *(iterator);
}

EdgeIF * EdgeArray::previous(std::vector<EdgeIF*>::const_iterator & iterator) {
	return *(--iterator);
}

EdgeIF * EdgeArray::peek(std::vector<EdgeIF*>::const_iterator & iterator,
		int moveIndex) throw (LogicExceptions::EmptyIteratorException) {
	EdgeIF* item { };
	std::vector<EdgeIF*>::const_iterator tmpIterator = iterator;
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

void EdgeArray::createIteratorIfNotExists(IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::vector<EdgeIF*>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

EdgeArray::EdgeArray() :
		EdgeSetIF() {
}

EdgeArray::EdgeArray(EdgeSetIF * edgeArray) :
		EdgeArray(edgeArray, true) {
}

EdgeArray::EdgeArray(EdgeSetIF * edgeArray, bool deepCopy) :
		EdgeSetIF(edgeArray) {
	this->edges.reserve(numberOfEdges);
	edgeArray->begin();
	if (deepCopy) {
	while (edgeArray->hasNext()) {
		this->edges.push_back(new EdgeImpl { edgeArray->next() });
	}
	} else {
		while (edgeArray->hasNext()) {
				this->edges.push_back(edgeArray->next());
			}
	}
}

EdgeArray::EdgeArray(EdgeCount numberOfEdges) :
		EdgeSetIF(numberOfEdges) {
	TRACE(logger, LogBundleKey::EDGE_ARRAY_CONSTRUCTOR, numberOfEdges);
	this->edges.reserve(numberOfEdges);
}

EdgeArray::~EdgeArray() {
	this->edges.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void EdgeArray::push_back(EdgeIF * const & edge) {
	edges.push_back(edge);
}

void EdgeArray::pop_back() {
	edges.pop_back();
}

void EdgeArray::remove(EdgeIF * const & edge) {
	edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
}

EdgeCount EdgeArray::size() const {
	return (EdgeCount) this->edges.size();
}

void EdgeArray::begin() {
	begin(this->edgeIterator);
}

void EdgeArray::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void EdgeArray::end() {
	end(this->edgeIterator);
}

void EdgeArray::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool EdgeArray::hasNext() {
	return hasNext(this->edgeIterator);
}

bool EdgeArray::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool EdgeArray::hasNext(Visibility const visibility) {
	return hasNext(this->edgeIterator, visibility);
}

bool EdgeArray::hasNext(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), visibility);
}

bool EdgeArray::hasNext(Connectivity const connectivity) {
	return hasNext(this->edgeIterator, connectivity);
}

bool EdgeArray::hasNext(IteratorId const iteratorId,
		Connectivity const connectivity) {
	return hasNext(iteratorMap.at(iteratorId), connectivity);
}

bool EdgeArray::hasNext(Connectivity const connectivity,
		Visibility const visibility) {
	return hasNext(this->edgeIterator, connectivity, visibility);
}

bool EdgeArray::hasNext(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), connectivity, visibility);
}

bool EdgeArray::hasPrevious() {
	return hasPrevious(this->edgeIterator);
}

bool EdgeArray::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

bool EdgeArray::hasPrevious(Visibility const visibility) {
	return hasPrevious(this->edgeIterator, visibility);
}

bool EdgeArray::hasPrevious(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), visibility);
}

bool EdgeArray::hasPrevious(Connectivity const connectivity) {
	return hasPrevious(this->edgeIterator, connectivity);
}

bool EdgeArray::hasPrevious(IteratorId const iteratorId,
		Connectivity const connectivity) {
	return hasPrevious(iteratorMap.at(iteratorId), connectivity);
}

bool EdgeArray::hasPrevious(Connectivity const connectivity,
		Visibility const visibility) {
	return hasPrevious(this->edgeIterator, connectivity, visibility);
}

bool EdgeArray::hasPrevious(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), connectivity, visibility);
}

EdgeIF * EdgeArray::next() {
	return next(this->edgeIterator);
}

EdgeIF * EdgeArray::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeArray::current() {
	return current(this->edgeIterator);
}

EdgeIF * EdgeArray::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeArray::previous() {
	return previous(this->edgeIterator);
}

EdgeIF * EdgeArray::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeArray::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->edgeIterator, moveIndex);
}

EdgeIF * EdgeArray::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId EdgeArray::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::vector<EdgeIF*>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::vector<EdgeIF*>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::vector<EdgeIF*>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId,
					std::vector<EdgeIF*>::const_iterator { }));
	return returnedId;
}

void EdgeArray::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//

