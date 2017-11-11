/*
 * BinarySearch_v1.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../include/imstsolver/BinarySearch_v1.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <algorithm>
#include <cmath>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../../include/enums/Connectivity.hpp"
#include "../../include/enums/EdgeConnectionType.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/mstsolver/MSTSolverIF.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/utils/EdgeSetUtils.hpp"
#include "../../include/utils/GraphUtils.hpp"
#include "../../include/utils/MathUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("imstsolver.BinarySearch_v1"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//std::map<int, std::map<int, double>> tmpLambdaSet;

void BinarySearch_v1::shrinkEdgeSet(EdgeSetIF* unboundedMSTSolution) {
	connectivityList = graph->storeEdgeConnectivity();
	INFO(logger, LogBundleKey::BS_V1_SHRINK_EDGE_SET, baseMSTSolution->size(),
			LogStringUtils::edgeSetVisualization(baseMSTSolution, "\t").c_str(),
			unboundedMSTSolution->size(),
			LogStringUtils::edgeSetVisualization(unboundedMSTSolution, "\t").c_str());
	graph->disconnectAllEdges();

	baseMSTSolution->begin();
	while (baseMSTSolution->hasNext()) {
		baseMSTSolution->next()->connect(EdgeConnectionType::UNDIRECTED);
	}
	unboundedMSTSolution->begin();
	while (unboundedMSTSolution->hasNext()) {
		unboundedMSTSolution->next()->connect(EdgeConnectionType::UNDIRECTED);
	}
	INFO(logger, LogBundleKey::BS_V1_SHRUNKEN_EDGE_SET,
			graph->getNumberOfEdges(Connectivity::CONNECTED),
			LogStringUtils::edgeSetVisualization(graph, Connectivity::CONNECTED,
					Visibility::VISIBLE, "\t").c_str());
}

void BinarySearch_v1::edgeCostsPreConfiguration() {
	EdgeIdx edgeIdx { };
	EdgeIF* edge { };
	graph->beginEdge();
	INFO(logger, LogBundleKey::BS_V1_EDGE_COST_PREPROCESSING,
			"c'_{e_{i}} = c_{e_{i}} + (mi^2 + i)/(m + 1)^3");
	edgeIdx = 1;
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edge = graph->nextEdge();
		INFO(logger, LogBundleKey::BS_V1_EDGE_COST_TEMP_CHANGE,
				LogStringUtils::edgeCostChanged(edge,
						edgeCostPerturbFunction(graph->getNumberOfEdges(),
								edgeIdx, edge->getEdgeCost()), "\t").c_str());
		edge->setEdgeCost(
				edgeCostPerturbFunction(graph->getNumberOfEdges(), edgeIdx,
						edge->getEdgeCost()));
		edgeIdx += 1;
	}

	baseMSTSolution->begin();
	while (baseMSTSolution->hasNext()) {
		baseMSTEdgeCosts->push_back(baseMSTSolution->next()->getEdgeCost());
	}
}

EdgeCost BinarySearch_v1::edgeCostPerturbFunction(EdgeCount const numberOfEdges,
		EdgeIdx const edgeIdx, EdgeCost const edgeCost) {
	return edgeCost
			+ (EdgeCost) (numberOfEdges * (edgeIdx * edgeIdx) + edgeIdx)
					/ ((numberOfEdges + 1) * (numberOfEdges + 1)
							* (numberOfEdges + 1));
}

void BinarySearch_v1::generateLambdaSet(EdgeSetIF* const unboundedMSTSolution) {
	EdgeSetIF* baseCompUnboundedSet = EdgeSetUtils::getSetComplement(
			baseMSTSolution, unboundedMSTSolution);
	EdgeSetIF* unboundedCompBaseSet = EdgeSetUtils::getSetComplement(
			unboundedMSTSolution, baseMSTSolution);

	/*int c = 0;
	 baseCompUnboundedSet->begin();
	 while(baseCompUnboundedSet->hasNext()) {
	 EdgeIF* e1 = baseCompUnboundedSet->next();
	 tmpLambdaSet.insert(std::make_pair(e1->getEdgeIdx(), std::map<int, double> { }));
	 unboundedCompBaseSet->begin();
	 while(unboundedCompBaseSet->hasNext()) {
	 EdgeIF* e2 = unboundedCompBaseSet->next();
	 tmpLambdaSet.at(e1->getEdgeIdx()).insert(std::make_pair(e2->getEdgeIdx(), e1->getEdgeCost() - e2->getEdgeCost()));
	 c += 1;
	 }
	 }*/

	GraphEdgeCostsImpl baseMSTEdgeCostTmp(baseCompUnboundedSet);
	GraphEdgeCostsImpl unboundedMSTEdgeCostTmp(unboundedCompBaseSet);
	EdgeIdx i { }, j { };
	this->maxLambdaSetKey = baseCompUnboundedSet->size() - 1;
	//std::cout << this->maxLambdaSetKey << " " << c << std::endl;
	baseMSTEdgeCostTmp.sortInc();
	unboundedMSTEdgeCostTmp.sortDec();

	//TODO przyśpieszenie normalnie tego nie liczymy, będziemy obliczać d(i,j) na bierząco

	TRACE_NOARG(logger, LogBundleKey::BS_V1_GEN_LAMBDA_SET);

	for (i = 0; i <= maxLambdaSetKey; i += 1) {
		lambdaSet.insert(std::make_pair(i, LambdaParamMap { }));
	}

	for (i = 0; i <= maxLambdaSetKey; i += 1) {
		for (j = 0; j <= maxLambdaSetKey; j += 1) {
			lambdaSet.at(i).insert(
					std::make_pair(j,
							baseMSTEdgeCostTmp.at(i)
									- unboundedMSTEdgeCostTmp.at(j)));
			TRACE(logger, LogBundleKey::BS_V1_ADD_LAMBDA_PARAM_TO_SET, i, j,
					baseMSTEdgeCostTmp.at(i), unboundedMSTEdgeCostTmp.at(j),
					(LambdaValue ) baseMSTEdgeCostTmp.at(i)
							- unboundedMSTEdgeCostTmp.at(j));
		}
	}
	MemoryUtils::removeCollection(unboundedCompBaseSet, false);
	MemoryUtils::removeCollection(baseCompUnboundedSet, false);
}

