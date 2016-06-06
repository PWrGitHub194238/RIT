/*
 * AIMSTUtils.hpp
 *
 *  Created on: 2 maj 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_AIMSTUTILS_HPP_
#define SRC_INCLUDE_UTILS_AIMSTUTILS_HPP_

#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

class GraphIF;

namespace AIMSTUtils {

extern const bool MULTITHREAD;

/** Zwraca identyczny zbiór co edgeSet, tyle że odnoszący się do grafu graph,
 * mającego inne dowiązania do wirzchołków.
 *
 * @param graph
 * @param edgeSet
 * @return
 */
EdgeSetIF* getEdgeSetCopy(GraphIF* const graph, EdgeSetIF* const edgeSet);

AIMSTSolution createAIMSTSolution(EdgeSetIF* const solution,
		GraphEdgeCostsIF* const scenario, EdgeCost const solutionCost);

EdgeSetIF* getEdgeSet(AIMSTSolution aimstSolution);

GraphEdgeCostsIF* getScenario(AIMSTSolution aimstSolution);

EdgeCost getSolutionCost(AIMSTSolution aimstSolution);

}  // namespace AIMSTUtils

#endif /* SRC_INCLUDE_UTILS_AIMSTUTILS_HPP_ */
