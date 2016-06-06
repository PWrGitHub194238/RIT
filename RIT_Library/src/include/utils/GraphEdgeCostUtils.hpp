/*
 * GraphEdgeCostUtils.hpp
 *
 *  Created on: 8 maj 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_GRAPHEDGECOSTUTILS_HPP_
#define SRC_INCLUDE_UTILS_GRAPHEDGECOSTUTILS_HPP_

#include "../typedefs/primitive.hpp"

class EdgeSetIF;
class GraphEdgeCostsIF;
class GraphIF;

namespace GraphEdgeCostUtils {

/** Zwraca koszty krawędzi grafu w postaci scenariusza.
 *
 * @param graph
 * @return
 */
GraphEdgeCostsIF * getScenario(GraphIF* graph);

/** Zwraca scenariusz, którego koszty wszystkich krawędzi poza wskazanym zbiorem są podnienione o addedValue.
 *
 * @param graph
 * @param spanningTree
 * @param addedValue
 * @return
 */
GraphEdgeCostsIF * getInverseCaseScenario(GraphIF* graph,
		EdgeSetIF * spanningTree, EdgeCost addedValue);

}  // namespace GraphEdgeCostUtils

#endif /* SRC_INCLUDE_UTILS_GRAPHEDGECOSTUTILS_HPP_ */
