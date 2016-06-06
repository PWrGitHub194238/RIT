/*
 * MSTSolver_v1.cpp
 *
 *  Created on: 2 mar 2016
 *      Author: tomasz
 */

#include "../../include/cplex/CPLEX_LP_MSTSolver_v2.hpp"

#include <ilconcert/iloenv.h>
#include <ilconcert/iloextractable.h>
#include <ilconcert/ilolinear.h>
#include <ilconcert/ilosys.h>
#include <ilcplex/ilocplexi.h>
#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "../../include/enums/Connectivity.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/utils/EnumUtils.hpp"

class GraphIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("cplex.CPLEX_LP_MSTSolver_v2"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void CPLEX_LP_MSTSolver_v2::buildModel() {
	INFO_NOARG(logger, LogBundleKey::CPLPMST2_BUILD_MODEL);
	generateFlowConstraints();
	generateFlowBoundsConstraints();
	generateMSTConstraint();
	generateGoal();
}

void CPLEX_LP_MSTSolver_v2::generateFlowConstraints() {
	INFO_NOARG(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_CONSTRAINTS);
	if (graph->hasAnyVertex(Visibility::VISIBLE)) {
		generateFlowConstraint(graph->nextVertex(), numberOfVertices - 1);
	}
	while (graph->hasNextVertex(Visibility::VISIBLE)) {
		generateFlowConstraint(graph->nextVertex(), -1);
	}
}

void CPLEX_LP_MSTSolver_v2::generateFlowConstraint(VertexIF* const vertex,
		int cost) {
	IloExpr inputFlow(env);
	IloExpr outputFlow(env);
	VertexIdx targetVertexIdx { };
	VertexIdx sourceVertexIdx { vertex->getVertexIdx() };

	INFO(logger,
			(cost == numberOfVertices - 1) ?
					LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_SRC_CONSTRAINT :
					LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_CONSTRAINT,
			vertex->getVertexIdx());

	vertex->beginOutputEdges();
	while (vertex->hasNextOutputEdge(Visibility::VISIBLE)) {
		targetVertexIdx =
				vertex->nextOutputEdge()->getOtherVertex(vertex)->getVertexIdx();
		INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_CONSTRAINT_ADD_OUT,
				sourceVertexIdx, targetVertexIdx,
				getVariableName(
						flowVariablesMap.at(sourceVertexIdx).at(
								targetVertexIdx)).c_str());
		outputFlow += flowVariablesMap.at(sourceVertexIdx).at(targetVertexIdx);
	}

	targetVertexIdx = sourceVertexIdx;

	vertex->beginInputEdges();
	while (vertex->hasNextInputEdge(Visibility::VISIBLE)) {
		sourceVertexIdx =
				vertex->nextInputEdge()->getOtherVertex(vertex)->getVertexIdx();
		INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_CONSTRAINT_ADD_IN,
				sourceVertexIdx, targetVertexIdx,
				getVariableName(
						flowVariablesMap.at(sourceVertexIdx).at(
								targetVertexIdx)).c_str());
		inputFlow += flowVariablesMap.at(sourceVertexIdx).at(targetVertexIdx);
	}

	model.add(outputFlow - inputFlow == cost);

	inputFlow.end();
	outputFlow.end();
}

