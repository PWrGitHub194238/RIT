/*
 * RRIMSTSolverIF.cpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#include "../../include/rrimstsolver/RRIMSTSolverIF.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>
#include <set>
#include <string>

#include "../../include/aimstsolver/AIMSTSolverIF.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/mstsolver/MSTSolverIF.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/utils/SolverFactory.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("RRIMSTsolver.RRIMSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

void RRIMSTSolverIF::backupGraphCosts(GraphEdgeCostsIF* newGraphCosts) {
	IteratorId graphIterator { };
	if (newGraphCosts == nullptr) {
		this->graphCostBackup = new GraphEdgeCostsImpl { graph };
		return;
	} else if (newGraphCosts->size() == graph->getNumberOfEdges()) {
		INFO(logger, LogBundleKey::RRIMSTS_IF_GRAPH_COST_BACKUP,
				LogStringUtils::edgeSetCostChanged(graph, newGraphCosts, "\t").c_str());
		this->graphCostBackup = new GraphEdgeCostsImpl { newGraphCosts };
		graphIterator = graph->getEdgeIteratorId();
		graph->beginEdge(graphIterator);
		newGraphCosts->begin();
		while (graph->hasNextEdge(graphIterator)) {
			graph->nextEdge(graphIterator)->setEdgeCost(newGraphCosts->next());
		}
		graph->removeEdgeIterator(graphIterator);
		return;
	}
	WARN(logger, LogBundleKey::RRIMSTS_IF_NEW_GRAPH_COST_SIZE_MISMATCH,
			newGraphCosts->size(), graph->getNumberOfEdges());
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

RRIMSTSolverIF::RRIMSTSolverIF(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) {
	this->aimstSolverType = aimstSolverType;
	this->imstSolverType = imstSolverType;
	this->mstSolverType = mstSolverType;
	this->aimstSolver = SolverFactory::getAIMSTSolver(aimstSolverType,
			imstSolverType, mstSolverType, graph, adversarialScenarioSet, k);
	this->mstSolver = SolverFactory::getMSTSolver(graph);
	this->graph = graph;
	this->graphCostBackup = nullptr;
	this->adversarialScenarioSet = adversarialScenarioSet;
	this->k = k;
}

RRIMSTSolverIF::RRIMSTSolverIF(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		RRIMSTSolverIF(AIMSTSolverEnum::DEFAULT, imstSolverType, mstSolverType,
				graph, adversarialScenarioSet, k) {

}

RRIMSTSolverIF::RRIMSTSolverIF(MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		RRIMSTSolverIF(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				mstSolverType, graph, adversarialScenarioSet, k) {

}

RRIMSTSolverIF::RRIMSTSolverIF(GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		RRIMSTSolverIF(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k) {

}

RRIMSTSolverIF::~RRIMSTSolverIF() {
	delete this->aimstSolver;
	delete this->mstSolver;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeSetIF* RRIMSTSolverIF::getSolution(GraphEdgeCostsIF* initialGraphCosts)
		throw (GraphExceptions::DisconnectedGraphException) {
	backupGraphCosts(initialGraphCosts);
	return resolve();
}

EdgeSetIF* RRIMSTSolverIF::getSolution()
		throw (GraphExceptions::DisconnectedGraphException) {
	return resolve();
}

//*************************************** GETTERS & SETTERS ****************************************//
