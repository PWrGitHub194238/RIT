/*
 * TabuSearch.hpp
 *
 *  Created on: 14 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_
#define SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_

#include "../enums/AIMSTSolverEnum.hpp"
#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "../utils/enums/TabuMapEnum.hpp"
#include "RIMSTSolverIF.hpp"

#include <sstream>

class MSTSolverIF;

class GraphIF;
class IMSTSolverIF;

class TabuSearch: public RIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	/// @brief Value r[i][k] determines how many times edge \f$\left( i, j \right)\f$ belonged to solutions
	///
	/// that was found by TabuSearch algorithm up to now.
	unsigned int **r;

	/// @brief Value r[i][k] represents a sum of every solution's cost that includes edge \f$\left( i, j \right)\f$.
	unsigned int **sr;

	/// @brief Value mr[i][k] is an average cost of solutions that includes edge \f$\left( i, j \right)\f$.
	double **mr;

	/// @brief List of forbidden moves.
	///
	/// TabuMap is build from two TabuSupMap where first contains list
	/// of those edges that cannot be dropped from solution for some remaining time.
	/// Second sublist contains edges that cannot be added to new solution.
	/// To get correct list, use getTabuList(TabuMapEnum).
	TabuMap tabuMap;

	/// @brief Cadency for every element in tabu list.
	///
	/// By performing one move i.e. form edge \f$\left( i, j \right)\f$ to \f$\left( k, l \right)\f$,
	/// both edges will be added to tabu list for the next tabuPeriod iterations of TabuSearch.
	/// Arc \f$\left( i, j \right)\f$ will be added to the tabu list described by TabuMapEnum::DROP
	/// and edge \f$\left( k, l \right)\f$  to the tabu list described by TabuMapEnum::ADD.
	TabuIterationCount tabuPeriod;

	/// @brief Number of overall iterations of TabuSearch algorithm.
	///
	/// After it reaches this number, algorithm will end.
	TabuIterationCount numberOfIterations;

	/// @brief Number of iterations for each algorithm's path.
	///
	/// After this number of iterations algorithm will
	/// try to find another solution to start from.
	TabuIterationCount numberOfPathIterations;

	/// @brief Number of vertices in given graph.
	VertexCount vertexCount;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** @brief Returns the worst case alternative for given spanning tree.
	 *
	 * @details Based on RIMSTSolverIF::adversarialScenarioSet, this method will chose a new cost
	 * for every edge in given graph RIMSTSolverIF::graph. That is for every edge
	 * that belongs to given spanningTree, it cost will match the highest cost
	 * of this edge among adversarial scenarios, lowest otherwise.
	 *
	 * @param spanningTree set of edges which costs will be set to highest as possible during generation of the worst case alternative.
	 * @return scenario that represents the worst case alternative for given spanningTree.
	 */
	GraphEdgeCostsIF * getWorstCaseScenario(EdgeSetIF * spanningTree);

	/** @brief Returns whenever stop criterion for TabuSearch algorithm occurs.
	 *
	 * @details TabuSearch algorithm runs TabuSearch::numberOfIterations number of iterations
	 * before it ends. This method returns true if algorithm runs long enough
	 * and it should be terminated with current best find result.
	 *
	 * @param iterationCount number of actual TabuSearch iteration
	 * @return <b>true</b> is iterationCount is greater than TabuSearch::numberOfIterations.
	 */
	bool stopCriterionMet(TabuIterationCount iterationCount);

	/** @brief Returns <b>true</b> if cost of spanning tree neighborCost is smaller than tmpSolutionCost.
	 *
	 * @details Aspiration criteria is an element for strategy to select best move in TabuSearch algorithm.
	 * If aspiration criteria is meet then move that satisfy it will be performed, regardless of tabu list.
	 *
	 * @param neighborCost cost of solution if given move will be accepted (it's not on tabu list
	 * or new solutions's cost satisfy aspiration criteria).
	 * @param tmpSolutionCost cost of actual solution
	 * @return <b>true</b> if solutions's cost neighborCost meet aspiration criteria.
	 */
	bool isAspirationCriteriaMet(EdgeCost const neighborCost,
			EdgeCost const tmpSolutionCost);

	/** @brief Returns tabu list.
	 *
	 * @details Returns tabu list that contains edges that are forbidden to remove from solution
	 * or collection of arcs that cannot be added.
	 *
	 * Move from one spanning tree to another (i.e. by removing edge \f$\left( i, j \right)\f$, adding \f$\left( k, l \right)\f$)
	 * is allowed if and only if element \f$\left( i, j \right)\f$ is not on list that will be returned
	 * by getTabuList(TabuMapEnum::DROP) and element \f$\left( k, l \right)\f$ is not on the other list,
	 * returned by getTabuList(TabuMapEnum::ADD).
	 *
	 * Move is also allowed, regardless of tabu list, if isAspirationCriteriaMet(EdgeCost,EdgeCost) returns <b>true</b>.
	 *
	 * @param tabuListType type of tabu list to be returned.
	 * @return tabu list that contains elements that are forbidden
	 * to be removed (TabuMapEnum::DROP) or added (TabuMapEnum::ADD) by move from one spanning tree to another.
	 */
	TabuSubMap getTabuList(TabuMapEnum tabuListType);

	/** @brief Check if given move between solutions is allowed by tabu list.
	 *
	 * @details TODO
	 *
	 * @param dropEdgeIdx
	 * @param addEdgeIdx
	 * @return
	 */
	bool isMoveAllowed(EdgeIdx const dropEdgeIdx, EdgeIdx const addEdgeIdx);

	/**
	 *
	 * @param initialSolution sąsiednie drzewo pierwotnego drzewa
	 * @return
	 */
	AIMSTSolution getSolutionForTree(EdgeSetIF* initialSolution);

	/** Już w czasie przeliczania kolejnych sąsiadów, wybiera tego najlepszego,
	 * zamiast policzyć wszystkich sąsiadów i wybrać minimum
	 *
	 * @param spanningTree
	 * @return
	 */
	NeighborSolution findMinimumInNeighborhood(EdgeSetIF* const spanningTree,
			EdgeCost const tmpSolutionCost,
			TabuIterationCount const globalIterationCount);

	TabuMoveValue calculateMoveValue(EdgeCost& oldSolutionCost,
			EdgeCost& newSolutionCost, const VertexIdx& addEdgeIdxI,
			const VertexIdx& addEdgeIdxJ, const VertexIdx& dropEdgeIdxK,
			const VertexIdx& dropEdgeIdxL,
			const TabuIterationCount& globalIterationCount);

	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx);

	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx,
			TabuIterationCount tabuPeriod);

	void eraseTabuList();

	void updateTabuList(SpanningTreeNeighbor && tabuMove);

	void updateMValTables(EdgeSetIF*& spanningTree,
			const EdgeCost spanningTreeCost);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual EdgeSetIF * resolve();

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			MSTSolverEnum innerMstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			MSTSolverEnum innerMstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	virtual ~TabuSearch();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_ */
