/*
 * SolverFactory.hpp
 *
 *  Created on: 17 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_SOLVERFACTORY_HPP_
#define SRC_INCLUDE_UTILS_SOLVERFACTORY_HPP_

#include "../enums/AIMSTSolverEnum.hpp"
#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

class GraphIF;
class MSTSolverIF;
class IMSTSolverIF;
class AIMSTSolverIF;

namespace SolverFactory {

MSTSolverIF* getMSTSolver(GraphIF* graph);

MSTSolverIF* getMSTSolver(MSTSolverEnum mstSolverType, GraphIF* graph);

IMSTSolverIF* getIMSTSolver(GraphIF* graph);

IMSTSolverIF* getIMSTSolver(IMSTSolverEnum imstSolverType, GraphIF* graph);

AIMSTSolverIF* getAIMSTSolver(GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

AIMSTSolverIF* getAIMSTSolver(MSTSolverEnum mstSolverType, GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

AIMSTSolverIF* getAIMSTSolver(IMSTSolverEnum imstSolverType, GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

AIMSTSolverIF* getAIMSTSolver(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

AIMSTSolverIF* getAIMSTSolver(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		GraphIF* graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k);

}  // namespace SolverFactory

#endif /* SRC_INCLUDE_UTILS_SOLVERFACTORY_HPP_ */
