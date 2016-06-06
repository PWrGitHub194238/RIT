/*
 * CPLEX_LP_IMSTSolverIF.hpp
 *
 *  Created on: 22 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolverIF_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolverIF_HPP_

#include <ilconcert/iloenv.h>
#include <ilconcert/iloexpression.h>
#include <ilconcert/ilomodel.h>
#include <ilcplex/ilocplexi.h>
#include <map>
#include <string>
#include <utility>

#include "../enums/EdgeConnectionType.hpp"
#include "../structures/EdgeIF.hpp"
#include "../typedefs/primitive.hpp"
#include "CPLEX_Typedefs.hpp"

#include "../exp/LogicExceptions.hpp"
#include "../exp/ModelExceptions.hpp"

class GraphIF;
class CPLEX_LP_MSTSolverIF;
class EdgeIF;

/** Mapowanie zmiennej LP z właściwą jej krawędzią po danych indeksach wierzchołków, które łączy
 *
 */
typedef std::map<VertexIdx, std::pair<IloNumVar, EdgeIF*>> IloTargetVertexEdgeVarMap;
typedef std::map<VertexIdx, IloTargetVertexEdgeVarMap> IloEdgeVarMap;

class CPLEX_LP_IMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	/** Common z IMSTSolverIF. Jeśli nie podano solvera mst, to tworzony jest nowy i trzeba go zwolnić
	 *
	 */
	bool defaultSolverUsed;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	virtual void buildModel(IncrementalParam k) = 0;

	virtual void generateGoal() = 0;

	virtual EdgeSetIF* LP_Solve() throw (IloException) = 0;

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	CPLEX_LP_MSTSolverIF* mstSolver;
	// Common z IMSTSolverIF.
	GraphIF* graph;

	/** Common z IMSTSolverIF. Trzyma koszty grafu (np. zmieniamy koszty, ale chcemy tylko zobaczyć co by było, a nie zmieniać koszty na stałe).
	 * Po wykonaniu roboty zostaną przywrócone.
	 *
	 */
	GraphEdgeCostsIF* baseGraphEdgeCosts;

	/** Common z IMSTSolverIF. Rozwiązanie, od którego chcemy się różnić nie więcej niż k
	 *
	 */
	EdgeSetIF* baseMSTSolution;
	IloEdgeValMap baseLPMSTSolution;
	// Common z IMSTSolverIF.
	bool isCostChanged;

	IloEnv env;
	IloModel model;
	IloCplex cplex;

	IloEdgeVarMap edgeVariableMap;
	IloNumVarArray edgeVariableArray;

	IloEdgeVarMap edgeAddVariableMap;
	IloNumVarArray edgeAddVariableArray;
	IloEdgeVarMap edgeDropVariableMap;
	IloNumVarArray edgeDropVariableArray;

	VertexCount numberOfVertices;
	EdgeCount numberOfEdges;

	EdgeSetIF* solution;

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** Common z IMSTSolverIF. Ustawia baseMSTSolution jako bazowe rozwiązanie
	 *
	 * @param baseMSTSolution
	 */
	void replaceBaseMSTSolution(EdgeSetIF* baseMSTSolution);

	EdgeSetIF* transformSolutionToEdgeSet();

	EdgeIF* getEdge(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getEdgeVariable(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getEdgeVariable(VertexIF* const sourceVertex,
			VertexIF* const targetVertex);

	IloNumVar getEdgeVariable(const EdgeIF* edge);

	IloNumVar getAddEdgeVariable(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getAddEdgeVariable(VertexIF* const sourceVertex,
			VertexIF* const targetVertex);

	IloNumVar getAddEdgeVariable(const EdgeIF* edge);

	IloNumVar getDropEdgeVariable(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getDropEdgeVariable(VertexIF* const sourceVertex,
			VertexIF* const targetVertex);

	IloNumVar getDropEdgeVariable(const EdgeIF* edge);

	std::string getVariableName(IloNumVar const & variable);

	std::string getCPLEXStatus(IloCplex& cplex);

	/** W LP_* przeciążamy LP_Solve, a nie resolve.
	 *
	 * @param initialVertex
	 * @return
	 */
	EdgeSetIF * resolve(IncrementalParam k)
			throw (ModelExceptions::GeneralConcertException);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/**
	 *
	 * @param mstSolver jeśli null to stworzy domyślny i w destruktorze go usunie
	 * @param graph
	 * @param baseSolution jeśli null to weźmie mst jako bazowe rozwiązanie
	 * @param edgeVariablesType
	 */
	CPLEX_LP_IMSTSolverIF(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF * const graph, EdgeSetIF * baseSolution,
			IloNumVar::Type edgeVariablesType);

	virtual ~CPLEX_LP_IMSTSolverIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Common z IMSTSolverIF.
	 *
	 * @param sourceVertex
	 * @param targetvertex
	 * @param connectionType
	 * @param newCost
	 */
	void changeEdgeCost(VertexIF* const sourceVertex,
			VertexIF* const targetvertex,
			EdgeConnectionType const connectionType, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	/** Common z IMSTSolverIF.
	 *
	 * @param sourceVertex
	 * @param targetvertex
	 * @param newCost
	 */
	void changeEdgeCost(VertexIF* const sourceVertex,
			VertexIF* const targetvertex, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	/** Common z IMSTSolverIF.
	 *
	 * @param sourceVertexId
	 * @param targetvertexId
	 * @param connectionType
	 * @param newCost
	 */
	void changeEdgeCost(VertexIdx const sourceVertexId,
			VertexIdx const targetvertexId,
			EdgeConnectionType const connectionType, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	/** Common z IMSTSolverIF.
	 *
	 * @param sourceVertexId
	 * @param targetvertexId
	 * @param newCost
	 */
	void changeEdgeCost(VertexIdx const sourceVertexId,
			VertexIdx const targetvertexId, EdgeCost newCost)
					throw (LogicExceptions::EdgeNullPointerException);

	/** Common z IMSTSolverIF.
	 *
	 * @param edge
	 * @param newCost
	 */
	void changeEdgeCost(EdgeIF* edge, EdgeCost newCost)
			throw (LogicExceptions::EdgeNullPointerException);

	/** Common z IMSTSolverIF. Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getMST(EdgeCount k, EdgeSetIF* baseMSTSolution,
			GraphEdgeCostsIF* newGraphCosts);

	/** Common z IMSTSolverIF. Zwraca MST dla grafu o kosztach newGraphCosts,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param newGraphCosts nowe koszty grafu
	 * @return
	 */
	EdgeSetIF* getMST(EdgeCount k, GraphEdgeCostsIF* newGraphCosts);

	/** Common z IMSTSolverIF. Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od baseMSTSolution.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @param baseMSTSolution narzuca konkretny zbiór krawędzi jako bazowy
	 * (niekoniecznie musi być to mst, ale algorytm musi znaleźć rozwiązanie
	 * różniące się od tego zbioru o nie więcej niż k).
	 * @return
	 */
	EdgeSetIF* getMST(EdgeCount k, EdgeSetIF* baseMSTSolution);

	/** Common z IMSTSolverIF. Zwraca MST dla podanego w konstruktorze grafu,
	 * które nie różni się o więcej niż k krawędzi od poprzedniego rozwiązania.
	 *
	 * @param k o ile może się maks różnić nowe rozwiązanie od bazowego
	 * @return
	 */
	EdgeSetIF* getMST(EdgeCount k);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolverIF_HPP_ */
