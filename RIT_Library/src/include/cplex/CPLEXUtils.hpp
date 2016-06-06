/*
 * CPLEXUtils.hpp
 *
 *  Created on: 24 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEXUTILS_HPP_
#define SRC_INCLUDE_CPLEX_CPLEXUTILS_HPP_

#include "CPLEX_Typedefs.hpp"

class GraphIF;

class EdgeSetIF;

class CPLEX_LP_MSTSolverIF;

namespace CPLEXUtils {

IloEdgeValMap transformEdgeSetToLP(GraphIF* graph, EdgeSetIF* edgeSet);

}  // namespace CPLEXUtils

#endif /* SRC_INCLUDE_CPLEX_CPLEXUTILS_HPP_ */
