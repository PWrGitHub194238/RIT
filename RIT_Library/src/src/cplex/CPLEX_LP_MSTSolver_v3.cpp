/*
 * MSTSolver_v1.cpp
 *
 *  Created on: 2 mar 2016
 *      Author: tomasz
 */

#include "../../include/cplex/CPLEX_LP_MSTSolver_v3.hpp"

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
		log4cxx::Logger::getLogger("cplex.CPLEX_LP_MSTSolver_v3"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void CPLEX_LP_MSTSolver_v3::buildModel() {
	INFO_NOARG(logger, LogBundleKey::CPLPMST3_BUILD_MODEL);
	generateMoveCommodityFromSourceConstraints();
	generatePassCommodityConstraints();
	generateStoreCommodityConstraints();
	generateFlowConstraints();
	generateMSTConstraint();
	generateGoal();
}

void CPLEX_LP_MSTSolver_v3::generateMoveCommodityFromSourceConstraints() {
	INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_MOVES_SRC, initialVertexIdx);
	for (auto& nodeCommodity : commodityBasedflowVariablesMap) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_MOVES_SRC1,
				nodeCommodity.first);
		generateMoveCommodityFromSourceConstraint(nodeCommodity.second);
	}
}

void CPLEX_LP_MSTSolver_v3::generateMoveCommodityFromSourceConstraint(
		IloFlowVarMap& edgeVariableMapForCommodity) {
	IloNumVarArray toSourceInputFlow(env);
	IloNumVarArray fromSourceOutputFlow(env);

	for (auto& toSourceEdge : edgeVariableMapForCommodity) {
		if (isEdgeToSourceExists(toSourceEdge.second)) {
			INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_MOVES_OUT_TO_SRC,
					toSourceEdge.first, toSourceEdge.first, initialVertexIdx,
					getVariableName(
							getFlowVariable(toSourceEdge, initialVertexIdx)).c_str());
			toSourceInputFlow.add(
					getFlowVariable(toSourceEdge, initialVertexIdx));
		}
	}

	for (auto& fromSourceEdge : edgeVariableMapForCommodity.at(initialVertexIdx)) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_MOVES_FROM_SRC_OUT,
				fromSourceEdge.first, initialVertexIdx, fromSourceEdge.first,
				getVariableName(getFlowVariable(fromSourceEdge)).c_str());
		fromSourceOutputFlow.add(getFlowVariable(fromSourceEdge));
	}

	model.add(IloSum(toSourceInputFlow) - IloSum(fromSourceOutputFlow) == -1);
	toSourceInputFlow.end();
	fromSourceOutputFlow.end();
}

void CPLEX_LP_MSTSolver_v3::generatePassCommodityConstraints() {
	INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_PASS_MOVES,
			initialVertexIdx);

	for (auto& nodeCommodity : commodityBasedflowVariablesMap) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_PASS_MOVES1,
				nodeCommodity.first);
		generatePassCommodityConstraint(nodeCommodity.second,
				nodeCommodity.first);
	}
}

void CPLEX_LP_MSTSolver_v3::generatePassCommodityConstraint(
		IloFlowVarMap& edgeVariableMapForCommodity,
		VertexIdx nodeCommodityIdx) {
	/// zakładamy, że graf jest nieskierowany, więc wyciągniemy wszystkie id wierzchołków z 1 pętli
	for (auto& targetVertexIdx : edgeVariableMapForCommodity) {
		if (targetVertexIdx.first != nodeCommodityIdx
				&& targetVertexIdx.first != initialVertexIdx) {
			INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_PASS_IN_NODE,
					targetVertexIdx.first, nodeCommodityIdx, initialVertexIdx);
			generatePassCommodity(edgeVariableMapForCommodity,
					targetVertexIdx.first);
		}
	}
}

