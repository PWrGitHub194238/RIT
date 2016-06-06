/*
 * TabuSearch.cpp
 *
 *  Created on: 14 kwi 2016
 *      Author: tomasz
 */

#include "../../include/rimstsolver/TabuSearch.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>
#include <utility>

#include "../../include/aimstsolver/AIMSTSolverIF.hpp"
#include "../../include/enums/Connectivity.hpp"
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
#include "../../include/utils/AIMSTUtils.hpp"
#include "../../include/utils/EdgeSetUtils.hpp"
#include "../../include/utils/GraphUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"
#include "../../include/utils/TabuSearchUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("rimstsolver.TabuSearch"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

GraphEdgeCostsIF * TabuSearch::getWorstCaseScenario(EdgeSetIF * spanningTree) {
	GraphEdgeCostsIF * worstCaseScenario = new GraphEdgeCostsImpl { };
	EdgeIdx edgeIdx { };
	EdgeCost edgeCost { };
	EdgeCost scenarioEdgeCost { };

	INFO(logger, LogBundleKey::TS_WORST_CASE_GEN,
			LogStringUtils::edgeSetVisualization(spanningTree, "\t").c_str(),
			this->adversarialScenarioSet.size());
	// Fill with best costs
	graph->beginEdge();
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edgeCost = GraphUtils::MAX_EDGE_COST;
		INFO(logger, LogBundleKey::TS_WORST_CASE_SEARCH_BEST_EDGE_COST,
				graph->currentEdge()->getEdgeIdx(),
				LogStringUtils::edgeVisualization(graph->currentEdge(), "\t").c_str(),
				this->adversarialScenarioSet.size());
		edgeIdx = graph->nextEdge()->getEdgeIdx();
		for (auto& advScenario : this->adversarialScenarioSet) {
			scenarioEdgeCost = advScenario->at(edgeIdx);
			INFO(logger, LogBundleKey::TS_WORST_CASE_EDGE_SCENARIO_BEST_COST,
					edgeIdx, advScenario->getName().c_str(), scenarioEdgeCost);

			if (edgeCost > scenarioEdgeCost) {
				edgeCost = scenarioEdgeCost;
			}
		}
		INFO(logger, LogBundleKey::TS_WORST_CASE_EDGE_BEST_COST, edgeIdx,
				edgeCost);

		worstCaseScenario->push_back(edgeCost);
	}

	INFO(logger, LogBundleKey::TS_WORST_CASE_EDGE_WORST_COST_GEN,
			LogStringUtils::edgeSetVisualization(spanningTree, "\t").c_str());

	// Change costs for edges in tree
	spanningTree->begin();
	while (spanningTree->hasNext()) {
		edgeCost = GraphUtils::MIN_EDGE_COST;
		INFO(logger, LogBundleKey::TS_WORST_CASE_SEARCH_WORST_EDGE_COST,
				spanningTree->current()->getEdgeIdx(),
				LogStringUtils::edgeVisualization(spanningTree->current(), "\t").c_str(),
				this->adversarialScenarioSet.size());
		edgeIdx = spanningTree->next()->getEdgeIdx();
		for (auto& advScenario : this->adversarialScenarioSet) {
			scenarioEdgeCost = advScenario->at(edgeIdx);
			INFO(logger, LogBundleKey::TS_WORST_CASE_EDGE_SCENARIO_WORST_COST,
					edgeIdx, advScenario->getName().c_str(), scenarioEdgeCost);

			if (edgeCost < scenarioEdgeCost) {
				edgeCost = scenarioEdgeCost;
			}
		}
		INFO(logger, LogBundleKey::TS_WORST_CASE_EDGE_WORST_COST, edgeIdx,
				edgeCost);

		(*worstCaseScenario)[edgeIdx] = edgeCost;
	}
	INFO(logger, LogBundleKey::TS_WORST_CASE_GENERATED,
			LogStringUtils::edgeCostSetDescription(worstCaseScenario, "\t").c_str());
	return worstCaseScenario;
}

EdgeSetIF * TabuSearch::getWorstCaseAlternative(
		GraphEdgeCostsIF * worstCaseScenario) {
	GraphEdgeCostsIF* graphEdgeCostsBackup = new GraphEdgeCostsImpl { graph };
	GraphUtils::changeGraphCosts(graph, worstCaseScenario);
	EdgeSetIF * mstForWorstScenario = this->mstSolver->getMST();
	GraphUtils::changeGraphCosts(graph, graphEdgeCostsBackup);
	delete graphEdgeCostsBackup;
	return mstForWorstScenario;
}

