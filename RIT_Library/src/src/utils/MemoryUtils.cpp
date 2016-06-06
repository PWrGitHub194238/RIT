/*
 * MemoryUtils.cpp

 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/MemoryUtils.hpp"

#include <log4cxx/logger.h>

#include "../../include/enums/Visibility.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetIF.hpp"
#include "../../include/structures/GraphEdgeCostsIF.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexSetIF.hpp"
#include "../../include/structures/VisibleIterableIF.hpp"
#include "../../include/typedefs/struct.hpp"

class GraphIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.MemoryUtils"));

template<typename Item>
void MemoryUtils::removeCollection(VisibleIterable<Item>* const & collection,
		bool withItems) {
	if (withItems) {
		collection->begin();
		while (collection->hasNext(Visibility::BOTH)) {
			delete collection->next();
		}
	}
	delete collection;
}

template<typename Item>
void MemoryUtils::removeCollection(VisibleIterable<Item>* const & collection) {
	removeCollection(collection, true);
}

void MemoryUtils::removeGraph(GraphIF* const & graph, bool withVertices,
		bool withEdges) {
	MemoryUtils::removeCollection(graph->vertexSet, withEdges);
	MemoryUtils::removeCollection(graph->edgeSet, withEdges);
	delete graph;
}

void MemoryUtils::removeGraph(GraphIF* const & graph) {
	removeGraph(graph, true, true);
}

void MemoryUtils::removeScenarioSet(GraphEdgeCostsSet scenarioSet) {
	for (GraphEdgeCostsIF* scenario : scenarioSet) {
		delete scenario;
	}
}

template void MemoryUtils::removeCollection<EdgeIF*>(
		VisibleIterable<EdgeIF*>* const & collection, bool withItems);

template void MemoryUtils::removeCollection<EdgeIF*>(
		VisibleIterable<EdgeIF*>* const & collection);