void BinarySearch_v1::checkLambdaBounds(EdgeCount k) {
	EdgeSetIF*lowerBoundMST { };
	EdgeSetIF*upperBoundMST { };
	GraphEdgeCostsIF* graphCostBackup { };
	INFO_NOARG(logger, LogBundleKey::BS_V1_LAMBDA_BOUNDS_CHECK);
	if (upperBound != lowerBound) {
		INFO(logger, LogBundleKey::BS_V1_LAMBDA_BOUNDS_NOT_DEFAULT_CHECK,
				lowerBound, upperBound);

		graphCostBackup = tempUpdateGraphEdgeCosts(lowerBound);
		lowerBoundMST = mstSolver->getSolution();
		GraphUtils::changeGraphCosts(graph, graphCostBackup);
		delete graphCostBackup;

		graphCostBackup = tempUpdateGraphEdgeCosts(upperBound);
		upperBoundMST = mstSolver->getSolution();
		GraphUtils::changeGraphCosts(graph, graphCostBackup);
		delete graphCostBackup;
	}

	if ((upperBound == lowerBound)
			|| ((getMSTDiff(lowerBoundMST) >= k)
					!= (getMSTDiff(upperBoundMST) <= k))) {
		INFO(logger,
				((upperBound == lowerBound) ?
						LogBundleKey::BS_V1_LAMBDA_DEFAULT_BOUNDS :
						LogBundleKey::BS_V1_LAMBDA_WRONG_BOUNDS),
				(upperBound == lowerBound) ? lambdaSet.at(0).at(0) : lowerBound,
				(upperBound == lowerBound) ?
						lambdaSet.at(this->maxLambdaSetKey).at(
								this->maxLambdaSetKey) :
						upperBound, k);
		lowerBound = lambdaSet.at(0).at(0);
		upperBound = lambdaSet.at(this->maxLambdaSetKey).at(
				this->maxLambdaSetKey);
	}
}

