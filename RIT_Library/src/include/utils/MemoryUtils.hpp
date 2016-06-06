/*
 * MemoryUtils.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_MEMORYUTILS_HPP_
#define SRC_INCLUDE_UTILS_MEMORYUTILS_HPP_

#include "../structures/VisibleIterableIF.hpp"
#include "../typedefs/struct.hpp"

class GraphIF;

namespace MemoryUtils {

template<typename Item>
void removeCollection(VisibleIterable<Item>* const & collection,
		bool withItems);

template<typename Item>
void removeCollection(VisibleIterable<Item>* const & collection);

void removeGraph(GraphIF* const & graph, bool withVertices, bool withEdges);

void removeGraph(GraphIF* const & graph);

void removeScenarioSet(GraphEdgeCostsSet scenarioSet);

}  // namespace MemoryUtils

#endif /* SRC_INCLUDE_UTILS_MEMORYUTILS_HPP_ */