void CPLEX_LP_MSTSolver_v3::generatePassCommodity(
		IloFlowVarMap& edgeVariableMapForCommodity, VertexIdx transitNodeIdx) {
	IloNumVarArray transitNodeInputFlow(env);
	IloNumVarArray transitNodeOutputFlow(env);

	for (auto& toTransitNodeEdge : edgeVariableMapForCommodity) {
		if (isEdgeExists(toTransitNodeEdge.second, transitNodeIdx)) {
			INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_PASS_IN_TO_NODE,
					toTransitNodeEdge.first, transitNodeIdx,
					toTransitNodeEdge.first, transitNodeIdx,
					getVariableName(
							getFlowVariable(toTransitNodeEdge, transitNodeIdx)).c_str());
			transitNodeInputFlow.add(
					getFlowVariable(toTransitNodeEdge, transitNodeIdx));
		}
	}

	for (auto& fromTransitNodeEdge : edgeVariableMapForCommodity.at(
			transitNodeIdx)) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_PASS_FROM_NODE_OUT,
				transitNodeIdx, fromTransitNodeEdge.first, transitNodeIdx,
				fromTransitNodeEdge.first,
				getVariableName(getFlowVariable(fromTransitNodeEdge)).c_str());
		transitNodeOutputFlow.add(getFlowVariable(fromTransitNodeEdge));
	}

	model.add(
			IloSum(transitNodeInputFlow) - IloSum(transitNodeOutputFlow) == 0);
	transitNodeInputFlow.end();
	transitNodeOutputFlow.end();
}

void CPLEX_LP_MSTSolver_v3::generateStoreCommodityConstraints() {
	INFO_NOARG(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_STORE);

	for (auto& nodeCommodity : commodityBasedflowVariablesMap) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_STORE1,
				nodeCommodity.first);
		generateStoreCommodityConstraint(nodeCommodity.second,
				nodeCommodity.first);
	}
}

void CPLEX_LP_MSTSolver_v3::generateStoreCommodityConstraint(
		IloFlowVarMap& edgeVariableMapForCommodity,
		VertexIdx nodeCommodityIdx) {
	IloNumVarArray toCommodityNodeInputFlow(env);
	IloNumVarArray fromCommodityNodeOutputFlow(env);

	for (auto& toCommodityEdge : edgeVariableMapForCommodity) {
		if (toCommodityEdge.second.count(nodeCommodityIdx) == 1) {
			INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_STORE_TO_NODE,
					toCommodityEdge.first, nodeCommodityIdx, nodeCommodityIdx,
					nodeCommodityIdx, toCommodityEdge.first, nodeCommodityIdx,
					getVariableName(
							getFlowVariable(toCommodityEdge, nodeCommodityIdx)).c_str());
			toCommodityNodeInputFlow.add(
					getFlowVariable(toCommodityEdge, nodeCommodityIdx));
		}
	}

	for (auto& fromCommodityEdge : edgeVariableMapForCommodity.at(
			nodeCommodityIdx)) {
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_STORE_FROM_NODE,
				nodeCommodityIdx, fromCommodityEdge.first, nodeCommodityIdx,
				nodeCommodityIdx, fromCommodityEdge.first, nodeCommodityIdx,
				getVariableName(getFlowVariable(fromCommodityEdge)).c_str());
		fromCommodityNodeOutputFlow.add(getFlowVariable(fromCommodityEdge));
	}

	model.add(
			IloSum(toCommodityNodeInputFlow)
					- IloSum(fromCommodityNodeOutputFlow) == 1);
	toCommodityNodeInputFlow.end();
	fromCommodityNodeOutputFlow.end();
}

void CPLEX_LP_MSTSolver_v3::generateFlowConstraints() {
	INFO_NOARG(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_FLOW_CONSTRAINTS);

	for (auto& nodeCommodity : commodityBasedflowVariablesMap) {
		for (auto& sourceVertex : nodeCommodity.second) {
			for (auto& targetVertex : sourceVertex.second) {
				model.add(
						targetVertex.second
								<= getEdgeVariable(sourceVertex.first,
										targetVertex.first));
				INFO(logger,
						LogBundleKey::CPLPMST3_BUILD_MODEL_FLOW_CONSTRAINTS1,
						nodeCommodity.first, sourceVertex.first,
						targetVertex.first, sourceVertex.first,
						targetVertex.first,
						getVariableName(targetVertex.second).c_str(),
						getVariableName(
								getEdgeVariable(sourceVertex.first,
										targetVertex.first)).c_str());
			}
		}
	}
}

void CPLEX_LP_MSTSolver_v3::generateMSTConstraint() {
	INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_MST_CONSTRAINT,
			graph->getNumberOfVertices(Visibility::VISIBLE) - 1);
	model.add(
			IloSum(edgeVariableArray)
					== graph->getNumberOfVertices(Visibility::VISIBLE) - 1);
}

