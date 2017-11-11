/*
 * AIMSTSolverIF.cpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#include "../../include/aimstsolver/AIMSTSolverIF.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>
#include <set>

#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/utils/AIMSTUtils.hpp"
#include "../../include/utils/GraphUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("aimstsolver.AIMSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

AIMSTSolution AIMSTSolverIF::resolve(EdgeSetIF* const baseSolution) {
	return AIMSTUtils::MULTITHREAD ?
			resolveWithMultThreads(baseSolution) :
			resolveWithSingleThread(baseSolution);
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

AIMSTSolverIF::AIMSTSolverIF(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) {
	this->imstSolverType = imstSolverType;
	this->mstSolverType = mstSolverType;
	this->graph = graph;
	this->adversarialScenarioSet = adversarialScenarioSet;
	this->k = k;
	if (this->adversarialScenarioSet.size() == 0) {
		WARN_NOARG(logger, LogBundleKey::AIMSTIF_INIT_NO_SCENARIO);
	}
}

AIMSTSolverIF::AIMSTSolverIF(IMSTSolverEnum imstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) :
		AIMSTSolverIF(imstSolverType, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k) {
}

AIMSTSolverIF::AIMSTSolverIF(GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		AIMSTSolverIF(IMSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k) {

}

AIMSTSolverIF::~AIMSTSolverIF() {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

AIMSTSolution AIMSTSolverIF::getSolution(EdgeSetIF* const baseSolution,
		GraphEdgeCostsIF* initialGraphCosts)
				throw (GraphExceptions::DisconnectedGraphException) {
	GraphEdgeCostsIF* graphCosts = new GraphEdgeCostsImpl { graph };
	GraphUtils::changeGraphCosts(graph, initialGraphCosts);
	AIMSTSolution solution = resolve(baseSolution);
	GraphUtils::changeGraphCosts(graph, graphCosts);
	delete graphCosts;
	return solution;
}

AIMSTSolution AIMSTSolverIF::getSolution(EdgeSetIF* const baseSolution)
		throw (GraphExceptions::DisconnectedGraphException) {
	return resolve(baseSolution);
}

//*************************************** GETTERS & SETTERS ****************************************//
