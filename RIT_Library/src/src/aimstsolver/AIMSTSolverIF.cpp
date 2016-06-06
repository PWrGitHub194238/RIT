/*
 * AIMSTSolverIF.cpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#include "../../include/aimstsolver/AIMSTSolverIF.hpp"

#include <log4cxx/logger.h>
#include <set>

#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/utils/GraphUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("aimstsolver.AIMSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

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

AIMSTSolution AIMSTSolverIF::getMST(EdgeSetIF* const baseSolution,
		GraphEdgeCostsIF* initialGraphCosts)
				throw (GraphExceptions::DisconnectedGraphException) {
	GraphEdgeCostsIF* graphCosts = new GraphEdgeCostsImpl { graph };
	GraphUtils::changeGraphCosts(graph, initialGraphCosts);
	AIMSTSolution solution = resolve(baseSolution);
	GraphUtils::changeGraphCosts(graph, graphCosts);
	delete graphCosts;
	return solution;
}

AIMSTSolution AIMSTSolverIF::getMST(EdgeSetIF* const baseSolution)
		throw (GraphExceptions::DisconnectedGraphException) {
	return resolve(baseSolution);
}

//*************************************** GETTERS & SETTERS ****************************************//
