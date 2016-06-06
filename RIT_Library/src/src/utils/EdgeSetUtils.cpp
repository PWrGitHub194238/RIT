/*
 * EdgeSetUtils.cpp
 *
 *  Created on: 17 kwi 2016
 *      Author: tomasz
 */

#include "../../include/utils/EdgeSetUtils.hpp"

#include <log4cxx/logger.h>
#include <iostream>
#include <map>
#include <utility>

#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/typedefs/primitive.hpp"
#include "../../include/utils/MemoryUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.EdgeSetUtils"));

EdgeSetIF* EdgeSetUtils::getSetUnion(EdgeSetIF* const firstEdgeSet,
		EdgeSetIF* const secondEdgeSet, bool deleteSets) {
	EdgeIF* edge { };
	std::map<EdgeIdx, EdgeIF*> tmpStorage { };
	EdgeSetIF* unionSet = new EdgeSetImpl { };
	IteratorId edgeIteratorId = firstEdgeSet->getIterator();
	firstEdgeSet->begin(edgeIteratorId);
	while (firstEdgeSet->hasNext(edgeIteratorId)) {
		edge = firstEdgeSet->next(edgeIteratorId);
		tmpStorage.insert(std::make_pair(edge->getEdgeIdx(), edge));
	}
	firstEdgeSet->removeIterator(edgeIteratorId);

	edgeIteratorId = secondEdgeSet->getIterator();
	secondEdgeSet->begin(edgeIteratorId);
	while (secondEdgeSet->hasNext(edgeIteratorId)) {
		edge = secondEdgeSet->next(edgeIteratorId);
		if (tmpStorage.count(edge->getEdgeIdx()) == 0) {
			tmpStorage.insert(std::make_pair(edge->getEdgeIdx(), edge));
		}
	}
	secondEdgeSet->removeIterator(edgeIteratorId);
	for (auto& unionEdge : tmpStorage) {
		unionSet->push_back(unionEdge.second);
	}
	if (deleteSets) {
		MemoryUtils::removeCollection(firstEdgeSet, false);
		MemoryUtils::removeCollection(secondEdgeSet, false);
	}
	return unionSet;
}

EdgeSetIF* EdgeSetUtils::getSetUnion(EdgeSetIF* const firstEdgeSet,
		EdgeSetIF* const secondEdgeSet) {
	return getSetUnion(firstEdgeSet, secondEdgeSet, false);
}

EdgeSetIF* EdgeSetUtils::getSetComplement(EdgeSetIF* const firstEdgeSet,
		EdgeSetIF* const secondEdgeSet, bool deleteSets) {
	EdgeIF* edge { };
	std::map<EdgeIdx, EdgeIF*> tmpStorage { };
	EdgeSetIF* complementSet = new EdgeSetImpl { };
	IteratorId edgeIteratorId = firstEdgeSet->getIterator();
	firstEdgeSet->begin(edgeIteratorId);
	while (firstEdgeSet->hasNext(edgeIteratorId)) {
		edge = firstEdgeSet->next(edgeIteratorId);
		tmpStorage.insert(std::make_pair(edge->getEdgeIdx(), edge));
	}
	firstEdgeSet->removeIterator(edgeIteratorId);

	edgeIteratorId = secondEdgeSet->getIterator();
	secondEdgeSet->begin(edgeIteratorId);
	while (secondEdgeSet->hasNext(edgeIteratorId)) {
		edge = secondEdgeSet->next(edgeIteratorId);
		tmpStorage.erase(edge->getEdgeIdx());
	}
	secondEdgeSet->removeIterator(edgeIteratorId);
	for (auto& complementEdge : tmpStorage) {
		complementSet->push_back(complementEdge.second);
	}
	if (deleteSets) {
		MemoryUtils::removeCollection(firstEdgeSet, false);
		MemoryUtils::removeCollection(secondEdgeSet, false);
	}
	return complementSet;
}

EdgeSetIF* EdgeSetUtils::getSetComplement(EdgeSetIF* const firstEdgeSet,
		EdgeSetIF* const secondEdgeSet) {
	return getSetComplement(firstEdgeSet, secondEdgeSet, false);
}
