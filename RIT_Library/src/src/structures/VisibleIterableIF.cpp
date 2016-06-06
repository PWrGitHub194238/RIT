/*
 * VisibleIterableIF.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#include "../../include/structures/VisibleIterableIF.hpp"

#include <log4cxx/logger.h>
#include <list>

#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/VertexIF.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VisibleIterableIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

//*************************************** PUBLIC FUNCTIONS *****************************************//

template<typename Item>
bool VisibleIterable<Item>::hasAny(Visibility const visibility) {
	this->begin();
	return hasNext(visibility);
}

template<typename Item>
bool VisibleIterable<Item>::hasAny(IteratorId const iteratorId,
		Visibility const visibility) {
	this->begin(iteratorId);
	return hasNext(iteratorId, visibility);
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::hideAll() {
	this->begin();
	while (this->hasNext(Visibility::VISIBLE)) {
		this->next().second->hide();
	}
}

template<typename Item>
void VisibleIterable<Item>::hideAll() {
	this->begin();
	while (this->hasNext(Visibility::VISIBLE)) {
		this->next()->hide();
	}
}

template<>
VisibilityList VisibleIterable<EdgeByVertexIdxPair>::storeVisibility() {
	VisibilityList visibilityList { };
	this->begin();
	while (this->hasNext(Visibility::BOTH)) {
		visibilityList.push_back(this->next().second->isVisible());
	}
	return visibilityList;
}

template<typename Item>
VisibilityList VisibleIterable<Item>::storeVisibility() {
	VisibilityList visibilityList { };
	this->begin();
	while (this->hasNext(Visibility::BOTH)) {
		visibilityList.push_back(this->next()->isVisible());
	}
	return visibilityList;
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::hideAll(
		IteratorId const iteratorId) {
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::VISIBLE)) {
		this->next(iteratorId).second->hide();
	}
}

template<typename Item>
void VisibleIterable<Item>::hideAll(IteratorId const iteratorId) {
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::VISIBLE)) {
		this->next(iteratorId)->hide();
	}
}

template<>
VisibilityList VisibleIterable<EdgeByVertexIdxPair>::storeVisibility(
		IteratorId const iteratorId) {
	EdgeByVertexIdxPair item { };
	VisibilityList visibilityList { };
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::BOTH)) {
		visibilityList.push_back(this->next(iteratorId).second->isVisible());
	}
	return visibilityList;
}

template<typename Item>
VisibilityList VisibleIterable<Item>::storeVisibility(
		IteratorId const iteratorId) {
	VisibilityList visibilityList { };
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::BOTH)) {
		visibilityList.push_back(this->next(iteratorId)->isVisible());
	}
	return visibilityList;
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::restoreVisibilityAll(
		VisibilityList const & visibilityList) {
	VisibilityList::const_iterator itBegin = visibilityList.begin();
	this->begin();
	while (this->hasNext(Visibility::BOTH)) {
		if (*itBegin == true) {
			this->next().second->show();
		} else {
			this->next().second->hide();
		}
		++itBegin;
	}
}

template<typename Item>
void VisibleIterable<Item>::restoreVisibilityAll(
		VisibilityList const & visibilityList) {
	VisibilityList::const_iterator itBegin = visibilityList.begin();
	this->begin();
	while (this->hasNext(Visibility::BOTH)) {
		if (*itBegin == true) {
			this->next()->show();
		} else {
			this->next()->hide();
		}
		++itBegin;
	}
}

template<typename Item>
void VisibleIterable<Item>::showAll() {
	this->begin();
	while (this->hasNext(Visibility::HIDDEN)) {
		this->next()->show();
	}
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::restoreVisibilityAll(
		VisibilityList const & visibilityList, IteratorId const iteratorId) {
	VisibilityList::const_iterator itBegin = visibilityList.begin();
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::BOTH)) {
		if (*itBegin == true) {
			this->next(iteratorId).second->show();
		} else {
			this->next(iteratorId).second->hide();
		}
		++itBegin;
	}
}

