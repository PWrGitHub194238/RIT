/*
 * GraphEdgeCostsArray.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../../include/structures/GraphEdgeCosts/GraphEdgeCostsArray.hpp"

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
		log4cxx::Logger::getLogger("structures.GraphEdgeCostsArray"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void GraphEdgeCostsArray::begin(
		std::vector<EdgeCost>::const_iterator & iterator) {
	this->edgeCostsIteratorBegin = this->edgeCosts.begin();
	iterator = this->edgeCosts.begin();
	this->edgeCostsIteratorEnd = this->edgeCosts.end();
}

void GraphEdgeCostsArray::end(
		std::vector<EdgeCost>::const_iterator & iterator) {
	this->edgeCostsIteratorBegin = this->edgeCosts.begin();
	iterator = this->edgeCosts.end();
	this->edgeCostsIteratorEnd = this->edgeCosts.end();
}

bool GraphEdgeCostsArray::hasNext(
		std::vector<EdgeCost>::const_iterator & iterator) {
	return iterator != this->edgeCostsIteratorEnd;
}

bool GraphEdgeCostsArray::hasPrevious(
		std::vector<EdgeCost>::const_iterator & iterator) {
	return iterator != this->edgeCostsIteratorBegin;
}

EdgeCost GraphEdgeCostsArray::next(
		std::vector<EdgeCost>::const_iterator & iterator) {
	return *(iterator++);
}

EdgeCost GraphEdgeCostsArray::current(
		std::vector<EdgeCost>::const_iterator & iterator) {
	return *(iterator);
}

EdgeCost GraphEdgeCostsArray::previous(
		std::vector<EdgeCost>::const_iterator & iterator) {
	return *(--iterator);
}

EdgeCost GraphEdgeCostsArray::peek(
		std::vector<EdgeCost>::const_iterator & iterator, int moveIndex)
				throw (LogicExceptions::EmptyIteratorException) {
	EdgeCost item { };
	std::vector<EdgeCost>::const_iterator tmpIterator = iterator;
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

void GraphEdgeCostsArray::createIteratorIfNotExists(
		IteratorId const iteratorId) {
	if (iteratorMap.count(iteratorId) == 0) {
		iteratorMap.insert(
				std::make_pair(iteratorId,
						std::vector<EdgeCost>::const_iterator { }));
	}
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

GraphEdgeCostsArray::GraphEdgeCostsArray() :
		GraphEdgeCostsIF() {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(std::string scenarioName) :
		GraphEdgeCostsIF(scenarioName) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(GraphEdgeCostsIF * graphEdgeCosts) :
		GraphEdgeCostsArray(graphEdgeCosts,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(GraphEdgeCostsIF * graphEdgeCosts,
		std::string scenarioName) {
	this->name = scenarioName;
	this->edgeCosts = std::vector<EdgeCost> { };
	graphEdgeCosts->begin();
	while (graphEdgeCosts->hasNext()) {
		this->edgeCosts.push_back(graphEdgeCosts->next());
	}
}

GraphEdgeCostsArray::GraphEdgeCostsArray(GraphIF* const graph) :
		GraphEdgeCostsArray(graph,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(GraphIF* const graph,
		std::string scenarioName) :
		GraphEdgeCostsIF(graph, scenarioName) {
	this->edgeCosts = std::vector<EdgeCost> { };
	graph->beginEdge();
	while (graph->hasNextEdge()) {
		this->edgeCosts.push_back(graph->nextEdge()->getEdgeCost());
	}
}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeSetIF* const edgeSet) :
		GraphEdgeCostsArray(edgeSet,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeSetIF* const edgeSet,
		std::string scenarioName) :
		GraphEdgeCostsIF(edgeSet, scenarioName) {
	this->edgeCosts = std::vector<EdgeCost> { };
	edgeSet->begin();
	while (edgeSet->hasNext()) {
		this->edgeCosts.push_back(edgeSet->next()->getEdgeCost());
	}
}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeCount numberOfEdges) :
		GraphEdgeCostsArray(numberOfEdges,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeCount numberOfEdges,
		std::string scenarioName) :
		GraphEdgeCostsIF(numberOfEdges, scenarioName) {
	this->edgeCosts = std::vector<EdgeCost>(numberOfEdges, 0);
}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeCount numberOfEdges,
		bool fillWithZeros) :
		GraphEdgeCostsArray(numberOfEdges,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME),
				fillWithZeros) {

}

GraphEdgeCostsArray::GraphEdgeCostsArray(EdgeCount numberOfEdges,
		std::string scenarioName, bool fillWithZeros) :
		GraphEdgeCostsIF(numberOfEdges, scenarioName) {
	if (fillWithZeros) {
		this->edgeCosts = std::vector<EdgeCost>(numberOfEdges, 0);
	} else {
		this->edgeCosts.reserve(numberOfEdges);
	}
}

GraphEdgeCostsArray::~GraphEdgeCostsArray() {
	this->edgeCosts.clear();
	this->iteratorMap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeCost& GraphEdgeCostsArray::operator[](EdgeIdx const edgeIdx) {
	return this->edgeCosts[edgeIdx];
}

void GraphEdgeCostsArray::push_back(EdgeCost edgecost) {
	this->edgeCosts.push_back(edgecost);
}

EdgeCost GraphEdgeCostsArray::at(EdgeIdx edgeIdx) {
	return *(std::next(this->edgeCosts.begin(), edgeIdx));
}

VertexCount GraphEdgeCostsArray::size() const {
	return (EdgeCount) this->edgeCosts.size();
}

void GraphEdgeCostsArray::sortInc() {
	std::sort(this->edgeCosts.begin(), this->edgeCosts.end(),
			compare_EdgeCost_inc());
}

void GraphEdgeCostsArray::sortDec() {
	std::sort(this->edgeCosts.begin(), this->edgeCosts.end(),
			compare_EdgeCost_dec());
}
void GraphEdgeCostsArray::begin() {
	begin(this->edgeCostsIterator);
}

void GraphEdgeCostsArray::begin(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	begin(iteratorMap.at(iteratorId));
}

void GraphEdgeCostsArray::end() {
	end(this->edgeCostsIterator);
}

void GraphEdgeCostsArray::end(IteratorId const iteratorId) {
	createIteratorIfNotExists(iteratorId);
	end(iteratorMap.at(iteratorId));
}

bool GraphEdgeCostsArray::hasNext() {
	return hasNext(this->edgeCostsIterator);
}

bool GraphEdgeCostsArray::hasNext(IteratorId const iteratorId) {
	return hasNext(iteratorMap.at(iteratorId));
}

bool GraphEdgeCostsArray::hasPrevious() {
	return hasPrevious(this->edgeCostsIterator);
}

bool GraphEdgeCostsArray::hasPrevious(IteratorId const iteratorId) {
	return hasPrevious(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsArray::next() {
	return next(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsArray::next(IteratorId const iteratorId) {
	return next(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsArray::current() {
	return current(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsArray::current(IteratorId const iteratorId) {
	return current(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsArray::previous() {
	return previous(this->edgeCostsIterator);
}

EdgeCost GraphEdgeCostsArray::previous(IteratorId const iteratorId) {
	return previous(iteratorMap.at(iteratorId));
}

EdgeCost GraphEdgeCostsArray::peek(int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(this->edgeCostsIterator, moveIndex);
}

EdgeCost GraphEdgeCostsArray::peek(IteratorId const iteratorId, int moveIndex)
		throw (LogicExceptions::EmptyIteratorException) {
	return peek(iteratorMap.at(iteratorId), moveIndex);
}

IteratorId GraphEdgeCostsArray::getIterator() {
	std::set<IteratorId> iteratorIdSet { };
	IteratorId returnedId { 0 };
	std::transform(this->iteratorMap.begin(), this->iteratorMap.end(),
			std::inserter(iteratorIdSet, iteratorIdSet.begin()),
			[](std::pair<IteratorId, std::vector<EdgeCost>::const_iterator> pair) {return pair.first;});

	if (iteratorMap.empty()) {
		iteratorMap.insert(
				std::make_pair(0, std::vector<EdgeCost>::const_iterator { }));
		return 0;
	}

	for (IteratorId id : iteratorIdSet) {
		if (returnedId != id) {
			iteratorMap.insert(
					std::make_pair(returnedId,
							std::vector<EdgeCost>::const_iterator { }));
			return returnedId;
		}
		returnedId += 1;
	}
	iteratorMap.insert(
			std::make_pair(returnedId,
					std::vector<EdgeCost>::const_iterator { }));
	return returnedId;
}

void GraphEdgeCostsArray::removeIterator(IteratorId const iteratorId) {
	this->iteratorMap.erase(iteratorId);
}

//*************************************** GETTERS & SETTERS ****************************************//