//TODO można by dodać do getMST funkcję, która bierze koszty, je zamienia, przywraca i opcjonalnie usuwa
//TODO można by też pamiętać poprzedni parametr lambda i różnicowo, już bez zapisywania kosztów
GraphEdgeCostsIF* BinarySearch_v1::tempUpdateGraphEdgeCosts(
		LambdaValue lamdaParameter) {
	EdgeIF* edge { };
	EdgeCost c { };
	baseMSTSolution->begin();
	baseMSTEdgeCosts->begin();

	// TODO tylko koszty baseMSTSolution
	GraphEdgeCostsIF* costBackup = new GraphEdgeCostsImpl { graph };

	INFO(logger, LogBundleKey::BS_V1_GRAPH_CHANGE_COST_LAMBDA, lamdaParameter);
	while (baseMSTSolution->hasNext()) {
		INFO(logger, LogBundleKey::BS_V1_EDGE_COST_TEMP_CHANGE,
				LogStringUtils::edgeCostChanged(baseMSTSolution->current(),
						baseMSTEdgeCosts->current() - lamdaParameter, true).c_str());
		baseMSTSolution->next()->setEdgeCost(
				baseMSTEdgeCosts->next() - (lamdaParameter + MIN_LAMBDA_VALUE));
	}
	return costBackup;
}

EdgeIdx BinarySearch_v1::findMinEdgeIdxForLambdaParam(
		EdgeIdx const baseMSTEdgeIdx) {
	EdgeIdx jEdgeIdx { 0 };
	LambdaParamMap lambdaMap = lambdaSet.at(baseMSTEdgeIdx);
	TRACE(logger, LogBundleKey::BS_V1_FIND_MIN_EDGE_IDX_LAMBDA, baseMSTEdgeIdx,
			lowerBound);
	while (lambdaMap.at(jEdgeIdx) < lowerBound
			&& this->maxLambdaSetKey != jEdgeIdx) {
		TRACE(logger, LogBundleKey::BS_V1_SEARCHING_MIN_EDGE_IDX_LAMBDA,
				lowerBound, baseMSTEdgeIdx, jEdgeIdx, lambdaMap.at(jEdgeIdx));
		jEdgeIdx += 1;
	}

	TRACE(logger,
			(lambdaMap.at(jEdgeIdx) < lowerBound) ?
					LogBundleKey::BS_V1_NO_FOUND_MIN_EDGE_IDX_LAMBDA :
					LogBundleKey::BS_V1_FOUND_MIN_EDGE_IDX_LAMBDA,
			baseMSTEdgeIdx, lowerBound,
			lambdaMap.at(jEdgeIdx) < lowerBound ? jEdgeIdx + 1 : jEdgeIdx);
	return lambdaMap.at(jEdgeIdx) < lowerBound ? jEdgeIdx + 1 : jEdgeIdx;
}

