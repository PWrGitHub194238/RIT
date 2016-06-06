/*
 * GraphEdgeCostsList.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../../include/structures/GraphEdgeCosts/GraphEdgeCostsList.hpp"

#include <log4cxx/logger.h>
#include <algorithm>
#include <iterator>
#include <set>
#include <utility>

#include "../../../include/bundle/Bundle.hpp"
#include "../../../include/exp/LogicExceptions.hpp"
#include "../../../include/structures/EdgeIF.hpp"
#include "../../../include/structures/EdgeSetIF.hpp"
#include "../../../include/structures/GraphIF.hpp"
#include "../../../include/utils/BundleUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.GraphEdgeCostsList"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void GraphEdgeCostsList::begin(std::list<EdgeCost>::const_iterator & iterator) {
	this->edgeCostsIteratorBegin = this->edgeCosts.begin();
	iterator = this->edgeCosts.begin();
	this->edgeCostsIteratorEnd = this->edgeCosts.end();
}

void GraphEdgeCostsList::end(std::list<EdgeCost>::const_iterator & iterator) {
	this->edgeCostsIteratorBegin = this->edgeCosts.begin();
	iterator = this->edgeCosts.end();
	this->edgeCostsIteratorEnd = this->edgeCosts.end();
}

bool GraphEdgeCostsList::hasNext(
		std::list<EdgeCost>::const_iterator & iterator) {
	return iterator != this->edgeCostsIteratorEnd;
}

bool GraphEdgeCostsList::hasPrevious(
		std::list<EdgeCost>::const_iterator & iterator) {
	return iterator != this->edgeCostsIteratorBegin;
}

EdgeCost GraphEdgeCostsList::next(
		std::list<EdgeCost>::const_iterator & iterator) {
	return *(iterator++);
}

EdgeCost GraphEdgeCostsList::current(
		std::list<EdgeCost>::const_iterator & iterator) {
	return *(iterator);
}

EdgeCost GraphEdgeCostsList::previous(
		std::list<EdgeCost>::const_iterator & iterator) {
	return *(--iterator);
}

EdgeCost GraphEdgeCostsList::peek(
		std::list<EdgeCost>::const_iterator & iterator, int moveIndex)
				throw (LogicExceptions::EmptyIteratorException) {
	EdgeCost item { };
	std::list<EdgeCost>::const_iterator tmpIterator = iterator;
	if (this->numberOfEdges > 0) {
		if (iterator == this->edgeCostsIteratorEnd) {
			iterator = std::prev(iterator);
		}
		if (moveIndex > 0) {
			for (; moveIndex > 0; moveIndex -= 1) {
				if (std::next(iterator) != this->edgeCostsIteratorEnd) {
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

void GraphEdgeCostsList::createIteratorIfNotExists(
		IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::list<EdgeCost>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

GraphEdgeCostsList::GraphEdgeCostsList() :
		GraphEdgeCostsIF() {

}

GraphEdgeCostsList::GraphEdgeCostsList(std::string scenarioName) :
		GraphEdgeCostsIF(scenarioName) {

}

GraphEdgeCostsList::GraphEdgeCostsList(GraphEdgeCostsIF * graphEdgeCosts) :
		GraphEdgeCostsList(graphEdgeCosts,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsList::GraphEdgeCostsList(GraphEdgeCostsIF * graphEdgeCosts,
		std::string scenarioName) {
	this->name = scenarioName;
	this->edgeCosts = std::list<EdgeCost> { };
	graphEdgeCosts->begin();
	while (graphEdgeCosts->hasNext()) {
		this->edgeCosts.push_back(graphEdgeCosts->next());
	}
}

GraphEdgeCostsList::GraphEdgeCostsList(GraphIF* const graph) :
		GraphEdgeCostsList(graph,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsList::GraphEdgeCostsList(GraphIF* const graph,
		std::string scenarioName) :
		GraphEdgeCostsIF(graph, scenarioName) {
	this->edgeCosts = std::list<EdgeCost> { };
	graph->beginEdge();
	while (graph->hasNextEdge()) {
		this->edgeCosts.push_back(graph->nextEdge()->getEdgeCost());
	}
}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeSetIF* const edgeSet) :
		GraphEdgeCostsList(edgeSet,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeSetIF* const edgeSet,
		std::string scenarioName) :
		GraphEdgeCostsIF(edgeSet, scenarioName) {
	this->edgeCosts = std::list<EdgeCost> { };
	edgeSet->begin();
	while (edgeSet->hasNext()) {
		this->edgeCosts.push_back(edgeSet->next()->getEdgeCost());
	}
}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeCount numberOfEdges) :
		GraphEdgeCostsList(numberOfEdges,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeCount numberOfEdges,
		std::string scenarioName) :
		GraphEdgeCostsIF(numberOfEdges, scenarioName) {
	this->edgeCosts = std::list<EdgeCost>(numberOfEdges, 0);
}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeCount numberOfEdges,
		bool fillWithZeros) :
		GraphEdgeCostsList(numberOfEdges,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME),
				fillWithZeros) {

}

GraphEdgeCostsList::GraphEdgeCostsList(EdgeCount numberOfEdges,
		std::string scenarioName, bool fillWithZeros) :
		GraphEdgeCostsIF(numberOfEdges, scenarioName) {
	if (fillWithZeros) {
		this->edgeCosts = std::list<EdgeCost>(numberOfEdges, 0);
	} else {
		this->edgeCosts = std::list<EdgeCost>();
	}
}

GraphEdgeCostsList::~GraphEdgeCostsList() {
	this->edgeCosts.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeCost& GraphEdgeCostsList::operator[](EdgeIdx const edgeIdx) {
	return *(std::next(this->edgeCosts.begin(), edgeIdx));
}

void GraphEdgeCostsList::push_back(EdgeCost edgecost) {
	this->edgeCosts.push_back(edgecost);
}

EdgeCost GraphEdgeCostsList::at(EdgeIdx edgeIdx) {
	return *(std::next(this->edgeCosts.begin(), edgeIdx));
}

EdgeCount GraphEdgeCostsList::size() const {
	return (EdgeCount) this->edgeCosts.size();
}

void GraphEdgeCostsList::sortInc() {
	this->edgeCosts.sort(compare_EdgeCost_inc());
}

void GraphEdgeCostsList::sortDec() {
	this->edgeCosts.sort(compare_EdgeCost_dec());
}

void GraphEdgeCostsList::begin() {
	begin(this->edgeCostsIterator);
}

void GraphEdgeCostsList::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void GraphEdgeCostsList::end() {
	end(this->edgeCostsIterator);
}

void GraphEdgeCostsList::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool GraphEdgeCostsList::hasNext() {
	return hasNext(this->edgeCostsIterator);
}

bool GraphEdgeCostsList::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool GraphEdgeCostsList::hasPrevious() {
	return hasPrevious(this->edgeCostsIterator);
}

bool GraphEdgeCostsList::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsList::next() {
	return next(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsList::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsList::current() {
	return current(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsList::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsList::previous() {
	return previous(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsList::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsList::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->edgeCostsIterator, moveIndex);
}

EdgeCost GraphEdgeCostsList::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId GraphEdgeCostsList::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::list<EdgeCost>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::list<EdgeCost>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::list<EdgeCost>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId,
					std::list<EdgeCost>::const_iterator { }));
	return returnedId;
}

void GraphEdgeCostsList::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//
