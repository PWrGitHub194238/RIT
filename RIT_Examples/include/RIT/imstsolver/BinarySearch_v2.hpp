/*
 * BinarySearch_v2.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_IMSTSOLVER_BINARYSEARCH_V2_HPP_
#define SRC_INCLUDE_IMSTSOLVER_BINARYSEARCH_V2_HPP_

#include <limits>

#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "IMSTSolverIF.hpp"

class EdgeSetIF;
class GraphIF;
class MSTSolverIF;
class VertexIF;

class EdgeIF;
class GraphEdgeCostsIF;

class BinarySearch_v2: public IMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	const unsigned TOTAL_LAMBDA_PARAMETER_COUNTER_THRESHOLD_MULT { 4 };
	const unsigned LAMBDA_PARAMETER_SEED_THRESHOLD_MULT { 2 };

	const LambdaValue MIN_LAMBDA_VALUE { std::numeric_limits<LambdaValue>::min() };

	//************************************** PRIVATE CLASS FIELDS **************************************//

	/** Shrinkujemy cały graf, nie chcemy w ogóle pewnych krawędzi, odłączamy je zatem, potem przywracamy.
	 *
	 */
	ConnectivityList connectivityList;

	/** Przed zabawą z obniżaniem kosztów mst lambdami, musimy mieć podstawowe koszty MST.
	 * W updateGraphEdgeCosts updatujemy dzięki temu różnicowo tj:
	 * c_1 = c_0 - a
	 * c_2 = c1 - (b - a) = c_0 - b
	 * c_3 = c2 - (c - b) = c_0 - c,
	 * gdzie c_0 to ten bazowy koszt.
	 *
	 */
	GraphEdgeCostsIF* baseMSTEdgeCosts;

	GraphIF* shrunkenGraph;

	GraphEdgeCostsIF* shrunkenGraphBaseCostSet;

	EdgeCost* lambdaIncBaseMSTEdgeCostSet;

	EdgeCost* lambdaDecUnboundedMSTEdgeCostTmp;

	EdgeIdx maxLambdaSetKey;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Tworzy nowy, tymczasowy graf z krawędziami baseMSTSolution i unboundedMSTSolution
	 *
	 */
	GraphIF* shrinkEdgeSet(EdgeSetIF* unboundedMSTSolution);

	/**
	 *
	 * modyfikuje tylko te krawędzie, które pozostały widoczne po odpaleniu shrinkEdgeSet (reszta i tak nie należy do rozwiązania).
	 *
	 */
	void edgeCostsPreConfiguration();

	EdgeCost edgeCostPerturbFunction(EdgeCount const numberOfEdges,
			EdgeIdx const edgeIdx, EdgeCost const edgeCost);

	void generateLambdaHelperSets(EdgeSetIF* const unboundedMSTSolution);

	void checkLambdaBounds(EdgeCount k);

	/** Zmniejsza koszty o różnice w lambdach.
	 *
	 * @param lamdaParameter
	 */
	void updateGraphEdgeCosts(LambdaValue lamdaParameter);

	//TODO do zmiany
	//EdgeIdx findMinEdgeIdxForLambdaParam(EdgeIdx const baseMSTEdgeIdx);

	//TODO do zmiany
	//EdgeIdx findMaxEdgeidxForLambdaParam(EdgeIdx const baseMSTEdgeIdx);

	EdgeSetIF* binarySearchForSolution(EdgeCount k,
			LambdaParamArray lambdaFeasibleParameterSet);

	LambdaValue getLambda(EdgeIdx i, EdgeIdx j);

	LambdaValue findMedianValue(
			LambdaParamArray lambdaFeasibleSeededParameterArray);

	EdgeSetIF* resolve(IncrementalParam k, VertexIF* initialVertex);

	EdgeSetIF * resolve(IncrementalParam k);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** Ustawia początkowe rozwiazanie na baseSolution,
	 * do rozwiązywania wykorzysta mstSolver.
	 *
	 * @param mstSolver
	 * @param graph
	 * @param baseSolution
	 * @param initialVertex
	 * @param lowerBound
	 * @param upperBound
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			EdgeSetIF* baseSolution, VertexIF* initialVertex,
			LambdaValue lowerBound, LambdaValue upperBound);

	/** Ustawia początkowe rozwiazanie na MST grafu,
	 * do rozwiązywania wykorzysta mstSolver.
	 *
	 * @param mstSolver
	 * @param graph
	 * @param initialVertex
	 * @param lowerBound
	 * @param upperBound
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			VertexIF* initialVertex, LambdaValue lowerBound,
			LambdaValue upperBound);

	/** Ustawia początkowe rozwiazanie na baseSolution,
	 * do rozwiązywania wykorzysta domyślny mstSolver.
	 *
	 * @param graph
	 * @param baseSolution
	 * @param initialVertex
	 * @param lowerBound
	 * @param upperBound
	 */
	BinarySearch_v2(GraphIF* const graph, EdgeSetIF* baseSolution,
			VertexIF* initialVertex, LambdaValue lowerBound,
			LambdaValue upperBound);

	/** Znajduje na wstępie MST dla podanego grafu. Klasa bazowa stworzy domyślną implementację MSTSolvera.
	 *
	 * @param graph
	 * @param initialVertex
	 * @param lowerBound
	 * @param upperBound
	 */
	BinarySearch_v2(GraphIF* const graph, VertexIF* initialVertex,
			LambdaValue lowerBound, LambdaValue upperBound);

	/** Znajduje na wstępie MST dla podanego grafu. Korzysta z podanego mstSolvera.
	 * Sam znajdzie parametry lambda.
	 *
	 * @param mstSolver
	 * @param graph
	 * @param initialVertex
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			VertexIF* initialVertex);

	/** Znajduje na wstępie MST dla podanego grafu. Korzysta z podanego mstSolvera.
	 *
	 *
	 * @param mstSolver
	 * @param graph
	 * @param lowerBound
	 * @param upperBound
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			LambdaValue lowerBound, LambdaValue upperBound);

	/** Znajduje na wstępie MST dla podanego grafu. Korzysta z podanego mstSolvera.
	 * Sam znajdzie lambdy.
	 *
	 * @param mstSolver
	 * @param graph
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			EdgeSetIF* baseSolution);

	/** Znajduje na wstępie MST dla podanego grafu. Korzysta z podanego mstSolvera.
	 * Sam znajdzie lambdy.
	 *
	 * @param mstSolver
	 * @param graph
	 */
	BinarySearch_v2(MSTSolverEnum const mstSolverType, GraphIF* const graph);

	/** Ustawia wstępne rozwiązanie na podane. Korzysta z domyślnego mstSolvera.
	 * Sam znajdzie lambdy.
	 *
	 * @param graph
	 * @param baseSolution
	 */
	BinarySearch_v2(GraphIF* const graph, EdgeSetIF* baseSolution);

	/** Znajduje na wstępie MST dla podanego grafu. Korzysta z domyślnego mstSolvera.
	 * Sam znajdzie lambdy.
	 *
	 * @param graph
	 */
	BinarySearch_v2(GraphIF* const graph);

	// Empty virtual destructor for proper cleanup
	virtual ~BinarySearch_v2() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_IMSTSOLVER_BINARYSEARCH_V2_HPP_ */
