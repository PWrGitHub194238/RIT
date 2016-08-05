/*
 * BinarySearch_v2.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../include/imstsolver/BinarySearch_v2.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "../../include/enums/Connectivity.hpp"
#include "../../include/enums/EdgeConnectionType.hpp"
#include "../../include/enums/GraphConstructMode.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/mstsolver/MSTSolverIF.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphInclude.hpp"
#include "../../include/utils/EdgeSetUtils.hpp"
#include "../../include/utils/GraphEdgeCostUtils.hpp"
#include "../../include/utils/GraphUtils.hpp"
#include "../../include/utils/MathUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("imstsolver.BinarySearch_v2"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

GraphIF* BinarySearch_v2::shrinkEdgeSet(EdgeSetIF* unboundedMSTSolution) {
	INFO(logger, LogBundleKey::BS_V2_SHRINK_EDGE_SET, baseMSTSolution->size(),
			LogStringUtils::edgeSetVisualization(baseMSTSolution, "\t").c_str(),
			unboundedMSTSolution->size(),
			LogStringUtils::edgeSetVisualization(unboundedMSTSolution, "\t").c_str());

	EdgeSetIF* baseUnionUnboundedEdgeSet = EdgeSetUtils::getSetUnion(
			baseMSTSolution, unboundedMSTSolution);
	GraphIF* shrunkenGraph = new GraphImpl { graph->getNumberOfVertices(
			Visibility::VISIBLE), baseUnionUnboundedEdgeSet->size(),
			GraphConstructMode::RESERVE_SPACE_ONLY };
	IteratorId vertexIt = graph->getVertexIteratorId();
	IteratorId edgeIt = graph->getEdgeIteratorId();

	connectivityList = graph->storeEdgeConnectivity(edgeIt);
	graph->disconnectAllEdges(edgeIt);

	graph->beginVertex(vertexIt);
	while (graph->hasNextVertex(vertexIt, Visibility::VISIBLE)) {
		shrunkenGraph->addVertex(graph->nextVertex(vertexIt));
	}

	graph->removeVertexIterator(vertexIt);

	baseUnionUnboundedEdgeSet->begin();
	while (baseUnionUnboundedEdgeSet->hasNext()) {
		baseUnionUnboundedEdgeSet->current()->connect(
				EdgeConnectionType::UNDIRECTED);
		shrunkenGraph->addEdge(baseUnionUnboundedEdgeSet->next());
	}

	MemoryUtils::removeCollection(baseUnionUnboundedEdgeSet, false);

	INFO(logger, LogBundleKey::BS_V2_SHRUNKEN_EDGE_SET,
			shrunkenGraph->getNumberOfVertices(Visibility::VISIBLE),
			shrunkenGraph->getNumberOfEdges(Connectivity::CONNECTED),
			LogStringUtils::edgeSetVisualization(shrunkenGraph,
					Connectivity::CONNECTED, Visibility::VISIBLE, "\t").c_str());
	return shrunkenGraph;
}

void BinarySearch_v2::edgeCostsPreConfiguration() {
	EdgeIdx edgeIdx { };
	EdgeIF* edge { };
	INFO(logger, LogBundleKey::BS_V2_EDGE_COST_PREPROCESSING,
			shrunkenGraph->getNumberOfEdges(Visibility::VISIBLE),
			"c'_{e_{i}} = c_{e_{i}} + (mi^2 + i)/(m + 1)^3");
	edgeIdx = 1;
	shrunkenGraph->beginEdge();
	while (shrunkenGraph->hasNextEdge(Visibility::VISIBLE)) {
		edge = shrunkenGraph->nextEdge();
		INFO(logger, LogBundleKey::BS_V2_EDGE_COST_TEMP_CHANGE,
				LogStringUtils::edgeCostChanged(edge,
						edgeCostPerturbFunction(graph->getNumberOfEdges(),
								edgeIdx, edge->getEdgeCost()), "\t").c_str());
		edge->setEdgeCost(
				edgeCostPerturbFunction(shrunkenGraph->getNumberOfEdges(),
						edgeIdx, edge->getEdgeCost()));
		edgeIdx += 1;
	}

	baseMSTSolution->begin();
	while (baseMSTSolution->hasNext()) {
		baseMSTEdgeCosts->push_back(baseMSTSolution->next()->getEdgeCost());
	}
}

EdgeCost BinarySearch_v2::edgeCostPerturbFunction(EdgeCount const numberOfEdges,
		EdgeIdx const edgeIdx, EdgeCost const edgeCost) {
	return edgeCost
			+ (EdgeCost) (numberOfEdges * (edgeIdx * edgeIdx) + edgeIdx)
					/ ((numberOfEdges + 1) * (numberOfEdges + 1)
							* (numberOfEdges + 1));
}

void BinarySearch_v2::generateLambdaHelperSets(
		EdgeSetIF* const unboundedMSTSolution) {
	EdgeIdx i { };
	TRACE_NOARG(logger, LogBundleKey::BS_V2_GEN_LAMBDA_SET);

	EdgeSetIF* baseCompUnboundedSet = EdgeSetUtils::getSetComplement(
			baseMSTSolution, unboundedMSTSolution);
	EdgeSetIF* unboundedCompBaseSet = EdgeSetUtils::getSetComplement(
			unboundedMSTSolution, baseMSTSolution);
	EdgeCount baseCompUnboundedSetSize = baseCompUnboundedSet->size();
	this->maxLambdaSetKey = baseCompUnboundedSetSize - 1;

	INFO(logger, LogBundleKey::BS_V2_GEN_LAMBDA_HELP_SETS,
			LogStringUtils::edgeSetVisualization(baseMSTSolution, "\t").c_str(),
			baseMSTSolution->size(),
			LogStringUtils::edgeSetVisualization(unboundedMSTSolution, "\t").c_str(),
			unboundedMSTSolution->size(), baseCompUnboundedSet->size(),
			LogStringUtils::edgeSetVisualization(baseCompUnboundedSet, "\t\t").c_str(),
			unboundedCompBaseSet->size(),
			LogStringUtils::edgeSetVisualization(unboundedCompBaseSet, "\t\t").c_str());

	lambdaIncBaseMSTEdgeCostSet = new EdgeCost[baseCompUnboundedSetSize] { };

	TRACE_NOARG(logger, LogBundleKey::BS_V2_GEN_LAMBDA_INC_SET);

	i = 0;
	baseCompUnboundedSet->begin();
	while (baseCompUnboundedSet->hasNext()) {
		lambdaIncBaseMSTEdgeCostSet[i++] =
				baseCompUnboundedSet->next()->getEdgeCost();
	}

	TRACE_NOARG(logger, LogBundleKey::BS_V2_SORT_LAMBDA_INC_SET);

	std::sort(lambdaIncBaseMSTEdgeCostSet,
			lambdaIncBaseMSTEdgeCostSet + baseCompUnboundedSetSize,
			[](EdgeCost l, EdgeCost r) {return l<r;});

	MemoryUtils::removeCollection(baseCompUnboundedSet, false);

	lambdaDecUnboundedMSTEdgeCostTmp =
			new EdgeCost[baseCompUnboundedSetSize] { };

	TRACE_NOARG(logger, LogBundleKey::BS_V2_GEN_LAMBDA_DEC_SET);

	i = 0;
	unboundedCompBaseSet->begin();
	while (unboundedCompBaseSet->hasNext()) {
		lambdaDecUnboundedMSTEdgeCostTmp[i++] =
				unboundedCompBaseSet->next()->getEdgeCost();
	}

	TRACE_NOARG(logger, LogBundleKey::BS_V2_SORT_LAMBDA_DEC_SET);

	std::sort(lambdaDecUnboundedMSTEdgeCostTmp,
			lambdaDecUnboundedMSTEdgeCostTmp + baseCompUnboundedSetSize,
			[](EdgeCost l, EdgeCost r) {return l>r;});

	MemoryUtils::removeCollection(unboundedCompBaseSet, false);
}

void BinarySearch_v2::checkLambdaBounds(EdgeCount k) {
	EdgeSetIF*lowerBoundMST { };
	EdgeSetIF*upperBoundMST { };
	GraphEdgeCostsIF* graphCostBackup { };
	INFO_NOARG(logger, LogBundleKey::BS_V2_LAMBDA_BOUNDS_CHECK);
	if (upperBound != lowerBound) {
		INFO(logger, LogBundleKey::BS_V2_LAMBDA_BOUNDS_NOT_DEFAULT_CHECK,
				lowerBound, upperBound);

		updateGraphEdgeCosts(lowerBound);
		lowerBoundMST = mstSolver->getMST();

		updateGraphEdgeCosts(upperBound);
		upperBoundMST = mstSolver->getMST();
	}

	if ((upperBound == lowerBound)
			|| ((getMSTDiff(lowerBoundMST) >= k)
					!= (getMSTDiff(upperBoundMST) <= k))) {
		INFO(logger,
				((upperBound == lowerBound) ?
						LogBundleKey::BS_V2_LAMBDA_DEFAULT_BOUNDS :
						LogBundleKey::BS_V2_LAMBDA_WRONG_BOUNDS),
				(upperBound == lowerBound) ? getLambda(0, 0) : lowerBound,
				(upperBound == lowerBound) ?
						getLambda(this->maxLambdaSetKey,
								this->maxLambdaSetKey) :
						upperBound, k);
		lowerBound = getLambda(0, 0);
		upperBound = getLambda(this->maxLambdaSetKey, this->maxLambdaSetKey);
	}
}

void BinarySearch_v2::updateGraphEdgeCosts(LambdaValue lamdaParameter) {
	INFO(logger, LogBundleKey::BS_V2_GRAPH_CHANGE_COST_LAMBDA, lamdaParameter);
	baseMSTSolution->begin();
	baseMSTEdgeCosts->begin();
	while (baseMSTSolution->hasNext()) {
		INFO(logger, LogBundleKey::BS_V2_EDGE_COST_TEMP_CHANGE,
				LogStringUtils::edgeCostChanged(baseMSTSolution->current(),
						baseMSTEdgeCosts->current(),
						baseMSTEdgeCosts->current() - lamdaParameter, true).c_str());
		baseMSTSolution->next()->setEdgeCost(
				baseMSTEdgeCosts->next() - (lamdaParameter + MIN_LAMBDA_VALUE));
	}
}

EdgeSetIF* BinarySearch_v2::binarySearchForSolution(EdgeCount k,
		LambdaParamArray lambdaFeasibleParameterSet) {
	LambdaIdx lambdaIdxLow = 0;
	LambdaIdx lambdaIdxHight = lambdaFeasibleParameterSet.size() - 1;
	LambdaIdx lambdaIdx { };
	EdgeSetIF* mstSolution { };
	EdgeCount differentEdges { };
	GraphEdgeCostsIF* graphCostBackup { };

	bool hasFeasibleBackupSolution { false };
	EdgeSetIF* feasibleBackupSolution { };
	EdgeCount feasibleBackupEdgeDiff { 0 };

	std::sort(lambdaFeasibleParameterSet.begin(),
			lambdaFeasibleParameterSet.end());

	INFO(logger, LogBundleKey::BS_V2_MST_BINNARY_SEARCH, lambdaIdxHight + 1);

	while (lambdaIdxLow <= lambdaIdxHight) {
		INFO(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_BOUNDS, lambdaIdxLow,
				lambdaFeasibleParameterSet.at(lambdaIdxLow), lambdaIdxHight,
				lambdaFeasibleParameterSet.at(lambdaIdxHight));

		lambdaIdx = (lambdaIdxLow + lambdaIdxHight) / 2;

		INFO(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_HALF, lambdaIdx,
				lambdaFeasibleParameterSet.at(lambdaIdx));

		updateGraphEdgeCosts(lambdaFeasibleParameterSet.at(lambdaIdx));

		mstSolution = mstSolver->getMST();
		differentEdges = getMSTDiff(mstSolution);

		if (k < differentEdges) {
			INFO(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_TO_BIGGER_LAMBDA,
					lambdaFeasibleParameterSet.at(lambdaIdx), differentEdges, k,
					LogStringUtils::edgeSetVisualization(mstSolution, "\t").c_str(),
					differentEdges,
					LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
							mstSolution, "\t").c_str(),
					mstSolution->getTotalEdgeCost(), lambdaIdx + 1);
			lambdaIdxLow = lambdaIdx + 1;
			if (lambdaIdxLow <= lambdaIdxHight) {
				MemoryUtils::removeCollection(mstSolution, false);
			}
		} else if (k > differentEdges) {
			if (lambdaIdxHight == 0) {
				break;
			}
			if (differentEdges > feasibleBackupEdgeDiff) {
				hasFeasibleBackupSolution = true;
				MemoryUtils::removeCollection(feasibleBackupSolution, false);
				feasibleBackupSolution = new EdgeSetImpl { mstSolution, false };
				feasibleBackupEdgeDiff = differentEdges;
			}
			INFO(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_TO_SMALLER_LAMBDA,
					lambdaFeasibleParameterSet.at(lambdaIdx), differentEdges, k,
					LogStringUtils::edgeSetVisualization(mstSolution, "\t").c_str(),
					differentEdges,
					LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
							mstSolution, "\t").c_str(),
					mstSolution->getTotalEdgeCost(), lambdaIdx - 1);
			lambdaIdxHight = lambdaIdx - 1;
			if (lambdaIdxLow <= lambdaIdxHight) {
				MemoryUtils::removeCollection(mstSolution, false);
			}
		} else {
			INFO(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_SOLUTION,
					lambdaFeasibleParameterSet.at(lambdaIdx),
					mstSolution->getTotalEdgeCost(),
					LogStringUtils::edgeSetVisualization(mstSolution, "\t").c_str(),
					differentEdges,
					LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
							mstSolution, "\t").c_str());
			MemoryUtils::removeCollection(feasibleBackupSolution, false);
			return mstSolution;
		}
	}
	if (hasFeasibleBackupSolution) {
		MemoryUtils::removeCollection(mstSolution, false);
		WARN(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_PART_SOLUTION,
				feasibleBackupSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(feasibleBackupSolution,
						"\t").c_str(), differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						feasibleBackupSolution, "\t").c_str(), k);
		return feasibleBackupSolution;
	} else {
		WARN(logger, LogBundleKey::BS_V2_MST_BIN_SEARCH_PART_SOLUTION,
				mstSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(mstSolution, "\t").c_str(),
				differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						mstSolution, "\t").c_str(), k);
		MemoryUtils::removeCollection(feasibleBackupSolution, false);
		return mstSolution;
	}
}

LambdaValue BinarySearch_v2::getLambda(EdgeIdx i, EdgeIdx j) {
	return lambdaIncBaseMSTEdgeCostSet[i] - lambdaDecUnboundedMSTEdgeCostTmp[j];
}

LambdaValue BinarySearch_v2::findMedianValue(
		LambdaParamArray lambdaFeasibleSeededParameterArray) {
	return MathUtils::median(lambdaFeasibleSeededParameterArray.begin(),
			lambdaFeasibleSeededParameterArray.end());
}

EdgeSetIF* BinarySearch_v2::resolve(IncrementalParam k,
		VertexIF* initialVertex) {
	EdgeSetIF* kboundedMSTSolution { };
	EdgeSetIF* unboundedMSTSolution { };
	if (this->isCostChanged && k > 0) {
		this->isCostChanged = false;
		INFO(logger, LogBundleKey::BS_V2_UNBOUNDED_SOLVE, k);
		unboundedMSTSolution = mstSolver->getMST(initialVertex);

		if (getMSTDiff(unboundedMSTSolution) <= k) {
			INFO(logger, LogBundleKey::BS_V2_UNBOUNDED_OPTIMAL, k,
					unboundedMSTSolution->getTotalEdgeCost(),
					LogStringUtils::edgeSetVisualization(unboundedMSTSolution,
							"\t").c_str(),
					LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
							unboundedMSTSolution, "\t").c_str());

			MemoryUtils::removeCollection(baseMSTSolution, false);
			this->baseMSTSolution = unboundedMSTSolution;
			return new EdgeSetImpl { this->baseMSTSolution, false };
		} else {
			INFO(logger, LogBundleKey::BS_V2_UNBOUNDED_UNACCEPTABLE, k);

			// tylko incjalizacja, ustawianie kosztów w edgeCostsPreConfiguration
			//bazowe koszty oryginalnego MST po zaburzeniach
			this->baseMSTEdgeCosts = new GraphEdgeCostsImpl {
					baseMSTSolution->size(), false };

			this->shrunkenGraph = shrinkEdgeSet(unboundedMSTSolution);
			this->shrunkenGraphBaseCostSet = GraphEdgeCostUtils::getScenario(
					shrunkenGraph);
			mstSolver->setGraph(this->shrunkenGraph);

			edgeCostsPreConfiguration();
			generateLambdaHelperSets(unboundedMSTSolution);
			MemoryUtils::removeCollection(unboundedMSTSolution, false);

			checkLambdaBounds(k);

			kboundedMSTSolution = resolve(k);
			delete[] lambdaIncBaseMSTEdgeCostSet;
			delete[] lambdaDecUnboundedMSTEdgeCostTmp;
			delete this->baseMSTEdgeCosts;

			// przywracanie kosztów grafu i jego pierwotnych rozmiarów
			GraphUtils::changeGraphCosts(shrunkenGraph,
					shrunkenGraphBaseCostSet);
			delete shrunkenGraphBaseCostSet;
			graph->restoreConnectivityAllEdges(connectivityList);
			MemoryUtils::removeGraph(shrunkenGraph, false, false);

			MemoryUtils::removeCollection(baseMSTSolution, false);
			this->baseMSTSolution = kboundedMSTSolution;
			return new EdgeSetImpl { this->baseMSTSolution, false };
		}
	}
	INFO_NOARG(logger, LogBundleKey::BS_V2_NO_CHANGE);
	return new EdgeSetImpl { this->baseMSTSolution, false };
}

EdgeSetIF * BinarySearch_v2::resolve(IncrementalParam k) {
	SEdgeIdx i { };
	SEdgeIdx j { };
	LambdaCount totalLambdaParameterCounter { };
	EdgeIdxArray minJEdgeIdxArray { };
	EdgeIdxArray maxJEdgeIdxArray { };
	LambdaParamArray lambdaFeasibleParameterArray { };

	LambdaValue seedValue { };
	LambdaCount lambdaSeedValue { };
	LambdaValue lambdaCurrentValue { };
	LambdaParamArray lambdaSeedFeasibleParameterArray { };

	EdgeSetIF* newMSTSolution { };
	EdgeCount differentEdges { };

	minJEdgeIdxArray.reserve(this->maxLambdaSetKey + 1);
	maxJEdgeIdxArray.reserve(this->maxLambdaSetKey + 1);

	maxJEdgeIdxArray[0] = maxLambdaSetKey;//żeby uniknąć ifa w 1 itersji przy j = maxJEdgeIdxArray[i];
	minJEdgeIdxArray[this->maxLambdaSetKey] = 0;//żeby uniknąć niepotrzebnej inicjalizacji wszystkich wartości

	INFO_NOARG(logger, LogBundleKey::BS_V2_IMST_BINARY_SEARCH);

	while (lowerBound <= upperBound) {
		INFO(logger, LogBundleKey::BS_V2_IMST_NEW_ITERATION, lowerBound,
				upperBound);

		totalLambdaParameterCounter = 0;

		i = maxLambdaSetKey;
		j = minJEdgeIdxArray[i];

		TRACE(logger, LogBundleKey::BS_V2_IMST_MIN_INDEX_SET_GEN, i, j);

		while (i >= 0 && j <= maxLambdaSetKey) {
			TRACE(logger, LogBundleKey::BS_V2_FIND_MIN_EDGE_IDX_LAMBDA, i,
					lowerBound);
			while (j <= maxLambdaSetKey && lowerBound > getLambda(i, j)) {
				TRACE(logger, LogBundleKey::BS_V2_SEARCHING_MIN_EDGE_IDX_LAMBDA,
						lowerBound, i, j, getLambda(i, j));
				j += 1;
			}

			if (j > maxLambdaSetKey) {
				break;
			}
			TRACE(logger, LogBundleKey::BS_V2_FOUND_MIN_EDGE_IDX_LAMBDA, i,
					lowerBound, j, getLambda(i, j));
			minJEdgeIdxArray[i] = j;
			i -= 1;
		}
		while (i >= 0) {
			TRACE(logger, LogBundleKey::BS_V2_NO_FOUND_MIN_EDGE_IDX_LAMBDA, i,
					lowerBound, j);
			minJEdgeIdxArray[i] = j;
			i -= 1;
		}

		i = 0;
		j = maxJEdgeIdxArray[i];

		TRACE(logger, LogBundleKey::BS_V2_IMST_MAX_INDEX_SET_GEN, i, j);

		while (i <= maxLambdaSetKey && j > 0) {	// jeśli j = 0 to i możemy wstawić już ręcznie, tutaj j może być unsigned
			TRACE(logger, LogBundleKey::BS_V2_FIND_MAX_EDGE_IDX_LAMBDA, i,
				upperBound);
			while (j > 0 && getLambda(i, j) > upperBound) {
				TRACE(logger, LogBundleKey::BS_V2_SEARCHING_MAX_EDGE_IDX_LAMBDA,
					upperBound, i, j, getLambda(i, j));
				j -= 1;
			}

			TRACE(logger, LogBundleKey::BS_V2_FOUND_MAX_EDGE_IDX_LAMBDA, i,
				upperBound, j, getLambda(i, j));

			maxJEdgeIdxArray[i] = j;
			i += 1;
		}
		while (i <= maxLambdaSetKey) { // jeśli U < d (i, 0) to i tak d(i, 0) < d(i+1,0) < ...
			TRACE(logger, LogBundleKey::BS_V2_NO_FOUND_MAX_EDGE_IDX_LAMBDA, i,
				upperBound, j);
			maxJEdgeIdxArray[i] = 0;// ... a jeśli U < d(i, 0) to też L < d(i,0), więc jest ok (między min a max jest co najmniej 1 element
						// więc nawet jeśli min[i] = 0 to jest ok.)
			i += 1;
		}

		TRACE_NOARG(logger, LogBundleKey::BS_V2_COUNT_FEASIBLE_LAMBDA);

		/*	for (i = 0; i <= maxLambdaSetKey; i += 1) {
		 std::cout << "MIN[" << i << "]: " << minJEdgeIdxArray[i] << "\tMAX["
		 << i << "]: " << maxJEdgeIdxArray[j] << std::endl;
		 }*/

		/*for (i = 0; i <= maxLambdaSetKey; i += 1) {
		 for (j = 0; j <= maxLambdaSetKey; j += 1) {
		 std::cout << "L(" << i << "," << j << ") = "
		 << lambdaIncBaseMSTEdgeCostSet[i] << " - "
		 << lambdaDecUnboundedMSTEdgeCostTmp[j] << " = "
		 << getLambda(i, j) << std::endl;
		 }
		 }*/

		for (i = 0; i <= maxLambdaSetKey; i += 1) {
			if (minJEdgeIdxArray[i] <= maxJEdgeIdxArray[i]) {
				totalLambdaParameterCounter += (maxJEdgeIdxArray[i]
						- minJEdgeIdxArray[i] + 1);
				TRACE(logger, LogBundleKey::BS_V2_COUNT_FEASIBLE_LAMBDA_I, i,
						maxJEdgeIdxArray[i] - minJEdgeIdxArray[i] + 1,
						minJEdgeIdxArray[i], maxJEdgeIdxArray[i],
						totalLambdaParameterCounter);
				/*std::cout << maxJEdgeIdxArray[i] - minJEdgeIdxArray[i] + 1
				 << " " << minJEdgeIdxArray[i] << " "
				 << maxJEdgeIdxArray[i] << " "
				 << totalLambdaParameterCounter << std::endl;*/
			}
		}

		if (totalLambdaParameterCounter
				<= TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT
						* maxLambdaSetKey) {

			INFO(logger, LogBundleKey::BS_V2_IMST_BIN_SEARCH_LAMBDA_COUNT_LOW,
					totalLambdaParameterCounter,
					TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT
							* maxLambdaSetKey);

			lambdaFeasibleParameterArray.reserve(totalLambdaParameterCounter);

			for (i = 0; i <= maxLambdaSetKey; i += 1) {
				for (j = minJEdgeIdxArray[i]; j <= maxJEdgeIdxArray[i]; j +=
						1) {
					lambdaCurrentValue = getLambda(i, j);
					TRACE(logger, LogBundleKey::BS_V2_ADD_FEASIBLE_LAMBDA,
							lowerBound, i, j, lambdaCurrentValue, upperBound);
					lambdaFeasibleParameterArray.push_back(lambdaCurrentValue);
				}
			}

			return binarySearchForSolution(k, lambdaFeasibleParameterArray);
		} else {

			INFO(logger, LogBundleKey::BS_V2_IMST_BIN_SEARCH_LAMBDA_COUNT,
					totalLambdaParameterCounter,
					TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT
							* maxLambdaSetKey);

			lambdaSeedValue = floor(
					(double) totalLambdaParameterCounter
							/ (LAMBDA_PARAMETER_SEED_THRESHOLD_MULT
									* maxLambdaSetKey));

			lambdaSeedFeasibleParameterArray.reserve(
					(LambdaCount) ceil(
							totalLambdaParameterCounter * 1.0
									/ lambdaSeedValue));

			TRACE(logger, LogBundleKey::BS_V2_GEN_SEED_FEASIBLE_LAMBDA,
					lambdaSeedValue,
					(LambdaCount ) ceil(
							totalLambdaParameterCounter * 1.0
									/ lambdaSeedValue));

			for (i = 0; i <= this->maxLambdaSetKey; i += 1) {
				seedValue = maxJEdgeIdxArray[i];
				for (j = minJEdgeIdxArray[i] + lambdaSeedValue - 1;
						j < seedValue; j += lambdaSeedValue) {
					lambdaCurrentValue = getLambda(i, j);
					TRACE(logger, LogBundleKey::BS_V2_ADD_SEED_FEASIBLE_LAMBDA,
							lowerBound, i, j, lambdaCurrentValue, upperBound);
					lambdaSeedFeasibleParameterArray.push_back(
							lambdaCurrentValue);
				}
			}

			INFO(logger, LogBundleKey::BS_V2_IMST_BIN_SEARCH_MEDIAN,
					lambdaSeedFeasibleParameterArray.size());

			lambdaCurrentValue = findMedianValue(
					lambdaSeedFeasibleParameterArray);

			updateGraphEdgeCosts(lambdaCurrentValue);

			INFO(logger, LogBundleKey::BS_V2_IMST_MEDIAN_BASED_NEW_MST,
					lambdaCurrentValue, lambdaSeedFeasibleParameterArray.size());

			newMSTSolution = mstSolver->getMST();
			differentEdges = getMSTDiff(newMSTSolution);

			if (differentEdges == k) {
				INFO(logger, LogBundleKey::BS_V2_IMST_BIN_SEARCH_SOLUTION,
						lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
						LogStringUtils::edgeSetVisualization(newMSTSolution,
								"\t").c_str(), k,
						LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
								newMSTSolution, "\t").c_str());
				return newMSTSolution;
			} else if (k < differentEdges) { // jeśli innych krawędzi jest za dużo to trzeba zwiększyć LP lambdy
				INFO(logger,
						LogBundleKey::BS_V2_IMST_BIN_SEARCH_TO_BIGGER_LAMBDA,
						lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
						LogStringUtils::edgeSetVisualization(newMSTSolution,
								"\t").c_str(), differentEdges,
						LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
								newMSTSolution, "\t").c_str(), k, lowerBound,
						lambdaCurrentValue);
				lowerBound = lambdaCurrentValue;
			} else { // jeśli innych krawędzi jest za mało to trzeba zmniejszyć UP lambdy
				INFO(logger,
						LogBundleKey::BS_V2_IMST_BIN_SEARCH_TO_LOWER_LAMBDA,
						lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
						LogStringUtils::edgeSetVisualization(newMSTSolution,
								"\t").c_str(), differentEdges,
						LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
								newMSTSolution, "\t").c_str(), k, upperBound,
						lambdaCurrentValue);
				upperBound = lambdaCurrentValue;
			}
			MemoryUtils::removeCollection(newMSTSolution, false);
		}
	}
	WARN_NOARG(logger, LogBundleKey::BS_V2_PART_SOLUTION);
	return new EdgeSetImpl { this->baseMSTSolution, false };
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, EdgeSetIF* baseSolution, VertexIF* initialVertex,
		LambdaValue lowerBound, LambdaValue upperBound) :
		IMSTSolverIF(mstSolverType, graph, baseSolution, initialVertex,
				lowerBound, upperBound) {
// Jeśli okaże się, że nieograniczone rozwiązanie nie może być, wtedy dopiero zapiszemy koszty oryginalnego mst,
// by móc modyfokować je lambdami
	this->baseMSTEdgeCosts = nullptr;
	this->shrunkenGraph = nullptr;
	this->shrunkenGraphBaseCostSet = nullptr;
	this->lambdaIncBaseMSTEdgeCostSet = nullptr;
	this->lambdaDecUnboundedMSTEdgeCostTmp = nullptr;
// ustawiane w funkcji, która faktycznie generuje zbiór lambd: generateLambdaSet
	this->maxLambdaSetKey = 0;
}

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, VertexIF* initialVertex, LambdaValue lowerBound,
		LambdaValue upperBound) :
		BinarySearch_v2(mstSolverType, graph, nullptr, initialVertex,
				lowerBound, upperBound) {
}

