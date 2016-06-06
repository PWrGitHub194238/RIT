/*
 * IMSTSolverIF.cpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#include "../../include/imstsolver/IMSTSolverIF.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>
#include <string>

#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/mstsolver/MSTSolverIF.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/struct.hpp"
#include "../../include/utils/GraphUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"
#include "../../include/utils/SolverFactory.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("imstsolver.IMSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

void IMSTSolverIF::replaceBaseMSTSolution(EdgeSetIF* baseMSTSolution) {
	if (baseMSTSolution != nullptr) {
		INFO(logger, LogBundleKey::IMSTS_IF_FORCE_NEW_BASE_MST,
				LogStringUtils::edgeSetVisualization(this->baseMSTSolution,
						"\t").c_str(),
				this->baseMSTSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(baseMSTSolution, "\t").c_str(),
				baseMSTSolution->getTotalEdgeCost());
		MemoryUtils::removeCollection(this->baseMSTSolution, false);
		this->baseMSTSolution = new EdgeSetImpl { baseMSTSolution, false };
		return;
	}
	INFO(logger, LogBundleKey::IMSTS_IF_BASE_MST_NOT_CHANGED,
			LogStringUtils::edgeSetVisualization(this->baseMSTSolution, "\t").c_str(),
			this->baseMSTSolution->getTotalEdgeCost());
}

void IMSTSolverIF::replaceEdgeCosts(GraphEdgeCostsIF* newGraphCosts) {
	if (newGraphCosts == nullptr) {
		this->baseGraphEdgeCosts = new GraphEdgeCostsImpl { graph };
		return;
	} else if (newGraphCosts->size() == graph->getNumberOfEdges()) {
		INFO(logger, LogBundleKey::IMSTS_IF_GRAPH_COST_CHANGED,
				LogStringUtils::edgeSetCostChanged(graph, newGraphCosts, "\t").c_str());
		this->baseGraphEdgeCosts = new GraphEdgeCostsImpl { newGraphCosts };
		this->isCostChanged = GraphUtils::changeGraphCostsWithCheck(graph,
				newGraphCosts);
		return;
	}
	WARN(logger, LogBundleKey::IMSTS_IF_NEW_GRAPH_COST_SIZE_MISMATCH,
			newGraphCosts->size(), graph->getNumberOfEdges());
}

void IMSTSolverIF::restoreBaseEdgeCosts() {
	if (baseGraphEdgeCosts->size() == graph->getNumberOfEdges()) {
		INFO(logger, LogBundleKey::IMSTS_IF_RESTORE_GRAPH_COSTS,
				LogStringUtils::edgeSetCostChanged(graph, baseGraphEdgeCosts,
						"\t").c_str());
		graph->beginEdge();
		baseGraphEdgeCosts->begin();
		while (graph->hasNextEdge()) {
			graph->nextEdge()->setEdgeCost(baseGraphEdgeCosts->next());
		}
		return;
	}
	ERROR(logger, LogBundleKey::IMSTS_IF_RESTORE_GRAPH_COST_SIZE_MISMATCH,
			baseGraphEdgeCosts->size(), graph->getNumberOfEdges());
}

EdgeCount IMSTSolverIF::getMSTDiff(EdgeSetIF* const currentMSTSolution) {
	EdgeCount edgesNotInBase { };
	VisibilityList visibilityList = baseMSTSolution->storeVisibility();
	baseMSTSolution->hideAll();
	edgesNotInBase = currentMSTSolution->size(Visibility::VISIBLE);
	INFO(logger, IMSTS_IF_EDGES_NOT_IN_BASE_SOLUTION,
			LogStringUtils::mstEdgeDifference(baseMSTSolution,
					currentMSTSolution, "\t").c_str(), edgesNotInBase);
	baseMSTSolution->restoreVisibilityAll(visibilityList);
	return edgesNotInBase;
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

IMSTSolverIF::IMSTSolverIF(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, EdgeSetIF * baseSolution, VertexIF* initialVertex,
		LambdaValue lowerBound, LambdaValue upperBound) {
	this->mstSolverType = mstSolverType;
	this->mstSolver = SolverFactory::getMSTSolver(mstSolverType, graph);
	this->graph = graph;
	this->baseGraphEdgeCosts = nullptr;
	this->baseMSTSolution = (baseSolution ? new EdgeSetImpl { baseSolution,
													false } :
											this->mstSolver->getMST());
	this->lowerBound = lowerBound;
	this->upperBound = upperBound;
	this->isCostChanged = false;
}

IMSTSolverIF::~IMSTSolverIF() {
	delete this->mstSolver;
	delete this->baseGraphEdgeCosts;
	MemoryUtils::removeCollection(this->baseMSTSolution, false);
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void IMSTSolverIF::changeEdgeCost(VertexIF* const sourceVertex,
		VertexIF* const targetvertex, EdgeConnectionType const connectionType,
		EdgeCost newCost) throw (LogicExceptions::EdgeNullPointerException) {
	EdgeIF* edge = nullptr;
	switch (connectionType) {
	case EdgeConnectionType::BACKWARD:
		edge = targetvertex->findOutputEdge(sourceVertex);
		break;
	case EdgeConnectionType::FORWARD:
		edge = sourceVertex->findOutputEdge(targetvertex);
		break;
	case EdgeConnectionType::UNDIRECTED:
		edge = sourceVertex->findOutputEdge(targetvertex);
		if (edge == nullptr) {
			edge = targetvertex->findOutputEdge(sourceVertex);
		}
		break;
	default:
		break;
	}
	changeEdgeCost(edge, newCost);
}

void IMSTSolverIF::changeEdgeCost(VertexIF* const sourceVertex,
		VertexIF* const targetvertex, EdgeCost newCost)
				throw (LogicExceptions::EdgeNullPointerException) {
	changeEdgeCost(sourceVertex, targetvertex, EdgeConnectionType::UNDIRECTED,
			newCost);
}

void IMSTSolverIF::changeEdgeCost(VertexIdx const sourceVertexId,
		VertexIdx const targetvertexId, EdgeConnectionType const connectionType,
		EdgeCost newCost) throw (LogicExceptions::EdgeNullPointerException) {
	EdgeIF* edge = nullptr;
	switch (connectionType) {
	case EdgeConnectionType::BACKWARD:
		edge = graph->getVertexByIdx(targetvertexId)->findOutputEdge(
				sourceVertexId);
		break;
	case EdgeConnectionType::FORWARD:
		edge = graph->getVertexByIdx(sourceVertexId)->findOutputEdge(
				targetvertexId);
		break;
	case EdgeConnectionType::UNDIRECTED:
		edge = graph->getVertexByIdx(sourceVertexId)->findOutputEdge(
				targetvertexId);
		if (edge == nullptr) {
			edge = graph->getVertexByIdx(targetvertexId)->findOutputEdge(
					sourceVertexId);
		}
		break;
	default:
		break;
	}
	changeEdgeCost(edge, newCost);
}

void IMSTSolverIF::changeEdgeCost(VertexIdx const sourceVertexId,
		VertexIdx const targetvertexId, EdgeCost newCost)
				throw (LogicExceptions::EdgeNullPointerException) {
	changeEdgeCost(sourceVertexId, targetvertexId,
			EdgeConnectionType::UNDIRECTED, newCost);
}

void IMSTSolverIF::changeEdgeCost(EdgeIF* edge, EdgeCost newCost)
		throw (LogicExceptions::EdgeNullPointerException) {
	if (edge != nullptr) {
		if (edge->getEdgeCost() != newCost) {
			INFO(logger, LogBundleKey::IMSTS_IF_CHANGE_EDGE_COST,
					LogStringUtils::edgeCostChanged(edge, newCost, "\t").c_str());
			edge->setEdgeCost(newCost);
			this->isCostChanged = true;
			return;
		}
		INFO(logger, LogBundleKey::IMSTS_IF_CHANGE_EDGE_NOT_REQUIRED,
				LogStringUtils::edgeVisualization(edge, "\t").c_str());
		return;
	}
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeSetIF* IMSTSolverIF::getMST() {
	return this->mstSolver->getMST();
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, EdgeSetIF* baseMSTSolution,
		VertexIF* initialVertex, GraphEdgeCostsIF* newGraphCosts) {
	replaceBaseMSTSolution(baseMSTSolution);
	replaceEdgeCosts(newGraphCosts);
	return resolve(k, initialVertex);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, VertexIF* initialVertex,
		GraphEdgeCostsIF* newGraphCosts) {
	return getIMST(k, nullptr, initialVertex, newGraphCosts);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, EdgeSetIF* baseMSTSolution,
		GraphEdgeCostsIF* newGraphCosts) {
	return getIMST(k, baseMSTSolution, nullptr, newGraphCosts);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, GraphEdgeCostsIF* newGraphCosts) {
	return getIMST(k, nullptr, nullptr, newGraphCosts);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, EdgeSetIF* baseMSTSolution,
		VertexIF* initialVertex) {
	return getIMST(k, baseMSTSolution, initialVertex, nullptr);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, VertexIF* initialVertex) {
	return getIMST(k, nullptr, initialVertex, nullptr);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k, EdgeSetIF* baseMSTSolution) {
	return getIMST(k, baseMSTSolution, nullptr, nullptr);
}

EdgeSetIF* IMSTSolverIF::getIMST(EdgeCount k) {
	return getIMST(k, nullptr, nullptr, nullptr);
}

//*************************************** GETTERS & SETTERS ****************************************//
