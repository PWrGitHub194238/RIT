/*
 * GraphEdgeCostUtils.cpp
 *
 *  Created on: 8 maj 2016
 *      Author: tomasz
 */

#include "../../include/utils/GraphEdgeCostUtils.hpp"

#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetIF.hpp"
#include "../../include/structures/GraphEdgeCostsIF.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/typedefs/primitive.hpp"

class GraphEdgeCostsIF;

GraphEdgeCostsIF * GraphEdgeCostUtils::getScenario(GraphIF* graph) {
	GraphEdgeCostsIF* scenario = new GraphEdgeCostsImpl {
			graph->getNumberOfEdges(), false };
	IteratorId edgeIterator = graph->getEdgeIteratorId();

	graph->beginEdge(edgeIterator);
	while (graph->hasNextEdge(edgeIterator)) {
		scenario->push_back(graph->nextEdge(edgeIterator)->getEdgeCost());
	}
	graph->removeEdgeIterator(edgeIterator);
	return scenario;
}

GraphEdgeCostsIF * GraphEdgeCostUtils::getInverseCaseScenario(GraphIF* graph,
		EdgeSetIF * spanningTree, EdgeCost addedValue) {
	EdgeIF* edge { };
	GraphEdgeCostsIF* scenario = new GraphEdgeCostsImpl {
			graph->getNumberOfEdges(), false };
	IteratorId edgeIterator = spanningTree->getIterator();

	spanningTree->begin(edgeIterator);
	while (spanningTree->hasNext(edgeIterator)) {
		edge = spanningTree->next(edgeIterator);
		edge->setEdgeCost(edge->getEdgeCost() + addedValue);
	}

	spanningTree->removeIterator(edgeIterator);
	edgeIterator = graph->getEdgeIteratorId();

	graph->beginEdge(edgeIterator);
	while (graph->hasNextEdge(edgeIterator)) {
		edge = graph->nextEdge(edgeIterator);
		scenario->push_back(edge->getEdgeCost());
	}

	graph->removeEdgeIterator(edgeIterator);
	edgeIterator = spanningTree->getIterator();

	spanningTree->begin(edgeIterator);
	while (spanningTree->hasNext(edgeIterator)) {
		edge = spanningTree->next(edgeIterator);
		edge->setEdgeCost(edge->getEdgeCost() - addedValue);
	}
	spanningTree->removeIterator(edgeIterator);
	return scenario;
}
