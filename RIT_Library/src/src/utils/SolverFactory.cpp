/*
 * SolverFactory.cpp
 *
 *  Created on: 17 kwi 2016
 *      Author: tomasz
 */

#include "../../include/utils/SolverFactory.hpp"

#include <log4cxx/logger.h>

#include "../../include/aimstsolver/AIMSTSolverInclude.hpp"
#include "../../include/enums/AIMSTSolverEnum.hpp"
#include "../../include/enums/IMSTSolverEnum.hpp"
#include "../../include/enums/MSTSolverEnum.hpp"
#include "../../include/imstsolver/IMSTSolverInclude.hpp"
#include "../../include/mstsolver/MSTSolverInclude.hpp"
#include "../../include/typedefs/primitive.hpp"
#include "../../include/typedefs/struct.hpp"

class GraphIF;
class MSTSolverIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.SolverFactory"));

MSTSolverIF* SolverFactory::getMSTSolver(GraphIF* graph) {
	return getMSTSolver(MSTSolverEnum::DEFAULT, graph);
}

MSTSolverIF* SolverFactory::getMSTSolver(MSTSolverEnum mstSolverType,
		GraphIF* graph) {
	switch (mstSolverType) {
	case MSTSolverEnum::DEFAULT:
		return new MSTSolverImpl { graph };
	default:
		return new MSTSolverImpl { graph };
	}
}

IMSTSolverIF* SolverFactory::getIMSTSolver(GraphIF* graph) {
	return getIMSTSolver(IMSTSolverEnum::DEFAULT, graph);
}

IMSTSolverIF* SolverFactory::getIMSTSolver(IMSTSolverEnum imstSolverType,
		GraphIF* graph) {
	switch (imstSolverType) {
	case IMSTSolverEnum::DEFAULT:
		return new IMSTSolverImpl { graph };
	default:
		return new IMSTSolverImpl { graph };
	}
}

AIMSTSolverIF* SolverFactory::getAIMSTSolver(GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) {
	return getAIMSTSolver(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
			MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k);
}

AIMSTSolverIF* SolverFactory::getAIMSTSolver(MSTSolverEnum mstSolverType,
		GraphIF* graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) {
	return getAIMSTSolver(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
			mstSolverType, graph, adversarialScenarioSet, k);
}

AIMSTSolverIF* SolverFactory::getAIMSTSolver(IMSTSolverEnum imstSolverType,
		GraphIF* graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) {
	return getAIMSTSolver(AIMSTSolverEnum::DEFAULT, imstSolverType,
			MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k);
}

AIMSTSolverIF* SolverFactory::getAIMSTSolver(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF* graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) {
	return getAIMSTSolver(AIMSTSolverEnum::DEFAULT, imstSolverType,
			mstSolverType, graph, adversarialScenarioSet, k);
}

AIMSTSolverIF* SolverFactory::getAIMSTSolver(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		GraphIF* graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) {
	switch (aimstSolverType) {
	case AIMSTSolverEnum::DEFAULT:
		return new AIMSTSolverImpl { imstSolverType, mstSolverType, graph,
				adversarialScenarioSet, k };
	default:
		return new AIMSTSolverImpl { imstSolverType, mstSolverType, graph,
				adversarialScenarioSet, k };
	}
}