BinarySearch_v2::BinarySearch_v2(GraphIF* const graph, EdgeSetIF* baseSolution,
		VertexIF* initialVertex, LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v2(MSTSolverEnum::DEFAULT, graph, baseSolution,
				initialVertex, lowerBound, upperBound) {
}

BinarySearch_v2::BinarySearch_v2(GraphIF* const graph, VertexIF* initialVertex,
		LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v2(MSTSolverEnum::DEFAULT, graph, nullptr, initialVertex,
				lowerBound, upperBound) {
}

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, VertexIF* initialVertex) :
		BinarySearch_v2(mstSolverType, graph, nullptr, initialVertex, 0, 0) {
}

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v2(mstSolverType, graph, nullptr, nullptr, lowerBound,
				upperBound) {
}

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, EdgeSetIF* const baseSolution) :
		BinarySearch_v2(mstSolverType, graph, baseSolution, nullptr, 0, 0) {
}

BinarySearch_v2::BinarySearch_v2(MSTSolverEnum const mstSolverType,
		GraphIF* const graph) :
		BinarySearch_v2(mstSolverType, graph, nullptr, nullptr, 0, 0) {
}

BinarySearch_v2::BinarySearch_v2(GraphIF* const graph, EdgeSetIF* baseSolution) :
		BinarySearch_v2(MSTSolverEnum::DEFAULT, graph, baseSolution, nullptr, 0,
				0) {
}

BinarySearch_v2::BinarySearch_v2(GraphIF* const graph) :
		BinarySearch_v2(MSTSolverEnum::DEFAULT, graph, nullptr, nullptr, 0, 0) {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