void CPLEX_LP_MSTSolver_v3::generateGoal() {
	IloExpr goalFormula(env);
	EdgeIF* edge { };

	INFO_NOARG(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_GOAL_CONSTRAINT);

	graph->beginEdge();
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edge = graph->nextEdge();
		INFO(logger, LogBundleKey::CPLPMST3_BUILD_MODEL_GOAL_ADD,
				edge->getSourceVertex()->getVertexIdx(),
				edge->getTargetVertex()->getVertexIdx(),
				edge->getSourceVertex()->getVertexIdx(),
				edge->getTargetVertex()->getVertexIdx(),
				edge->getTargetVertex()->getVertexIdx(),
				edge->getSourceVertex()->getVertexIdx(), edge->getEdgeCost(),
				getVariableName(
						getEdgeVariable(edge->getSourceVertex(),
								edge->getTargetVertex())).c_str(),
				getVariableName(
						getEdgeVariable(edge->getTargetVertex(),
								edge->getSourceVertex())).c_str());

		goalFormula += edge->getEdgeCost()
				* (getEdgeVariable(edge->getSourceVertex(),
						edge->getTargetVertex())
						+ getEdgeVariable(edge->getTargetVertex(),
								edge->getSourceVertex()));
	}
	model.add(IloMinimize(env, goalFormula));
	goalFormula.end();
}

EdgeSetIF* CPLEX_LP_MSTSolver_v3::LP_Solve() throw (IloException) {
	EdgeSetIF* solution { };
	cplex.extract(model);

	INFO_NOARG(logger, LogBundleKey::CPLPMST3_SOLVE_INIT);

	if (cplex.solve()) {
		INFO(logger, LogBundleKey::CPLPMST3_SOLUTION_FOUND,
				getCPLEXStatus(cplex).c_str(), (EdgeCost ) cplex.getObjValue());
		solution = transformSolutionToEdgeSet();
	} else {
		WARN_NOARG(logger, LogBundleKey::CPLPMST3_SOLVE_NO_SOLUTION);
	}
	env.end();
	return solution;
}

IloEdgeValMap CPLEX_LP_MSTSolver_v3::LP_RAW_Solve() throw (IloException) {
	IloEdgeValMap solution { };
	cplex.extract(model);

	INFO_NOARG(logger, LogBundleKey::CPLPMST3_SOLVE_INIT);

	if (cplex.solve()) {
		INFO(logger, LogBundleKey::CPLPMST3_SOLUTION_FOUND,
				getCPLEXStatus(cplex).c_str(), (EdgeCost ) cplex.getObjValue());
		solution = transformSolutionToRawMap();
	} else {
		WARN_NOARG(logger, LogBundleKey::CPLPMST3_SOLVE_NO_SOLUTION);
	}
	env.end();
	return solution;
}

void CPLEX_LP_MSTSolver_v3::createUndirectedEdgeVariables(
		GraphIF * const graph) {
	createUndirectedEdgeVariables(graph, IloNumVar::Type::Float);
}

void CPLEX_LP_MSTSolver_v3::createUndirectedEdgeVariables(GraphIF * const graph,
		IloNumVar::Type edgeVariablesType) {
	EdgeIdx edgeId { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeIF* edge { };
	IteratorId edgeIterator = graph->getEdgeIteratorId();

	INFO(logger, LogBundleKey::CPLPMST3_EDGE_VAR_GEN,
			2 * graph->getNumberOfEdges(Visibility::VISIBLE),
			EnumUtils::getLPVariableTypeString(
					static_cast<unsigned int>(edgeVariablesType)));
	edgeVariableArray = IloNumVarArray(env,
			2 * graph->getNumberOfEdges(Visibility::VISIBLE), 0, 1,
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

		INFO(logger, LogBundleKey::CPLPMST3_EDGE_VAR_ADD,
				LogStringUtils::edgeVisualization(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).second,
						"\t").c_str(), sourceVertexIdx, targetVertexIdx,
				getVariableName(
						edgeVariableMap.at(sourceVertexIdx).at(targetVertexIdx).first).c_str());

		edgeId += 1;

		if (edgeVariableMap.count(targetVertexIdx) == 0) {
			edgeVariableMap.insert(
					std::make_pair(targetVertexIdx,
							IloTargetVertexEdgeVarMap()));
		}

		edgeVariableMap.at(targetVertexIdx).insert(
				std::make_pair(sourceVertexIdx,
						std::make_pair(edgeVariableArray[edgeId], edge)));

		INFO(logger, LogBundleKey::CPLPMST3_EDGE_VAR_ADD,
				LogStringUtils::edgeVisualization(
						edgeVariableMap.at(targetVertexIdx).at(sourceVertexIdx).second,
						"\t").c_str(), targetVertexIdx, sourceVertexIdx,
				getVariableName(
						edgeVariableMap.at(targetVertexIdx).at(sourceVertexIdx).first).c_str());

		edgeId += 1;
	}

	graph->removeEdgeIterator(edgeIterator);
}