bool TabuSearch::stopCriterionMet(TabuIterationCount iterationCount) {
	INFO(logger, LogBundleKey::TS_RESOLVE_CHECK_STOP, iterationCount + 1,
			numberOfIterations);
	return iterationCount >= numberOfIterations;
}

bool TabuSearch::isAspirationCriteriaMet(EdgeCost const neighborCost,
		EdgeCost const tmpSolutionCost) {
	INFO(logger,
			(neighborCost < tmpSolutionCost ?
					LogBundleKey::TS_FIND_NEIGHBORHOOD_ASP_CRIT_TRUE :
					LogBundleKey::TS_FIND_NEIGHBORHOOD_ASP_CRIT_FALSE),
			neighborCost, tmpSolutionCost);
	return neighborCost < tmpSolutionCost;
}

TabuSubMap TabuSearch::getTabuList(TabuMapEnum tabuListType) {
	return tabuListType == TabuMapEnum::DROP ?
			std::get<0>(tabuMap) : std::get<1>(tabuMap);
}

bool TabuSearch::isMoveAllowed(EdgeIdx const dropEdgeIdx,
		EdgeIdx const addEdgeIdx) {
	INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_TABU_CRIT, dropEdgeIdx,
			addEdgeIdx);
	if (getTabuList(TabuMapEnum::DROP).count(dropEdgeIdx) != 0) {
		INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_TABU_CANNOT_DROP,
				dropEdgeIdx, getTabuList(TabuMapEnum::DROP).at(dropEdgeIdx));
		return false;
	}
	if (getTabuList(TabuMapEnum::ADD).count(addEdgeIdx) != 0) {
		INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_TABU_CANNOT_ADD,
				addEdgeIdx, getTabuList(TabuMapEnum::ADD).at(addEdgeIdx));
		return false;
	}
	INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_TABU_CRIT_OK, dropEdgeIdx,
			addEdgeIdx);
	return true;
}

AIMSTSolution TabuSearch::getSolutionForTree(EdgeSetIF* initialSolution) {
	IteratorId graphIteratorId = graph->getEdgeIteratorId();
	VisibilityList visibilityList = graph->storeEdgeVisibility(graphIteratorId);
	graph->showAllEdges(graphIteratorId);
	AIMSTSolution solution = aimstSolver->getMST(initialSolution);
	graph->restoreVisibilityAllEdges(visibilityList, graphIteratorId);
	graph->removeEdgeIterator(graphIteratorId);
	return solution;
}

