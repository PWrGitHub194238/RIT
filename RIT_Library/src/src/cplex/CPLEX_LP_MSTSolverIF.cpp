/*
 * CPLEX_LP_MSTSolverIF.cpp
 *
 *  Created on: 22 kwi 2016
 *      Author: tomasz
 */

#include "../../include/cplex/CPLEX_LP_MSTSolverIF.hpp"

#include <ilconcert/iloalg.h>
#include <ilconcert/iloextractable.h>
#include <ilconcert/ilosys.h>
#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <map>
#include <memory>
#include <sstream>
#include <utility>

#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"

class GraphIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("cplex.CPLEX_LP_MSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

EdgeSetIF* CPLEX_LP_MSTSolverIF::transformSolutionToEdgeSet() {
	EdgeSetIF* solution = new EdgeSetImpl { graph->getNumberOfVertices(
			Visibility::VISIBLE) - 1 };
	INFO_NOARG(logger, LogBundleKey::CPLPMSTIF_BUILD_EDGE_SET);
	for (auto& sourceVertex : edgeVariableMap) {
		for (auto& targetVertex : sourceVertex.second) {
			if (cplex.getValue(targetVertex.second.first) == 1) {
				INFO(logger, LogBundleKey::CPLPMSTIF_BUILD_EDGE_SET_ADD,
						LogStringUtils::edgeVisualization(
								graph->findEdge(sourceVertex.first,
										targetVertex.first), "\t").c_str());
				solution->push_back(
						getEdge(sourceVertex.first, targetVertex.first));
			}
		}
	}
	return solution;
}

IloEdgeValMap CPLEX_LP_MSTSolverIF::transformSolutionToRawMap() {
	IloEdgeValMap solution { };
	INFO_NOARG(logger, LogBundleKey::CPLPMSTIF_BUILD_RAW_SET);
	for (auto& sourceVertex : edgeVariableMap) {
		solution.insert(
				std::make_pair(sourceVertex.first,
						IloTargetVertexEdgeValMap { }));
		for (auto& targetVertex : sourceVertex.second) {
			solution.at(sourceVertex.first).insert(
					std::make_pair(targetVertex.first,
							(cplex.getValue(targetVertex.second.first) == 1)));
		}
	}
	return solution;
}

EdgeIF* CPLEX_LP_MSTSolverIF::getEdge(const VertexIdx sourceVertexIdx,
		const VertexIdx targetVertexIdx) {
	return edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).second;
}

IloNumVar CPLEX_LP_MSTSolverIF::getEdgeVariable(const VertexIdx sourceVertexIdx,
		const VertexIdx targetVertexIdx) {
	return edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first;
}

IloNumVar CPLEX_LP_MSTSolverIF::getEdgeVariable(VertexIF* const sourceVertex,
		VertexIF* const targetVertex) {
	return edgeVariableMap.at(sourceVertex->getVertexIdx()).at(
			targetVertex->getVertexIdx()).first;
}

IloNumVar CPLEX_LP_MSTSolverIF::getEdgeVariable(const EdgeIF* edge) {
	return edgeVariableMap.at(edge->getSourceVertex()->getVertexIdx()).at(
			edge->getTargetVertex()->getVertexIdx()).first;
}

std::string CPLEX_LP_MSTSolverIF::getVariableName(IloNumVar const & variable) {
	std::ostringstream oss { };
	oss << variable << std::flush;
	return oss.str();
}

std::string CPLEX_LP_MSTSolverIF::getCPLEXStatus(IloCplex& cplex) {
	std::ostringstream oss { };
	oss << cplex.getStatus() << std::flush;
	return oss.str();
}

EdgeSetIF * CPLEX_LP_MSTSolverIF::resolve(VertexIF * const initialVertex)
		throw (ModelExceptions::GeneralConcertException) {
	EdgeSetIF* solution { };
	buildModel();

	try {
		solution = LP_Solve();
	} catch (IloException& e) {
		ERROR(logger, LogBundleKey::CPLPMSTIF_SOLVER_ERROR, e.getMessage());
		throw ModelExceptions::GeneralConcertException();
	}
	INFO(logger, LogBundleKey::CPLPMSTIF_SOLUTION,
			LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
			solution->getTotalEdgeCost());
	return solution;
}

IloEdgeValMap CPLEX_LP_MSTSolverIF::resolveRAW()
		throw (ModelExceptions::GeneralConcertException) {
	IloEdgeValMap solution { };
	buildModel();

	try {
		solution = LP_RAW_Solve();
	} catch (IloException& e) {
		ERROR(logger, LogBundleKey::CPLPMSTIF_SOLVER_ERROR, e.getMessage());
		throw ModelExceptions::GeneralConcertException();
	}
	return solution;
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

CPLEX_LP_MSTSolverIF::CPLEX_LP_MSTSolverIF(GraphIF * graph,
		IloNumVar::Type edgeVariablesType) :
		MSTSolverIF(graph), numberOfVertices { graph->getNumberOfVertices(
				Visibility::VISIBLE) }, numberOfEdges { graph->getNumberOfEdges(
				Visibility::VISIBLE) } {

	model = IloModel(env);
	cplex = IloCplex(model);

	TRACE(logger, LogBundleKey::CPLPMSTIF_INIT,
			LogStringUtils::graphDescription(graph, "\t").c_str());

	solution = nullptr;
}

CPLEX_LP_MSTSolverIF::CPLEX_LP_MSTSolverIF(GraphIF * graph) :
		CPLEX_LP_MSTSolverIF(graph, IloNumVar::Type::Float) {

}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