EdgeIdx BinarySearch_v1::findMaxEdgeidxForLambdaParam(
		EdgeIdx const baseMSTEdgeIdx) {
	EdgeIdx jEdgeIdx { this->maxLambdaSetKey + 1 };
	LambdaParamMap lambdaMap = lambdaSet.at(baseMSTEdgeIdx);
	TRACE(logger, LogBundleKey::BS_V1_FIND_MAX_EDGE_IDX_LAMBDA, baseMSTEdgeIdx,
			upperBound);
	do {
		jEdgeIdx -= 1;
		TRACE(logger, LogBundleKey::BS_V1_SEARCHING_MAX_EDGE_IDX_LAMBDA,
				lambdaMap.at(jEdgeIdx), baseMSTEdgeIdx, jEdgeIdx, upperBound);
	} while (lambdaMap.at(jEdgeIdx) > upperBound && jEdgeIdx > 0);

	TRACE(logger,
			(lambdaMap.at(jEdgeIdx) > upperBound) ?
					LogBundleKey::BS_V1_NO_FOUND_MAX_EDGE_IDX_LAMBDA :
					LogBundleKey::BS_V1_FOUND_MAX_EDGE_IDX_LAMBDA,
			baseMSTEdgeIdx, upperBound, jEdgeIdx);
	return jEdgeIdx;
}

EdgeSetIF* BinarySearch_v1::binarySearchForSolution(EdgeCount k,
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

	INFO(logger, LogBundleKey::BS_V1_MST_BINNARY_SEARCH, lambdaIdxHight + 1);

	while (lambdaIdxLow <= lambdaIdxHight) {
		INFO(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_BOUNDS, lambdaIdxLow,
				lambdaFeasibleParameterSet.at(lambdaIdxLow), lambdaIdxHight,
				lambdaFeasibleParameterSet.at(lambdaIdxHight));

		lambdaIdx = (lambdaIdxLow + lambdaIdxHight) / 2;

		INFO(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_HALF, lambdaIdx,
				lambdaFeasibleParameterSet.at(lambdaIdx));

		graphCostBackup = tempUpdateGraphEdgeCosts(
				lambdaFeasibleParameterSet.at(lambdaIdx));

		mstSolution = mstSolver->getSolution();
		differentEdges = getMSTDiff(mstSolution);
		GraphUtils::changeGraphCosts(graph, graphCostBackup);
		delete graphCostBackup;

		if (k < differentEdges) {
			INFO(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_TO_BIGGER_LAMBDA,
					lambdaFeasibleParameterSet.at(lambdaIdx), differentEdges, k,
					lambdaIdx + 1);
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
			INFO(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_TO_SMALLER_LAMBDA,
					lambdaFeasibleParameterSet.at(lambdaIdx), differentEdges, k,
					lambdaIdx - 1);
			lambdaIdxHight = lambdaIdx - 1;
			if (lambdaIdxLow <= lambdaIdxHight) {
				MemoryUtils::removeCollection(mstSolution, false);
			}
		} else {
			INFO(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_SOLUTION,
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
		WARN(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_PART_SOLUTION,
				feasibleBackupSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(feasibleBackupSolution,
						"\t").c_str(), differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						feasibleBackupSolution, "\t").c_str(), k);
		return feasibleBackupSolution;
	} else {
		WARN(logger, LogBundleKey::BS_V1_MST_BIN_SEARCH_PART_SOLUTION,
				mstSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(mstSolution, "\t").c_str(),
				differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						mstSolution, "\t").c_str(), k);
		MemoryUtils::removeCollection(feasibleBackupSolution, false);
		return mstSolution;
	}

}

LambdaValue BinarySearch_v1::findMedianValue(
		LambdaParamArray lambdaFeasibleSeededParameterArray) {
	return MathUtils::median(lambdaFeasibleSeededParameterArray.begin(),
			lambdaFeasibleSeededParameterArray.end());
}