NeighborSolution TabuSearch::findMinimumInNeighborhood(
		EdgeSetIF* const rootSpanningTree, EdgeCost const tmpSolutionCost) {
	NeighborSolution neighbor { };
	EdgeIdx dropEdgeIdx { };
	EdgeIdx addEdgeIdx { };
	AIMSTSolution neighborSolutionSet { };
	EdgeCost neighborCost { };
	EdgeSetIF* bestNeighbourEdgeSet { };
	EdgeCost bestNeighbourCost { GraphUtils::MAX_EDGE_COST };
	EdgeSetIF* path { };
	IteratorId edgeIteratorId = graph->getEdgeIteratorId();
	IteratorId pathIteratorId = graph->getEdgeIteratorId();

	VisibilityList visibilityList = graph->storeEdgeVisibility();
	EdgeIF * noMSTedge { };
	EdgeIF * pathEdge { };

	INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_INIT,
			LogStringUtils::edgeSetVisualization(rootSpanningTree, "\t").c_str());
	rootSpanningTree->begin(edgeIteratorId);
	while (rootSpanningTree->hasNext(edgeIteratorId)) {
		rootSpanningTree->next(edgeIteratorId)->hide();
	}
	graph->beginEdge(edgeIteratorId);
	while (graph->hasNextEdge(edgeIteratorId, Connectivity::CONNECTED,
			Visibility::VISIBLE)) {
		noMSTedge = graph->nextEdge(edgeIteratorId);
		INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_ADD_EDGE,
				LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str());
		path = GraphUtils::getMSTEdgesBeetwenNodes(graph, rootSpanningTree,
				noMSTedge->getSourceVertex(), noMSTedge->getTargetVertex());
		rootSpanningTree->push_back(noMSTedge);
		INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_PATH_INSTEAD_EDGE,
				LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str(),
				noMSTedge->getSourceVertex()->getVertexIdx(),
				noMSTedge->getTargetVertex()->getVertexIdx(),
				LogStringUtils::edgeSetVisualization(path, "\t").c_str());
		path->begin(pathIteratorId);
		while (path->hasNext(pathIteratorId)) {
			pathEdge = path->next(pathIteratorId);
			rootSpanningTree->remove(pathEdge);

			addEdgeIdx = noMSTedge->getEdgeIdx();
			dropEdgeIdx = pathEdge->getEdgeIdx();

			neighborSolutionSet = getSolutionForTree(rootSpanningTree);
			neighborCost = rootSpanningTree->getTotalEdgeCost()
					+ AIMSTUtils::getSolutionCost(neighborSolutionSet);

			INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_GEN_SET,
					LogStringUtils::edgeSetVisualization(rootSpanningTree, "\t").c_str(),
					addEdgeIdx,
					LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str(),
					dropEdgeIdx,
					LogStringUtils::edgeVisualization(pathEdge, "\t").c_str(),
					neighborCost);

			if (isMoveAllowed(dropEdgeIdx, addEdgeIdx)
					|| isAspirationCriteriaMet(neighborCost, tmpSolutionCost)) {
				if (neighborCost < bestNeighbourCost) {
					INFO(logger,
							LogBundleKey::TS_FIND_NEIGHBORHOOD_BETTER_MOVE_FOUND,
							dropEdgeIdx, addEdgeIdx,
							LogStringUtils::edgeSetVisualization(
									rootSpanningTree, "\t").c_str(),
							neighborCost,
							LogStringUtils::wrapInfinity(bestNeighbourCost).c_str());
					MemoryUtils::removeCollection(bestNeighbourEdgeSet, false);
					bestNeighbourEdgeSet =
							new EdgeSetImpl { AIMSTUtils::getEdgeSet(
									neighborSolutionSet), false };
					bestNeighbourCost = neighborCost;
					neighbor = std::make_tuple(
							TabuSearchUtils::createSpanningTreeNeighbor(
									dropEdgeIdx, addEdgeIdx,
									bestNeighbourEdgeSet), bestNeighbourCost);
				}
			}
			rootSpanningTree->push_back(pathEdge);
			MemoryUtils::removeCollection(
					AIMSTUtils::getEdgeSet(neighborSolutionSet), false);
		}
		rootSpanningTree->remove(noMSTedge);
		MemoryUtils::removeCollection(path, false);
	}
	graph->removeEdgeIterator(pathIteratorId);
	graph->removeEdgeIterator(edgeIteratorId);
	graph->restoreVisibilityAllEdges(visibilityList);
	INFO(logger, LogBundleKey::TS_FIND_NEIGHBORHOOD_END,
			LogStringUtils::edgeSetVisualization(rootSpanningTree, "\t").c_str(),
			LogStringUtils::edgeSetVisualization(bestNeighbourEdgeSet, "\t").c_str(),
			bestNeighbourCost);
	return neighbor;
}

void TabuSearch::insertIntoTabu(TabuMapEnum const tabuListType,
		EdgeIdx const edgeIdx) {
	insertIntoTabu(tabuListType, edgeIdx,
			TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD);
}

void TabuSearch::insertIntoTabu(TabuMapEnum const tabuListType,
		EdgeIdx const edgeIdx, TabuIterationCount tabuPeriod) {
	INFO(logger,
			(tabuListType == TabuMapEnum::DROP ?
					LogBundleKey::TS_FIND_NEIGHBORHOOD_ADD_TABU_DROP :
					LogBundleKey::TS_FIND_NEIGHBORHOOD_ADD_TABU_ADD), edgeIdx,
			tabuPeriod);
	getTabuList(tabuListType).insert(std::make_pair(edgeIdx, tabuPeriod));
}

void TabuSearch::eraseTabuList() {
	getTabuList(TabuMapEnum::DROP).clear();
	getTabuList(TabuMapEnum::ADD).clear();
}