void CPLEX_LP_MSTSolver_v2::generateFlowBoundsConstraints() {
	EdgeIF* edge { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeCount flowUpperBound { numberOfVertices - 1 };

	INFO_NOARG(logger,
			LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_BOUNDS_CONSTRAINTS);

	graph->beginEdge();
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edge = graph->nextEdge();
		sourceVertexIdx = edge->getSourceVertex()->getVertexIdx();
		targetVertexIdx = edge->getTargetVertex()->getVertexIdx();

		INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_BOUNDS_CONSTRAINT,
				sourceVertexIdx, targetVertexIdx, flowUpperBound,
				sourceVertexIdx, targetVertexIdx,
				getVariableName(
						flowVariablesMap.at(sourceVertexIdx).at(
								targetVertexIdx)).c_str(), flowUpperBound,
				getVariableName(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first).c_str());

		model.add(
				flowVariablesMap.at(sourceVertexIdx).at(targetVertexIdx)
						<= IloNum(flowUpperBound)
								* edgeVariableMap.at(sourceVertexIdx).at(
										targetVertexIdx).first);

		INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_FLOW_BOUNDS_CONSTRAINT,
				targetVertexIdx, sourceVertexIdx, flowUpperBound,
				sourceVertexIdx, targetVertexIdx,
				getVariableName(
						flowVariablesMap.at(targetVertexIdx).at(
								sourceVertexIdx)).c_str(), flowUpperBound,
				getVariableName(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first).c_str());

		model.add(
				flowVariablesMap.at(targetVertexIdx).at(sourceVertexIdx)
						<= IloNum(flowUpperBound)
								* edgeVariableMap.at(sourceVertexIdx).at(
										targetVertexIdx).first);

	}
}

void CPLEX_LP_MSTSolver_v2::generateMSTConstraint() {
	INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_MST_CONSTRAINT,
			graph->getNumberOfVertices(Visibility::VISIBLE) - 1);
	model.add(
			IloSum(edgeVariableArray)
					== graph->getNumberOfVertices(Visibility::VISIBLE) - 1);
}

void CPLEX_LP_MSTSolver_v2::generateGoal() {
	IloExpr goalFormula(env);
	EdgeIF* edge { };

	INFO_NOARG(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_GOAL_CONSTRAINT);

	graph->beginEdge();
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edge = graph->nextEdge();
		INFO(logger, LogBundleKey::CPLPMST2_BUILD_MODEL_GOAL_ADD,
				edge->getSourceVertex()->getVertexIdx(),
				edge->getTargetVertex()->getVertexIdx(),
				edge->getSourceVertex()->getVertexIdx(),
				edge->getSourceVertex()->getVertexIdx(), edge->getEdgeCost(),
				getVariableName(
						getEdgeVariable(edge->getSourceVertex(),
								edge->getTargetVertex())).c_str());

		goalFormula += edge->getEdgeCost()
				* getEdgeVariable(edge->getSourceVertex(),
						edge->getTargetVertex());
	}
	model.add(IloMinimize(env, goalFormula));
}

EdgeSetIF* CPLEX_LP_MSTSolver_v2::LP_Solve() throw (IloException) {
	EdgeSetIF* solution { };
	cplex.extract(model);

	INFO_NOARG(logger, LogBundleKey::CPLPMST2_SOLVE_INIT);

	if (cplex.solve()) {
		INFO(logger, LogBundleKey::CPLPMST2_SOLUTION_FOUND,
				getCPLEXStatus(cplex).c_str(), (EdgeCost ) cplex.getObjValue());
		solution = transformSolutionToEdgeSet();
	} else {
		WARN_NOARG(logger, LogBundleKey::CPLPMST2_SOLVE_NO_SOLUTION);
	}
	env.end();
	return solution;
}

IloEdgeValMap CPLEX_LP_MSTSolver_v2::LP_RAW_Solve() throw (IloException) {
	IloEdgeValMap solution { };
	cplex.extract(model);

	INFO_NOARG(logger, LogBundleKey::CPLPMST2_SOLVE_INIT);

	if (cplex.solve()) {
		INFO(logger, LogBundleKey::CPLPMST2_SOLUTION_FOUND,
				getCPLEXStatus(cplex).c_str(), (EdgeCost ) cplex.getObjValue());
		solution = transformSolutionToRawMap();
	} else {
		WARN_NOARG(logger, LogBundleKey::CPLPMST2_SOLVE_NO_SOLUTION);
	}
	env.end();
	return solution;
}

void CPLEX_LP_MSTSolver_v2::createEdgeVariables(GraphIF * const graph) {
	createEdgeVariables(graph, IloNumVar::Type::Bool);
}