void CPLEX_LP_MSTSolver_v3::generateFlowVariables() {
	EdgeIdx edgeIdCount { };
	VertexIdx vertexIdCount { };
	VertexIdx commodityVertexIdx { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeIF* edge { };
	VertexIF* vertex { };

	commodityBasedflowVariables = IloArray<IloNumVarArray>(env,
			numberOfVertices - 1);

	INFO_NOARG(logger, LogBundleKey::CPLPMST3_FLOW_GEN);

	graph->beginVertex();
	if (graph->hasNextVertex(Visibility::VISIBLE)) {
		initialVertexIdx = graph->nextVertex()->getVertexIdx();
		INFO(logger, LogBundleKey::CPLPMST3_FLOW_GEN_INIT_NODE,
				initialVertexIdx);
		vertexIdCount = 0;
		while (graph->hasNextVertex(Visibility::VISIBLE)) {
			vertex = graph->nextVertex();
			commodityVertexIdx = vertex->getVertexIdx();
			INFO(logger, LogBundleKey::CPLPMST3_FLOW_GEN_COMMODITY,
					2 * numberOfEdges, commodityVertexIdx);

			commodityBasedflowVariables[vertexIdCount] = IloNumVarArray(env,
					2 * numberOfEdges, 0, IloInfinity);
			model.add(commodityBasedflowVariables[vertexIdCount]);

			commodityBasedflowVariablesMap.insert(
					std::make_pair(commodityVertexIdx, IloFlowVarMap()));

			edgeIdCount = 0;

			for (auto& sourceVertex : edgeVariableMap) {
				sourceVertexIdx = sourceVertex.first;
				if (commodityBasedflowVariablesMap.at(commodityVertexIdx).count(
						sourceVertexIdx) == 0) {
					commodityBasedflowVariablesMap.at(commodityVertexIdx).insert(
							std::make_pair(sourceVertexIdx,
									IloTargetVertexFlowVarMap()));
				}
				for (auto& targetVertex : sourceVertex.second) {
					targetVertexIdx = targetVertex.first;
					INFO(logger, LogBundleKey::CPLPMST3_FLOW_GEN_COMMODITY_VAR,
							commodityVertexIdx, sourceVertexIdx,
							targetVertexIdx,
							getVariableName(
									commodityBasedflowVariables[vertexIdCount][edgeIdCount]).c_str());

					commodityBasedflowVariablesMap.at(commodityVertexIdx).at(
							sourceVertexIdx).insert(
							std::make_pair(targetVertexIdx,
									commodityBasedflowVariables[vertexIdCount][edgeIdCount]));
					edgeIdCount += 1;
				}
			}
			vertexIdCount += 1;
		}
	}
}

IloNumVar CPLEX_LP_MSTSolver_v3::getFlowVariable(
		IloFlowVarMapEntry& targetVertexFlowVarMap,
		const VertexIdx& targetVertexIdx) {
	return targetVertexFlowVarMap.second.at(targetVertexIdx);
}

IloNumVar CPLEX_LP_MSTSolver_v3::getFlowVariable(
		IloTargetVertexFlowVarEntry& targetVertexFlowVarEntry) {
	return targetVertexFlowVarEntry.second;
}

bool CPLEX_LP_MSTSolver_v3::isEdgeToSourceExists(
		IloTargetVertexFlowVarMap& targetVertexFlowVarEntry) {
	return targetVertexFlowVarEntry.count(initialVertexIdx) == 1;
}

bool CPLEX_LP_MSTSolver_v3::isEdgeExists(
		IloTargetVertexFlowVarMap& targetVertexFlowVarEntry,
		VertexIdx targetVertexIdx) {
	return targetVertexFlowVarEntry.count(targetVertexIdx) == 1;
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

CPLEX_LP_MSTSolver_v3::CPLEX_LP_MSTSolver_v3(GraphIF * graph) :
		CPLEX_LP_MSTSolverIF(graph) {
	createUndirectedEdgeVariables(graph);
	generateFlowVariables();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//