void TabuSearch::updateTabuList(SpanningTreeNeighbor && tabuMove) {
	TabuSubMap tabuList { };
	INFO_NOARG(logger, LogBundleKey::TS_UPDATE_TABU);

	tabuList = getTabuList(TabuMapEnum::DROP);

	for (auto it = tabuList.cbegin(); it != tabuList.cend();
	/* no increment */) {
		if (it->second == 0) {
			tabuList.erase(it++);
		} else {
			tabuList[it->first] -= 1;
			++it;
		}
	}

	tabuList = getTabuList(TabuMapEnum::ADD);
	for (auto it = tabuList.cbegin(); it != tabuList.cend();
	/* no increment */) {
		if (it->second == 0) {
			tabuList.erase(it++);
		} else {
			tabuList[it->first] -= 1;
			++it;
		}
	}

	insertIntoTabu(TabuMapEnum::DROP,
			TabuSearchUtils::getRemovedEdge(tabuMove));
	insertIntoTabu(TabuMapEnum::ADD,
			TabuSearchUtils::getInsertedEdge(tabuMove));
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

EdgeSetIF * TabuSearch::resolve() {
	EdgeSetIF * solution { };
	EdgeCost solutionCost { };
	EdgeSetIF* nextEdgeSet { };
	EdgeSetIF* treeWorstCaseAlternative { };
	GraphEdgeCostsIF* worstCaseScenario { };
	NeighborSolution bestNeighbor { };
	EdgeSetIF* bestNeighborSpanningTree { };
	TabuIterationCount iterationCount { 0 };
	TabuIterationCount globalIterationCount { 0 };

	INFO(logger, LogBundleKey::TS_RESOLVE_INIT,
			LogStringUtils::graphDescription(graph, "\t").c_str());

	EdgeSetIF* spanningTree = GraphUtils::getRandomSpanningTree(graph);

	EdgeCost spanningTreeCost = spanningTree->getTotalEdgeCost(
			Visibility::VISIBLE);

	INFO(logger, LogBundleKey::TS_RESOLVE_INIT_RANDOM,
			LogStringUtils::edgeSetVisualization(spanningTree, "\t").c_str(),
			spanningTreeCost);

	AIMSTSolution initialSolution = getSolutionForTree(spanningTree);
	solution = AIMSTUtils::getEdgeSet(initialSolution);
	solutionCost = spanningTreeCost
			+ AIMSTUtils::getSolutionCost(initialSolution);

	INFO(logger, LogBundleKey::TS_RESOLVE_INIT_SOL,
			LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
			LogStringUtils::edgeCostSetDescription(
					AIMSTUtils::getScenario(initialSolution), "\t").c_str(),
			AIMSTUtils::getSolutionCost(initialSolution), spanningTreeCost,
			solutionCost);

	worstCaseScenario = getWorstCaseScenario(solution);
	nextEdgeSet = getWorstCaseAlternative(worstCaseScenario);

	while (!stopCriterionMet(globalIterationCount)) {
		bestNeighbor = findMinimumInNeighborhood(spanningTree, solutionCost);
		MemoryUtils::removeCollection(spanningTree, false);
		spanningTree = TabuSearchUtils::getEdgeSet(bestNeighbor);
		spanningTreeCost = TabuSearchUtils::getEdgeSetCost(bestNeighbor);

		INFO(logger, LogBundleKey::TS_RESOLVE_NEIGHBOR_SOL,
				LogStringUtils::edgeSetVisualization(spanningTree, "\t").c_str(),
				spanningTreeCost, solutionCost);

		globalIterationCount += 1;
		iterationCount += 1;

		INFO(logger, LogBundleKey::TS_RESOLVE_STATE, iterationCount,
				numberOfPathIterations, globalIterationCount);

		if (spanningTreeCost < solutionCost) {
			MemoryUtils::removeCollection(solution, false);
			solution = new EdgeSetImpl { spanningTree, false };
			INFO(logger, LogBundleKey::TS_RESOLVE_BETTER,
					LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
					spanningTreeCost, solutionCost);
			solutionCost = spanningTreeCost;
			delete worstCaseScenario;
			worstCaseScenario = getWorstCaseScenario(solution);
			treeWorstCaseAlternative = getWorstCaseAlternative(
					worstCaseScenario);
			nextEdgeSet = EdgeSetUtils::getSetUnion(nextEdgeSet,
					treeWorstCaseAlternative, true);
			iterationCount = 0;
		}

		if (iterationCount > numberOfPathIterations) {
			MemoryUtils::removeCollection(spanningTree, false);
			spanningTree = GraphUtils::getRandomSpanningTree(graph,
					nextEdgeSet);
			spanningTreeCost = spanningTree->getTotalEdgeCost(
					Visibility::VISIBLE);
			initialSolution = getSolutionForTree(spanningTree);
			MemoryUtils::removeCollection(spanningTree, false);
			spanningTree = AIMSTUtils::getEdgeSet(initialSolution);
			spanningTreeCost = spanningTreeCost
					+ AIMSTUtils::getSolutionCost(initialSolution);

			INFO(logger, LogBundleKey::TS_RESOLVE_RESTART, iterationCount - 1,
					numberOfPathIterations, solutionCost,
					LogStringUtils::edgeSetVisualization(spanningTree, "\t").c_str(),
					LogStringUtils::edgeCostSetDescription(
							AIMSTUtils::getScenario(initialSolution)).c_str(),
					AIMSTUtils::getSolutionCost(initialSolution),
					spanningTreeCost
							- AIMSTUtils::getSolutionCost(initialSolution),
					spanningTreeCost);

			if (spanningTreeCost < solutionCost) {
				solution = spanningTree;
				solutionCost = spanningTreeCost;
				INFO(logger, LogBundleKey::TS_RESOLVE_BETTER,
						LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
						solutionCost);
			}
			eraseTabuList();
			delete worstCaseScenario;
			worstCaseScenario = getWorstCaseScenario(spanningTree);
			MemoryUtils::removeCollection(nextEdgeSet, false);
			nextEdgeSet = getWorstCaseAlternative(worstCaseScenario);
			iterationCount = 0;
			continue;
		} else {
			updateTabuList(TabuSearchUtils::getMove(bestNeighbor));
		}
	}
	MemoryUtils::removeCollection(spanningTree, false);
	delete worstCaseScenario;
	MemoryUtils::removeCollection(nextEdgeSet, false);
	INFO(logger, LogBundleKey::TS_RESOLVE_END,
			LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
			solutionCost);
	return solution;
}

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

TabuSearch::TabuSearch(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		MSTSolverEnum innerMstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
		TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		RIMSTSolverIF(aimstSolverType, imstSolverType, mstSolverType,
				innerMstSolverType, graph, adversarialScenarioSet, k) {
	this->tabuPeriod = tabuPeriod;
	this->numberOfPathIterations = numberOfPathIterations;
	this->numberOfIterations = numberOfIterations;
}

TabuSearch::TabuSearch(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		MSTSolverEnum innerMstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		TabuSearch(aimstSolverType, imstSolverType, mstSolverType,
				innerMstSolverType, graph, adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k, TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(aimstSolverType, imstSolverType, mstSolverType,
				MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k,
				tabuPeriod, numberOfPathIterations, numberOfIterations) {
}

TabuSearch::TabuSearch(AIMSTSolverEnum aimstSolverType,
		IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) :
		TabuSearch(aimstSolverType, imstSolverType, mstSolverType,
				MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k, TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, imstSolverType, mstSolverType,
				innerMstSolverType, graph, adversarialScenarioSet, k,
				tabuPeriod, numberOfPathIterations, numberOfIterations) {
}

TabuSearch::TabuSearch(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
		GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
		IncrementalParam k) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, imstSolverType, mstSolverType,
				innerMstSolverType, graph, adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
		TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, imstSolverType, mstSolverType,
				MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k,
				tabuPeriod, numberOfPathIterations, numberOfIterations) {
}

TabuSearch::TabuSearch(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, imstSolverType, mstSolverType,
				MSTSolverEnum::DEFAULT, graph, adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(MSTSolverEnum mstSolverType,
		MSTSolverEnum innerMstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
		TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				mstSolverType, innerMstSolverType, graph,
				adversarialScenarioSet, k, tabuPeriod, numberOfPathIterations,
				numberOfIterations) {
}

TabuSearch::TabuSearch(MSTSolverEnum mstSolverType,
		MSTSolverEnum innerMstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				mstSolverType, innerMstSolverType, graph,
				adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
		TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				mstSolverType, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k, tabuPeriod, numberOfPathIterations,
				numberOfIterations) {
}

TabuSearch::TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				mstSolverType, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

TabuSearch::TabuSearch(GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
		TabuIterationCount tabuPeriod,
		TabuIterationCount numberOfPathIterations,
		TabuIterationCount numberOfIterations) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				MSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k, tabuPeriod, numberOfPathIterations,
				numberOfIterations) {
}

TabuSearch::TabuSearch(GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		TabuSearch(AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT,
				MSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT, graph,
				adversarialScenarioSet, k,
				TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
				TabuSearchUtils::PATH_ITER_NUM_DEFAULT,
				TabuSearchUtils::ITER_NUM_DEFAULT) {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