void CPLEX_LP_MSTSolver_v2::createEdgeVariables(GraphIF * const graph,
		IloNumVar::Type edgeVariablesType) {
	EdgeIdx edgeId { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeIF* edge { };
	IteratorId edgeIterator = graph->getEdgeIteratorId();

	INFO(logger, LogBundleKey::CPLPMST2_EDGE_VAR_GEN,
			graph->getNumberOfEdges(Visibility::VISIBLE),
			EnumUtils::getLPVariableTypeString(
					static_cast<unsigned int>(edgeVariablesType)));
	edgeVariableArray = IloNumVarArray(env,
			graph->getNumberOfEdges(Visibility::VISIBLE), 0, 1,
			edgeVariablesType);
	model.add(edgeVariableArray);

	graph->beginEdge(edgeIterator);

	while (graph->hasNextEdge(edgeIterator, Connectivity::CONNECTED,
			Visibility::VISIBLE)) {
		edge = graph->nextEdge(edgeIterator);
		sourceVertexIdx = edge->getSourceVertex()->getVertexIdx();
		targetVertexIdx = edge->getTargetVertex()->getVertexIdx();

		if (edgeVariableMap.count(sourceVertexIdx) == 0) {
			edgeVariableMap.insert(
					std::make_pair(sourceVertexIdx,
							IloTargetVertexEdgeVarMap()));
		}

		edgeVariableMap.at(sourceVertexIdx).insert(
				std::make_pair(targetVertexIdx,
						std::make_pair(edgeVariableArray[edgeId], edge)));

		INFO(logger, LogBundleKey::CPLPMST2_EDGE_VAR_ADD,
				LogStringUtils::edgeVisualization(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).second,
						"\t").c_str(), sourceVertexIdx, targetVertexIdx,
				getVariableName(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first).c_str());

		edgeId += 1;
	}

	graph->removeEdgeIterator(edgeIterator);
}

void CPLEX_LP_MSTSolver_v2::generateFlowVariables() {
	EdgeIdx edgeIdCount { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeIF* edge { };
	VertexIF* vertex { };

	flowVariables = IloNumVarArray(env, 2 * numberOfEdges, 0, IloInfinity);

	INFO_NOARG(logger, LogBundleKey::CPLPMST2_FLOW_GEN);

	if (graph->hasAnyVertex(Visibility::VISIBLE)) {
		initialVertexIdx = graph->nextVertex()->getVertexIdx();

		INFO(logger, LogBundleKey::CPLPMST2_FLOW_GEN_INIT_NODE,
				initialVertexIdx);

		flowVariablesMap.insert(
				std::make_pair(initialVertexIdx,
						IloTargetVertexFlowVarMap { }));

		while (graph->hasNextVertex(Visibility::VISIBLE)) {
			flowVariablesMap.insert(
					std::make_pair(graph->nextVertex()->getVertexIdx(),
							IloTargetVertexFlowVarMap { }));
		}

		edgeIdCount = 0;

		graph->beginEdge();
		while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
			edge = graph->nextEdge();
			sourceVertexIdx = edge->getSourceVertex()->getVertexIdx();
			targetVertexIdx = edge->getTargetVertex()->getVertexIdx();

			INFO(logger, LogBundleKey::CPLPMST2_FLOW_GEN_VAR, sourceVertexIdx,
					targetVertexIdx,
					getVariableName(flowVariables[edgeIdCount]).c_str());

			flowVariablesMap.at(sourceVertexIdx).insert(
					std::make_pair(targetVertexIdx,
							flowVariables[edgeIdCount]));

			edgeIdCount += 1;

			INFO(logger, LogBundleKey::CPLPMST2_FLOW_GEN_VAR, sourceVertexIdx,
					targetVertexIdx,
					getVariableName(flowVariables[edgeIdCount]).c_str());

			flowVariablesMap.at(targetVertexIdx).insert(
					std::make_pair(sourceVertexIdx,
							flowVariables[edgeIdCount]));

			edgeIdCount += 1;
		}
	}
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

CPLEX_LP_MSTSolver_v2::CPLEX_LP_MSTSolver_v2(GraphIF * graph) :
		CPLEX_LP_MSTSolverIF(graph) {
	createEdgeVariables(graph);
	generateFlowVariables();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//
