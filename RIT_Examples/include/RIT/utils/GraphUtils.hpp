/*
 * GraphUtils.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_GRAPHUTILS_HPP_
#define SRC_INCLUDE_UTILS_GRAPHUTILS_HPP_

#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

#include "../exp/GraphExceptions.hpp"

class EdgeSetIF;

class GraphIF;

namespace GraphUtils {

extern const EdgeCost MIN_EDGE_COST;
extern const EdgeCost MAX_EDGE_COST;

/** Perform DFS on graph with all but nodHiddenEdges hidden (they do'n exists from DFS's point of view)
 *
 * @param graph
 * @param nodHiddenEdges
 * @return
 */
bool isGraphConnected(GraphIF* const graph, EdgeSetIF* const notHiddenEdges);

bool isGraphConnected(GraphIF* const graph);

void changeGraphCosts(GraphIF* graph, GraphEdgeCostsIF* const newGraphCosts);

bool changeGraphCostsWithCheck(GraphIF* graph,
		GraphEdgeCostsIF* const newGraphCosts);

ConnectivityList shrinkConnectivityToSet(GraphIF* const graph,
		EdgeSetIF* const visibleSet);

VisibilityList shrinkVisibilityToSet(GraphIF* const graph,
		EdgeSetIF* const visibleSet);

EdgeSetIF* getRandomSpanningTree(GraphIF* const graph,
		EdgeSetIF* const visibleSet)
				throw (GraphExceptions::DisconnectedGraphException);

EdgeSetIF* getRandomSpanningTree(GraphIF* const graph)
		throw (GraphExceptions::DisconnectedGraphException);

GraphIF* getRandomGraph(VertexCount numberOfVertices, Density density,
		EdgeCost lowerEdgeCost, EdgeCost upperEdgeCost);

GraphIF* getRandomGraph(VertexCount numberOfVertices, EdgeCount numberOfEdges,
		EdgeCost lowerEdgeCost, EdgeCost upperEdgeCost);

EdgeSetIF* getMSTEdgesBeetwenNodes(GraphIF* graph, EdgeSetIF* mstEdgeSet,
		VertexIdx const sourceVertexIdx, VertexIdx const targetVertexIdx);

EdgeSetIF* getMSTEdgesBeetwenNodes(GraphIF* graph, EdgeSetIF* mstEdgeSet,
		VertexIF* sourceVertex, VertexIF* targetVertex);

SpanningTreeNeighborhood getEdgeSetNeighbourhood(GraphIF * graph,
		EdgeSetIF* originalSet);

}  // namespace GraphUtils

#endif /* SRC_INCLUDE_UTILS_GRAPHUTILS_HPP_ */
