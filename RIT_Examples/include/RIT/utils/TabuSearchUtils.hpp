/*
 * TabuSearchUtils.hpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_TABUSEARCHUTILS_HPP_
#define SRC_INCLUDE_UTILS_TABUSEARCHUTILS_HPP_

#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

class GraphIF;

class EdgeSetIF;

namespace TabuSearchUtils {

extern const TabuIterationCount TABU_ELEMENT_DEFAULT_PERIOD;
extern const TabuIterationCount ITER_NUM_DEFAULT;
extern const TabuIterationCount PATH_ITER_NUM_DEFAULT;

SpanningTreeNeighbor createSpanningTreeNeighbor(EdgeIdx const removedEdgeIdx,
		EdgeIdx const insertedEdgeIdx, EdgeSetIF * const spanningTree);

EdgeIdx getRemovedEdge(SpanningTreeNeighbor const spanningTreeNeighbor);

EdgeIdx getInsertedEdge(SpanningTreeNeighbor const spanningTreeNeighbor);

EdgeSetIF* getEdgeSet(SpanningTreeNeighbor const spanningTreeNeighbor);

SpanningTreeNeighbor getMove(
		NeighborSolution const & spanningTreeNeighborWithCost);

EdgeSetIF* getEdgeSet(NeighborSolution const & spanningTreeNeighborWithCost);

EdgeCost getEdgeSetCost(NeighborSolution const & spanningTreeNeighborWithCost);

}  // namespace TabuSearchUtils

#endif /* SRC_INCLUDE_UTILS_TABUSEARCHUTILS_HPP_ */