EdgeSetIF* BinarySearch_v1::resolve(IncrementalParam k,
		VertexIF* initialVertex) {
	EdgeSetIF* kboundedMSTSolution { };
	EdgeSetIF* unboundedMSTSolution { };
	if (this->isCostChanged && k > 0) {
		this->isCostChanged = false;
		INFO(logger, LogBundleKey::BS_V1_UNBOUNDED_SOLVE, k);
		unboundedMSTSolution = mstSolver->getSolution(initialVertex);

		if (getMSTDiff(unboundedMSTSolution) <= k) {
			INFO(logger, LogBundleKey::BS_V1_UNBOUNDED_OPTIMAL, k,
					unboundedMSTSolution->getTotalEdgeCost(),
					LogStringUtils::edgeSetVisualization(unboundedMSTSolution,
							"\t").c_str(),
					LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
							unboundedMSTSolution, "\t").c_str());

			MemoryUtils::removeCollection(baseMSTSolution, false);
			this->baseMSTSolution = unboundedMSTSolution;
			return new EdgeSetImpl { this->baseMSTSolution, false };
		} else {
			INFO(logger, LogBundleKey::BS_V1_UNBOUNDED_UNACCEPTABLE, k);

			// tylko incjalizacja, ustawianie kosztów w edgeCostsPreConfiguration
			this->baseMSTEdgeCosts = new GraphEdgeCostsImpl {
					baseMSTSolution->size(), false };
			shrinkEdgeSet(unboundedMSTSolution);
			edgeCostsPreConfiguration();
			generateLambdaSet(unboundedMSTSolution);
			MemoryUtils::removeCollection(unboundedMSTSolution, false);
			checkLambdaBounds(k);

			/*	std::cout << "SHRINK SET:\n\n\n" << std::endl;

			 int a = 0, b = 0;

			 std::cout << "\\begin{tabu}{" << std::flush;
			 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
			 std::cout << "|c" << std::flush;
			 }
			 std::cout << "|}" << std::endl;

			 std::cout << "\t\\hline" << std::endl;

			 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
			 std::cout << "\t" << std::flush;
			 for (int j = 0; j < graph->getNumberOfEdges(); j += 1) {
			 if (j + 1 < graph->getNumberOfEdges()) {
			 std::cout << "& " << std::flush;
			 }
			 if (tmpLambdaSet.count(i) > 0 && tmpLambdaSet.at(i).count(j) > 0) {
			 a += 1;
			 if (lowerBound <= tmpLambdaSet.at(i).at(j) && tmpLambdaSet.at(i).at(j) <= upperBound) {
			 b += 1;
			 std::cout << "\\cellcolor{black}\t" << std::flush;
			 } else {
			 std::cout << "\t" << std::flush;
			 }
			 }
			 }
			 std::cout << "\\\\\\hline" << std::endl;
			 }
			 std::cout << "\\end{tabu}%" << std::endl;
			 std::cout << a << " " << b << std::endl;
			 */

			kboundedMSTSolution = resolve(k);
			delete this->baseMSTEdgeCosts;
			restoreBaseEdgeCosts();
			graph->restoreConnectivityAllEdges(connectivityList);

			MemoryUtils::removeCollection(baseMSTSolution, false);
			this->baseMSTSolution = kboundedMSTSolution;
			return new EdgeSetImpl { this->baseMSTSolution, false };
		}
	}
	INFO_NOARG(logger, LogBundleKey::BS_V1_NO_CHANGE);
	return new EdgeSetImpl { this->baseMSTSolution, false };
}