template<typename Item>
void VisibleIterable<Item>::restoreVisibilityAll(
		VisibilityList const & visibilityList, IteratorId const iteratorId) {
	VisibilityList::const_iterator itBegin = visibilityList.begin();
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::BOTH)) {
		if (*itBegin == true) {
			this->next(iteratorId)->show();
		} else {
			this->next(iteratorId)->hide();
		}
		++itBegin;
	}
}

template<typename Item>
void VisibleIterable<Item>::showAll(IteratorId const iteratorId) {
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::HIDDEN)) {
		this->next(iteratorId)->show();
	}
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::showAll() {
	this->begin();
	while (this->hasNext(Visibility::HIDDEN)) {
		this->next().second->show();
	}
}

template<>
void VisibleIterable<EdgeByVertexIdxPair>::showAll(
		IteratorId const iteratorId) {
	this->begin(iteratorId);
	while (this->hasNext(iteratorId, Visibility::HIDDEN)) {
		this->next(iteratorId).second->show();
	}
}

template bool VisibleIterable<EdgeByVertexIdxPair>::hasAny(
		Visibility const visibility);
template bool VisibleIterable<EdgeByVertexIdxPair>::hasAny(
		IteratorId const iteratorId, Visibility const visibility);
template bool VisibleIterable<VertexIF*>::hasAny(Visibility const visibility);
template bool VisibleIterable<VertexIF*>::hasAny(IteratorId const iteratorId,
		Visibility const visibility);
template bool VisibleIterable<EdgeIF*>::hasAny(Visibility const visibility);
template bool VisibleIterable<EdgeIF*>::hasAny(IteratorId const iteratorId,
		Visibility const visibility);

template void VisibleIterable<EdgeByVertexIdxPair>::hideAll();
template void VisibleIterable<EdgeByVertexIdxPair>::hideAll(
		IteratorId const iteratorId);
template void VisibleIterable<VertexIF*>::hideAll();
template void VisibleIterable<VertexIF*>::hideAll(IteratorId const iteratorId);
template void VisibleIterable<EdgeIF*>::hideAll();
template void VisibleIterable<EdgeIF*>::hideAll(IteratorId const iteratorId);

template VisibilityList VisibleIterable<EdgeByVertexIdxPair>::storeVisibility();
template VisibilityList VisibleIterable<EdgeByVertexIdxPair>::storeVisibility(
		IteratorId const iteratorId);
template VisibilityList VisibleIterable<VertexIF*>::storeVisibility();
template VisibilityList VisibleIterable<VertexIF*>::storeVisibility(
		IteratorId const iteratorId);
template VisibilityList VisibleIterable<EdgeIF*>::storeVisibility();
template VisibilityList VisibleIterable<EdgeIF*>::storeVisibility(
		IteratorId const iteratorId);

template void VisibleIterable<EdgeByVertexIdxPair>::restoreVisibilityAll(
		VisibilityList const & visibilityList);
template void VisibleIterable<EdgeByVertexIdxPair>::restoreVisibilityAll(
		VisibilityList const & visibilityList, IteratorId const iteratorId);
template void VisibleIterable<VertexIF*>::restoreVisibilityAll(
		VisibilityList const & visibilityList);
template void VisibleIterable<VertexIF*>::restoreVisibilityAll(
		VisibilityList const & visibilityList, IteratorId const iteratorId);
template void VisibleIterable<EdgeIF*>::restoreVisibilityAll(
		VisibilityList const & visibilityList);
template void VisibleIterable<EdgeIF*>::restoreVisibilityAll(
		VisibilityList const & visibilityList, IteratorId const iteratorId);

template void VisibleIterable<EdgeByVertexIdxPair>::showAll();
template void VisibleIterable<EdgeByVertexIdxPair>::showAll(
		IteratorId const iteratorId);
template void VisibleIterable<VertexIF*>::showAll();
template void VisibleIterable<VertexIF*>::showAll(IteratorId const iteratorId);
template void VisibleIterable<EdgeIF*>::showAll();
template void VisibleIterable<EdgeIF*>::showAll(IteratorId const iteratorId);

//*************************************** GETTERS & SETTERS ****************************************//

