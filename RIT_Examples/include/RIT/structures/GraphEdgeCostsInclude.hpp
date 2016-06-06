/*
 * GraphEdgeCostsInclude.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSINCLUDE_HPP_
#define SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSINCLUDE_HPP_

#define GraphEdgeCostsIF_GraphEdgeCostsArray

#include "GraphEdgeCostsIF.hpp"

#ifdef GraphEdgeCostsIF_GraphEdgeCostsList
#include "GraphEdgeCosts/GraphEdgeCostsList.hpp"
#elif defined(GraphEdgeCostsIF_GraphEdgeCostsArray)
#include "GraphEdgeCosts/GraphEdgeCostsArray.hpp"
#endif

typedef GraphEdgeCostsArray GraphEdgeCostsImpl;

#endif /* SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSINCLUDE_HPP_ */
