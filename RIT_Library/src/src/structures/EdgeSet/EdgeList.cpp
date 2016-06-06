/*
 * EdgeList.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/EdgeSet/EdgeList.hpp"

#include <log4cxx/logger.h>
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "../../../include/exp/LogicExceptions.hpp"
#include "../../../include/structures/EdgeInclude.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.EdgeList"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void EdgeList::begin(std::list<EdgeIF*>::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edges.begin();
	iterator = this->edges.begin();
	this->edgeIteratorEnd = this->edges.end();
}

void EdgeList::end(std::list<EdgeIF*>::const_iterator & iterator) {
	this->edgeIteratorBegin = this->edges.begin();
	iterator = this->edges.end();
	this->edgeIteratorEnd = this->edges.end();
}

bool EdgeList::hasNext(std::list<EdgeIF*>::const_iterator & iterator) {
	return iterator != this->edgeIteratorEnd;
}

bool EdgeList::hasNext(std::list<EdgeIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasNext(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeList::hasNext(std::list<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity) {
	while (hasNext(iterator)
			&& (connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity))) {
		next(iterator);
	}
	return hasNext(iterator);
}

bool EdgeList::hasNext(std::list<EdgeIF*>::const_iterator & iterator,
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

bool EdgeList::hasPrevious(std::list<EdgeIF*>::const_iterator & iterator) {
	return iterator != this->edgeIteratorBegin;
}

bool EdgeList::hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
		Visibility const visibility) {
	while (hasPrevious(iterator)
			&& (visibility != Visibility::BOTH
					&& current(iterator)->getVisibility() != visibility)) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

bool EdgeList::hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
		Connectivity const connectivity) {
	while (hasPrevious(iterator)
			&& (connectivity != Connectivity::BOTH
					&& !current(iterator)->isInState(connectivity))) {
		previous(iterator);
	}
	return hasPrevious(iterator);
}

bool EdgeList::hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
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

EdgeIF * EdgeList::next(std::list<EdgeIF*>::const_iterator & iterator) {
	return *(iterator++);
}

EdgeIF * EdgeList::current(std::list<EdgeIF*>::const_iterator & iterator) {
	return *(iterator);
}

EdgeIF * EdgeList::previous(std::list<EdgeIF*>::const_iterator & iterator) {
	return *(--iterator);
}

EdgeIF * EdgeList::peek(std::list<EdgeIF*>::const_iterator & iterator,
		int moveIndex) throw (LogicExceptions::EmptyIteratorException) {
	EdgeIF* item { };
	std::list<EdgeIF*>::const_iterator tmpIterator = iterator;
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

void EdgeList::createIteratorIfNotExists(IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::list<EdgeIF*>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

EdgeList::EdgeList() :
		EdgeSetIF() {
}

EdgeList::EdgeList(EdgeSetIF * edgeList) :
		EdgeList(edgeList, true) {
}

EdgeList::EdgeList(EdgeSetIF * edgeList, bool deepCopy) :
		EdgeList(edgeList) {
	edgeList->begin();
	while (edgeList->hasNext()) {
		this->edges.push_back(
				deepCopy ?
						new EdgeImpl { edgeList->next() } : edgeList->next());
	}
}

EdgeList::EdgeList(EdgeCount numberOfEdges) :
		EdgeSetIF(numberOfEdges) {
	this->edges = std::list<EdgeIF*> { };
}

EdgeList::~EdgeList() {
	this->edges.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void EdgeList::push_back(EdgeIF * const & edge) {
	edges.push_back(edge);
}

void EdgeList::pop_back() {
	edges.pop_back();
}

void EdgeList::remove(EdgeIF * const & edge) {
	edges.remove(edge);
}

EdgeIF * EdgeList::getElementAt(EdgeIdx const edgeIdx) {
	return *(std::next(edges.begin(), edgeIdx));
}

EdgeCount EdgeList::size() const {
	return (EdgeCount) this->edges.size();
}

void EdgeList::begin() {
	begin(this->edgeIterator);
}

void EdgeList::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void EdgeList::end() {
	end(this->edgeIterator);
}

void EdgeList::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool EdgeList::hasNext() {
	return hasNext(this->edgeIterator);
}

bool EdgeList::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool EdgeList::hasNext(Visibility const visibility) {
	return hasNext(this->edgeIterator, visibility);
}

bool EdgeList::hasNext(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), visibility);
}

bool EdgeList::hasNext(Connectivity const connectivity) {
	return hasNext(this->edgeIterator, connectivity);
}

bool EdgeList::hasNext(IteratorId const iteratorId,
		Connectivity const connectivity) {
	return hasNext(iteratorMap.at(iteratorId), connectivity);
}

bool EdgeList::hasNext(Connectivity const connectivity,
		Visibility const visibility) {
	return hasNext(this->edgeIterator, connectivity, visibility);
}

bool EdgeList::hasNext(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	return hasNext(iteratorMap.at(iteratorId), connectivity, visibility);
}

bool EdgeList::hasPrevious() {
	return hasPrevious(this->edgeIterator);
}

bool EdgeList::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

bool EdgeList::hasPrevious(Visibility const visibility) {
	return hasPrevious(this->edgeIterator, visibility);
}

bool EdgeList::hasPrevious(IteratorId const iteratorId,
		Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), visibility);
}

bool EdgeList::hasPrevious(Connectivity const connectivity) {
	return hasPrevious(this->edgeIterator, connectivity);
}

bool EdgeList::hasPrevious(IteratorId const iteratorId,
		Connectivity const connectivity) {
	return hasPrevious(iteratorMap.at(iteratorId), connectivity);
}

bool EdgeList::hasPrevious(Connectivity const connectivity,
		Visibility const visibility) {
	return hasPrevious(this->edgeIterator, connectivity, visibility);
}

bool EdgeList::hasPrevious(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	return hasPrevious(iteratorMap.at(iteratorId), connectivity, visibility);
}

EdgeIF * EdgeList::next() {
	return next(this->edgeIterator);
}

EdgeIF * EdgeList::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeList::current() {
	return current(this->edgeIterator);
}

EdgeIF * EdgeList::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeList::previous() {
	return previous(this->edgeIterator);
}

EdgeIF * EdgeList::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

EdgeIF * EdgeList::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->edgeIterator, moveIndex);
}

EdgeIF * EdgeList::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId EdgeList::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::list<EdgeIF*>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::list<EdgeIF*>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::list<EdgeIF*>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId, std::list<EdgeIF*>::const_iterator { }));
	return returnedId;
}

void EdgeList::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//

