/*
 * CPLEX_LP_IMSTSolverIF.cpp
 *
 *  Created on: 22 kwi 2016
 *      Author: tomasz
 */

#include "../../include/cplex/CPLEX_LP_IMSTSolverIF.hpp"

#include <ilconcert/iloalg.h>
#include <ilconcert/iloextractable.h>
#include <ilconcert/ilosys.h>
#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>
#include <sstream>

#include "../../include/cplex/CPLEX_LP_MSTSolverInclude.hpp"
#include "../../include/cplex/CPLEXUtils.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/utils/GraphUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"

class GraphIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("cplex.CPLEX_LP_IMSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

void CPLEX_LP_IMSTSolverIF::replaceBaseMSTSolution(EdgeSetIF* baseMSTSolution) {
	if (baseMSTSolution != nullptr) {
		INFO(logger, LogBundleKey::CPLPIMSTIF_IF_FORCE_NEW_BASE_MST,
				LogStringUtils::edgeSetVisualization(this->baseMSTSolution,
						"\t").c_str(),
				this->baseMSTSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(baseMSTSolution, "\t").c_str(),
				baseMSTSolution->getTotalEdgeCost());
		MemoryUtils::removeCollection(this->baseMSTSolution, false);
		this->baseMSTSolution = baseMSTSolution;
		return;
	}
	INFO(logger, LogBundleKey::CPLPIMSTIF_IF_BASE_MST_NOT_CHANGED,
			LogStringUtils::edgeSetVisualization(this->baseMSTSolution, "\t").c_str(),
			this->baseMSTSolution->getTotalEdgeCost());
}

EdgeSetIF* CPLEX_LP_IMSTSolverIF::transformSolutionToEdgeSet() {
	EdgeSetIF* solution = new EdgeSetImpl { graph->getNumberOfVertices(
			Visibility::VISIBLE) - 1 };
	INFO_NOARG(logger, LogBundleKey::CPLPIMSTIF_BUILD_EDGE_SET);
	for (auto& sourceVertex : edgeVariableMap) {
		for (auto& targetVertex : sourceVertex.second) {
			if (cplex.getValue(targetVertex.second.first) == 1) {
				INFO(logger, LogBundleKey::CPLPIMSTIF_BUILD_EDGE_SET_ADD,
						LogStringUtils::edgeVisualization(
								graph->findEdge(sourceVertex.first,
										targetVertex.first), "\t").c_str());;
				solution->push_back(
						getEdge(sourceVertex.first, targetVertex.first));
			}
		}
	}
	return solution;
}

EdgeIF* CPLEX_LP_IMSTSolverIF::getEdge(const VertexIdx sourceVertexIdx,
		const VertexIdx targetVertexIdx) {
	return edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).second;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getEdgeVariable(
		const VertexIdx sourceVertexIdx, const VertexIdx targetVertexIdx) {
	return edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getEdgeVariable(VertexIF* const sourceVertex,
		VertexIF* const targetVertex) {
	return edgeVariableMap.at(sourceVertex->getVertexIdx()).at(
			targetVertex->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getEdgeVariable(const EdgeIF* edge) {
	return edgeVariableMap.at(edge->getSourceVertex()->getVertexIdx()).at(
			edge->getTargetVertex()->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getAddEdgeVariable(
		const VertexIdx sourceVertexIdx, const VertexIdx targetVertexIdx) {
	return edgeAddVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getAddEdgeVariable(
		VertexIF* const sourceVertex, VertexIF* const targetVertex) {
	return edgeAddVariableMap.at(sourceVertex->getVertexIdx()).at(
			targetVertex->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getAddEdgeVariable(const EdgeIF* edge) {
	return edgeAddVariableMap.at(edge->getSourceVertex()->getVertexIdx()).at(
			edge->getTargetVertex()->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getDropEdgeVariable(
		const VertexIdx sourceVertexIdx, const VertexIdx targetVertexIdx) {
	return edgeDropVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getDropEdgeVariable(
		VertexIF* const sourceVertex, VertexIF* const targetVertex) {
	return edgeDropVariableMap.at(sourceVertex->getVertexIdx()).at(
			targetVertex->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_IMSTSolverIF::getDropEdgeVariable(const EdgeIF* edge) {
	return edgeDropVariableMap.at(edge->getSourceVertex()->getVertexIdx()).at(
			edge->getTargetVertex()->getVertexIdx()).first;
}

std::string CPLEX_LP_IMSTSolverIF::getVariableName(IloNumVar const & variable) {
	std::ostringstream oss { };
	oss << variable << std::flush;
	return oss.str();
}

std::string CPLEX_LP_IMSTSolverIF::getCPLEXStatus(IloCplex& cplex) {
	std::ostringstream oss { };
	oss << cplex.getStatus() << std::flush;
	return oss.str();
}

EdgeSetIF * CPLEX_LP_IMSTSolverIF::resolve(IncrementalParam k)
		throw (ModelExceptions::GeneralConcertException) {
	EdgeSetIF* solution { };
	if (this->isCostChanged && k > 0) {
		this->isCostChanged = false;
		buildModel(k);

		try {
			solution = LP_Solve();
		} catch (IloException& e) {
			ERROR(logger, LogBundleKey::CPLPIMSTIF_SOLVER_ERROR, e.getMessage());
			throw ModelExceptions::GeneralConcertException();
		}
		INFO(logger, LogBundleKey::CPLPIMSTIF_SOLUTION,
				LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
				solution->getTotalEdgeCost());
		return solution;
	}

	solution = new EdgeSetImpl { this->baseMSTSolution, false };
	INFO(logger, LogBundleKey::CPLPIMSTIF_SOLUTION,
			LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
			solution->getTotalEdgeCost());
	return solution;
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

CPLEX_LP_IMSTSolverIF::CPLEX_LP_IMSTSolverIF(
		CPLEX_LP_MSTSolverIF* const mstSolver, GraphIF * const graph,
		EdgeSetIF * baseSolution, IloNumVar::Type edgeVariablesType) :
		graph { graph }, numberOfVertices { graph->getNumberOfVertices(
				Visibility::VISIBLE) }, numberOfEdges { graph->getNumberOfEdges(
				Visibility::VISIBLE) } {
	INFO(logger, LogBundleKey::CPLPIMSTIF_INIT,
			LogStringUtils::graphDescription(graph, "\t").c_str());

	this->model = IloModel(env);
	this->cplex = IloCplex(model);
	this->defaultSolverUsed = (mstSolver == nullptr);
	this->mstSolver = (mstSolver ? mstSolver : new CPLEX_LP_MSTSolverImpl {
											graph });
	this->graph = graph;
	this->baseGraphEdgeCosts = nullptr;
	this->baseMSTSolution =
			baseSolution ?
					new EdgeSetImpl { baseSolution, false } :
					this->mstSolver->getMST();
	this->baseLPMSTSolution = CPLEXUtils::transformEdgeSetToLP(graph,
			baseMSTSolution);
	this->isCostChanged = false;
	this->solution = nullptr;
}

CPLEX_LP_IMSTSolverIF::~CPLEX_LP_IMSTSolverIF() {
	if (defaultSolverUsed) {
		delete mstSolver;
	}
	MemoryUtils::removeCollection(baseMSTSolution, false);
	env.end();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void CPLEX_LP_IMSTSolverIF::changeEdgeCost(VertexIF* const sourceVertex,
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

void CPLEX_LP_IMSTSolverIF::changeEdgeCost(VertexIF* const sourceVertex,
		VertexIF* const targetvertex, EdgeCost newCost)
				throw (LogicExceptions::EdgeNullPointerException) {
	changeEdgeCost(sourceVertex, targetvertex, EdgeConnectionType::UNDIRECTED,
			newCost);
}

void CPLEX_LP_IMSTSolverIF::changeEdgeCost(VertexIdx const sourceVertexId,
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

void CPLEX_LP_IMSTSolverIF::changeEdgeCost(VertexIdx const sourceVertexId,
		VertexIdx const targetvertexId, EdgeCost newCost)
				throw (LogicExceptions::EdgeNullPointerException) {
	changeEdgeCost(sourceVertexId, targetvertexId,
			EdgeConnectionType::UNDIRECTED, newCost);
}

void CPLEX_LP_IMSTSolverIF::changeEdgeCost(EdgeIF* edge, EdgeCost newCost)
		throw (LogicExceptions::EdgeNullPointerException) {
	if (edge != nullptr) {
		if (edge->getEdgeCost() != newCost) {
			INFO(logger, LogBundleKey::CPLPIMSTIF_IF_CHANGE_EDGE_COST,
					LogStringUtils::edgeCostChanged(edge, newCost, "\t").c_str());
			edge->setEdgeCost(newCost);
			this->isCostChanged = true;
			return;
		}
		INFO(logger, LogBundleKey::CPLPIMSTIF_IF_CHANGE_EDGE_NOT_REQUIRED,
				LogStringUtils::edgeVisualization(edge, "\t").c_str());
		return;
	}
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeSetIF* CPLEX_LP_IMSTSolverIF::getMST(EdgeCount k,
		EdgeSetIF* baseMSTSolution, GraphEdgeCostsIF* newGraphCosts) {
	replaceBaseMSTSolution(baseMSTSolution);
	if (baseMSTSolution != nullptr) {
		this->baseLPMSTSolution = CPLEXUtils::transformEdgeSetToLP(graph,
				baseMSTSolution);
	}
	if (newGraphCosts != nullptr) {
		GraphUtils::changeGraphCosts(graph, newGraphCosts);
		this->isCostChanged = true;
	}
	return resolve(k);
}

EdgeSetIF* CPLEX_LP_IMSTSolverIF::getMST(EdgeCount k,
		GraphEdgeCostsIF* newGraphCosts) {
	return getMST(k, nullptr, newGraphCosts);
}

EdgeSetIF* CPLEX_LP_IMSTSolverIF::getMST(EdgeCount k,
		EdgeSetIF* baseMSTSolution) {
	return getMST(k, baseMSTSolution, nullptr);
}

EdgeSetIF* CPLEX_LP_IMSTSolverIF::getMST(EdgeCount k) {
	return getMST(k, nullptr, nullptr);
}

//*************************************** GETTERS & SETTERS ****************************************//
