/*
 * IMSTSolverIF.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERIF_HPP_
#define SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERIF_HPP_

#include "../enums/EdgeConnectionType.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"

#include "../exp/LogicExceptions.hpp"

class EdgeIF;
class EdgeSetIF;
class GraphEdgeCostsIF;
class GraphIF;
class MSTSolverIF;
class VertexIF;

class IMSTSolverIF {

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	MSTSolverEnum mstSolverType;
	MSTSolverIF* mstSolver;
	GraphIF* graph;

	/** Trzyma koszty grafu (np. zmieniamy koszty, ale chcemy tylko zobaczyć co by było, a nie zmieniać koszty na stałe).
	 * Po wykonaniu roboty zostaną przywrócone.
	 *
	 */
	GraphEdgeCostsIF* baseGraphEdgeCosts;

	/** Rozwiązanie, od którego chcemy się różnić nie więcej niż k
	 *
	 */
	EdgeSetIF* baseMSTSolution;

	LambdaValue lowerBound;
	LambdaValue upperBound;
	bool isCostChanged;

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** Ustawia baseMSTSolution jako bazowe rozwiązanie
	 *
	 * @param baseMSTSolution
	 */
	void replaceBaseMSTSolution(EdgeSetIF* baseMSTSolution);

	/** Aktualizuje koszty krawędzi w grafie.
	 *
	 */
	void replaceEdgeCosts(GraphEdgeCostsIF* newGraphCosts);

	/** Przywraca koszty w caym grafie po znalezieniu rozwiązania (przywraca baseGraphEdgeCosts)
	 *
	 */
	void restoreBaseEdgeCosts();

	/** Zwraca liczbę krawędzi, które są w podanym zbiorze krawędzi a nie ma ich w baseMSTSolution
	 *
	 */
	EdgeCount getMSTDiff(EdgeSetIF* const currentMSTSolution);

	/**
	 *
	 * @param k
	 * @param initialVertex
	 * @return
	 */
	virtual EdgeSetIF* resolve(IncrementalParam k, VertexIF* initialVertex) = 0;

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//}

	/**
	 *
	 * @param mstSolverType
	 * @param graph
	 * @param baseSolution jeśli null to weźmie mst jako bazowe rozwiązanie
	 * @param initialVertex
	 * @param lowerBound
	 * @param upperBound
	 */
	IMSTSolverIF(MSTSolverEnum const mstSolverType, GraphIF* const graph,
			EdgeSetIF * baseSolution, VertexIF* initialVertex,
			LambdaValue lowerBound, LambdaValue upperBound);

	virtual ~IMSTSolverIF();

//*************************************** PUBLIC FUNCTIONS *****************************************//

	void changeEdgeCost(VertexIF* const sourceVertex,
			VertexIF* const targetvertex,
			EdgeConnectionType const connectionType, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	void changeEdgeCost(VertexIF* const sourceVertex,
			VertexIF* const targetvertex, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	void changeEdgeCost(VertexIdx const sourceVertexId,
			VertexIdx const targetvertexId,
			EdgeConnectionType const connectionType, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	void changeEdgeCost(VertexIdx const sourceVertexId,
			VertexIdx const targetvertexId, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	void changeEdgeCost(EdgeIF* edge, EdgeCost newCost)
			throw (LogicExceptions::EdgeNullPointerException);

	EdgeSetIF* getMST();

	/** Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @param initialVertex początkowy wierzchołek dla MSTSolver przy szukaniu unbounded
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, EdgeSetIF* baseMSTSolution,
			VertexIF* initialVertex, GraphEdgeCostsIF* newGraphCosts);

	/** Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param initialVertex początkowy wierzchołek dla MSTSolver przy szukaniu unbounded
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, VertexIF* initialVertex,
			GraphEdgeCostsIF* newGraphCosts);

	/** Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, EdgeSetIF* baseMSTSolution,
			GraphEdgeCostsIF* newGraphCosts);

	/** Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, GraphEdgeCostsIF* newGraphCosts);

	/** Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @param initialVertex początkowy wierzchołek dla MSTSolver przy szukaniu unbounded
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, EdgeSetIF* baseMSTSolution,
			VertexIF* initialVertex);

	/** Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param initialVertex początkowy wierzchołek dla MSTSolver przy szukaniu unbounded
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, VertexIF* initialVertex);

	/** Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k, EdgeSetIF* baseMSTSolution);

	/** Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @return
	 */
	EdgeSetIF* getSolution(EdgeCount k);

//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERIF_HPP_ */