EdgeSetIF * BinarySearch_v1::resolve(IncrementalParam k) {
	EdgeIdx i { };
	EdgeIdx j { };
	EdgeIdx l { };
	LambdaCount totalLambdaParameterCounter { 0 };
	LambdaParamArray minJEdgeIdxArray { };
	LambdaParamArray maxJEdgeIdxArray { };
	LambdaParamArray countArray; //TODO można to usunąć

	LambdaParamArray lambdaFeasibleParameterArray { };

	//std::cout << "NEXT ITERATION\n\n\n\n" << std::endl;

	LambdaValue lambdaCurrentValue { };

	LambdaValue seedValue { };
	LambdaCount lambdaSeedValue { };
	LambdaParamPairMap lambdaSeedArray;	//TODO baaaardzo duży overkill, po co sumować oddzielnie H(i) jak potem i tak je zaraz sumujemy

	EdgeSetIF* newMSTSolution { };
	EdgeCount differentEdges { };
	GraphEdgeCostsIF* graphCostBackup { };

	minJEdgeIdxArray.reserve(this->maxLambdaSetKey + 1);
	maxJEdgeIdxArray.reserve(this->maxLambdaSetKey + 1);
	countArray.reserve(this->maxLambdaSetKey + 1);

	INFO_NOARG(logger, LogBundleKey::BS_V1_IMST_BINARY_SEARCH);

	for (i = 0; i <= this->maxLambdaSetKey; i += 1) {
		TRACE(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_GEN_LAMBDA_SET, i);
		minJEdgeIdxArray[i] = findMinEdgeIdxForLambdaParam(i);
		maxJEdgeIdxArray[i] = findMaxEdgeidxForLambdaParam(i);
		countArray[i] =
				(minJEdgeIdxArray[i] <= maxJEdgeIdxArray[i]) ?
						maxJEdgeIdxArray[i] - minJEdgeIdxArray[i] + 1 : 0;
	}

	for (i = 0; i < this->maxLambdaSetKey + 1; i += 1) {
		totalLambdaParameterCounter += countArray[i];
	}

	if (totalLambdaParameterCounter
			<= TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT
					* maxLambdaSetKey) {

		INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_LAMBDA_COUNT_LOW,
				totalLambdaParameterCounter,
				TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT * maxLambdaSetKey);

		LambdaParamPairMap::const_iterator itEnd = lambdaSet.end();	//TODO Do usunięcia, a obliczania za pomocą minJEdgeIdxArray i maxJEdgeIdxArray
		for (LambdaParamPairMap::const_iterator itBegin = lambdaSet.begin();
				itBegin != itEnd; ++itBegin) {
			LambdaParamMap::const_iterator ittEnd = itBegin->second.end();
			for (LambdaParamMap::const_iterator ittBegin =
					itBegin->second.begin(); ittBegin != ittEnd; ++ittBegin) {
				lambdaCurrentValue = ittBegin->second;
				if (lowerBound <= lambdaCurrentValue
						&& lambdaCurrentValue <= upperBound) {
					lambdaFeasibleParameterArray.push_back(lambdaCurrentValue);
				}
			}
		}

		return binarySearchForSolution(k, lambdaFeasibleParameterArray);
	}

	INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_LAMBDA_COUNT,
			totalLambdaParameterCounter,
			TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT * maxLambdaSetKey);

	lambdaSeedValue = floor(
			(double) totalLambdaParameterCounter
					/ (LAMBDA_PARAMETER_SEED_THRESHOLD_MULT * maxLambdaSetKey));

	/*int a = 0, b = 0;

	 std::cout << "\\begin{tabu}{" << std::flush;
	 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
	 std::cout << "|c" << std::flush;
	 }
	 std::cout << "|}" << std::endl;

	 std::cout << "\t\\hline" << std::endl;

	 for (int i = 0; i < this->maxLambdaSetKey + 1; i += 1) {
	 std::cout << "\t" << std::flush;
	 for (int j = 0; j < this->maxLambdaSetKey + 1; j += 1) {
	 if (j + 1 < graph->getNumberOfEdges()) {
	 std::cout << "& " << std::flush;
	 }
	 if (lambdaSet.count(i) > 0 && lambdaSet.at(i).count(j) > 0) {
	 a += 1;
	 if (lowerBound <= lambdaSet.at(i).at(j)
	 && lambdaSet.at(i).at(j) <= upperBound) {
	 b += 1;
	 std::cout << "\\cellcolor{black}\t" << std::flush;
	 } else {
	 std::cout << "\t" << std::flush;
	 }
	 }
	 }
	 std::cout << "\\\\\\hline" << std::endl;
	 }
	 std::cout << "\\end{tabu}%" << std::endl;
	 std::cout << a << " " << b << std::endl;*/

	for (i = 0; i < this->maxLambdaSetKey + 1; i += 1) {
		seedValue = maxJEdgeIdxArray[i];
		lambdaSeedArray.insert(std::make_pair(i, LambdaParamMap { }));
		for (j = minJEdgeIdxArray[i] + lambdaSeedValue - 1; j < seedValue; j +=
				lambdaSeedValue) {
			lambdaCurrentValue = lambdaSet.at(i).at(j);
			if (lowerBound <= lambdaCurrentValue
					&& lambdaCurrentValue <= upperBound) {
				lambdaSeedArray.at(i).insert(
						std::make_pair(j, lambdaCurrentValue));
			}
		}
	}

	/*a = 0;
	 b = 0;
	 std::cout << "\\begin{tabu}{" << std::flush;
	 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
	 std::cout << "|c" << std::flush;
	 }
	 std::cout << "|}" << std::endl;

	 std::cout << "\t\\hline" << std::endl;

	 for (int i = 0; i < this->maxLambdaSetKey + 1; i += 1) {
	 std::cout << "\t" << std::flush;
	 for (int j = 0; j < this->maxLambdaSetKey + 1; j += 1) {
	 if (j + 1 < graph->getNumberOfEdges()) {
	 std::cout << "& " << std::flush;
	 }
	 if (lambdaSeedArray.count(i) > 0
	 && lambdaSeedArray.at(i).count(j) > 0) {
	 a += 1;
	 if (lowerBound <= lambdaSeedArray.at(i).at(j)
	 && lambdaSeedArray.at(i).at(j) <= upperBound) {
	 b += 1;
	 std::cout << "\\cellcolor{black}\t" << std::flush;
	 } else {
	 std::cout << "\t" << std::flush;
	 }
	 }
	 }
	 std::cout << "\\\\\\hline" << std::endl;
	 }
	 std::cout << "\\end{tabu}%" << std::endl;
	 std::cout << a << " " << b << std::endl;*/

	for (auto& lambdaMap : lambdaSeedArray) {
		for (auto& lambdaValue : lambdaMap.second) {
			lambdaFeasibleParameterArray.push_back(lambdaValue.second);
		}
	}

	lambdaCurrentValue = findMedianValue(lambdaFeasibleParameterArray);

	INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_MEDIAN, lambdaCurrentValue,
			lambdaFeasibleParameterArray.size());

	graphCostBackup = tempUpdateGraphEdgeCosts(lambdaCurrentValue);

	newMSTSolution = mstSolver->getSolution();
	differentEdges = getMSTDiff(newMSTSolution);

	GraphUtils::changeGraphCosts(graph, graphCostBackup);
	delete graphCostBackup;

	if (differentEdges == k) {
		INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_SOLUTION,
				lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(newMSTSolution, "\t").c_str(),
				k,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						newMSTSolution, "\t").c_str());
		return newMSTSolution;
	} else if (k < differentEdges) { // jeśli innych krawędzi jest za dużo to trzeba zwiększyć LP lambdy
		INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_TO_BIGGER_LAMBDA,
				lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(newMSTSolution, "\t").c_str(),
				differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						newMSTSolution, "\t").c_str(), k, lowerBound,
				lambdaCurrentValue);
		lowerBound = lambdaCurrentValue;
		MemoryUtils::removeCollection(newMSTSolution, false);
	} else { // jeśli innych krawędzi jest za mało to trzeba zmniejszyć UP lambdy
		INFO(logger, LogBundleKey::BS_V1_IMST_BIN_SEARCH_TO_LOWER_LAMBDA,
				lambdaCurrentValue, newMSTSolution->getTotalEdgeCost(),
				LogStringUtils::edgeSetVisualization(newMSTSolution, "\t").c_str(),
				differentEdges,
				LogStringUtils::mstEdgeDifference(this->baseMSTSolution,
						newMSTSolution, "\t").c_str(), k, upperBound,
				lambdaCurrentValue);
		upperBound = lambdaCurrentValue;
		MemoryUtils::removeCollection(newMSTSolution, false);
	}

	/*int a = 0, b = 0;

	 std::cout << "\\begin{tabu}{" << std::flush;
	 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
	 std::cout << "|c" << std::flush;
	 }
	 std::cout << "|}" << std::endl;

	 std::cout << "\t\\hline" << std::endl;

	 for (int i = 0; i < graph->getNumberOfEdges(); i += 1) {
	 std::cout << "\t" << std::flush;
	 for (int j = 0; j < graph->getNumberOfEdges(); j += 1) {
	 if (j + 1 < graph->getNumberOfEdges()) {
	 std::cout << "& " << std::flush;
	 }
	 if (tmpLambdaSet.count(i) > 0 && tmpLambdaSet.at(i).count(j) > 0) {
	 a += 1;
	 if (lowerBound <= tmpLambdaSet.at(i).at(j) && tmpLambdaSet.at(i).at(j) <= upperBound) {
	 b += 1;
	 std::cout << "\\cellcolor{black}\t" << std::flush;
	 } else {
	 std::cout << "\t" << std::flush;
	 }
	 }
	 }
	 std::cout << "\\\\\\hline" << std::endl;
	 }
	 std::cout << "\\end{tabu}%" << std::endl;
	 std::cout << a << " " << b << std::endl;
	 */

	return resolve(k);
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, EdgeSetIF* baseSolution, VertexIF* initialVertex,
		LambdaValue lowerBound, LambdaValue upperBound) :
		IMSTSolverIF(mstSolverType, graph, baseSolution, initialVertex,
				lowerBound, upperBound) {
	// Nie zachowujemy kosztóœ bazowego rozwiązania, bo może się okazać, że nie potrzeba ich modyfikować.
	this->baseMSTEdgeCosts = nullptr;
	this->lambdaSet = LambdaParamPairMap { };
	// ustawiane w funkcji, która faktycznie generuje zbiór lambd: generateLambdaSet
	this->maxLambdaSetKey = 0;
}

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, VertexIF* initialVertex, LambdaValue lowerBound,
		LambdaValue upperBound) :
		BinarySearch_v1(mstSolverType, graph, nullptr, initialVertex,
				lowerBound, upperBound) {
}

BinarySearch_v1::BinarySearch_v1(GraphIF* const graph, EdgeSetIF* baseSolution,
		VertexIF* initialVertex, LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v1(MSTSolverEnum::DEFAULT, graph, baseSolution,
				initialVertex, lowerBound, upperBound) {
}

BinarySearch_v1::BinarySearch_v1(GraphIF* const graph, VertexIF* initialVertex,
		LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v1(MSTSolverEnum::DEFAULT, graph, nullptr, initialVertex,
				lowerBound, upperBound) {
}

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, VertexIF* initialVertex) :
		BinarySearch_v1(mstSolverType, graph, nullptr, initialVertex, 0, 0) {
}

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, LambdaValue lowerBound, LambdaValue upperBound) :
		BinarySearch_v1(mstSolverType, graph, nullptr, nullptr, lowerBound,
				upperBound) {
}

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph, EdgeSetIF* const baseSolution) :
		BinarySearch_v1(mstSolverType, graph, baseSolution, nullptr, 0, 0) {
}

BinarySearch_v1::BinarySearch_v1(MSTSolverEnum const mstSolverType,
		GraphIF* const graph) :
		BinarySearch_v1(mstSolverType, graph, nullptr, nullptr, 0, 0) {
}

BinarySearch_v1::BinarySearch_v1(GraphIF* const graph, EdgeSetIF* baseSolution) :
		BinarySearch_v1(MSTSolverEnum::DEFAULT, graph, baseSolution, nullptr, 0,
				0) {
}

BinarySearch_v1::BinarySearch_v1(GraphIF* const graph) :
		BinarySearch_v1(MSTSolverEnum::DEFAULT, graph, nullptr, nullptr, 0, 0) {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

