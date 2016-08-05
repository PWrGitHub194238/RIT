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

/** @brief Implements TabuSearch algorithm for Robust Recoverable Incremental Minimum Spanning Tree.
 *
 * @details It shears common interface RIMSTSolverIF that allows user to use it like any other solver-type class.
 *
 * Example below shows how to solve RRIMST problem with TabuSearch:
 *
 * @code
 *	std::ostringstream oss { };
 *
 *	IncrementalParam k { 5 };
 *	TabuIterationCount tabuPeriod { 100 };
 *	TabuIterationCount numberOfPathIterations { 50 };
 *	TabuIterationCount numberOfIterations { 2000 };
 *
 *	GraphIF* g = InputUtils::readGraph("test/rimst/TabuSearch/4/s0.json", InputFormat::VA, InputMode::HDD);
 *	GraphEdgeCostsSet adversarialScenarios { };
 *
 *	for (int i = 1; i <= 2; i += 1) {
 *		oss << "test/rimst/TabuSearch/4/Adv/s" << i << ".json";
 *		adversarialScenarios.insert(
 *			InputUtils::readCosts(oss.str().c_str(), InputFormat::VA, InputMode::HDD)
 *		);
 *		oss.str("");
 *		oss.clear();
 *	}
 *
 *	RIMSTSolverIF* rimstSolver = new RIMSTSolverImpl { AIMSTSolverEnum::DEFAULT,
 *		IMSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT, g, adversarialScenarios, k,
 *		tabuPeriod, numberOfPathIterations, numberOfIterations
 *	};
 *
 *	EdgeSetIF* solution = rimstSolver->getMST();
 *	delete rimstSolver;
 *
 *	std::cout << solution->toString() << std::endl;
 *	std::cout << solution->getTotalEdgeCost() << std::endl;
 *
 *	MemoryUtils::removeCollection(solution, false);
 *	MemoryUtils::removeScenarioSet(adversarialScenarios);
 *	MemoryUtils::removeGraph(g, true, true);
 * @endcode
 *
 */
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
	 * @details The move from one spanning tree to another is allowed if and only if
	 * edge with index dropEdgeIdx, which will be exchanged by new edge with index
	 * addEdgeIdx (edge that is not a part of current spanning tree), is not listed on
	 * tabu list as well as the new one. If edge with index dropEdgeIdx is on tabu list
	 * that contains dropped elements or if edge with index addEdgeIdx is on tabu list
	 * that contains added elements then move is forbidden.
	 *
	 * @param dropEdgeIdx index of graph's edge to be removed from spanning tree
	 * @param addEdgeIdx index of graph's edge that will replace removed edge in new solution that would be obtained after given move.
	 * @return <b>true</b> if given move is not forbidden by tabu list.
	 */
	bool isMoveAllowed(EdgeIdx const dropEdgeIdx, EdgeIdx const addEdgeIdx);

	/** @brief Returns a solution for Adversarial Incremental Minimum Spanning Tree problem for given initial solution initialSolution.
	 *
	 * @details Using RIMSTSolverIF::aimstSolver, this function will return the solution for AIMST based on
	 * scenarios available for adversary RIMSTSolverIF::adversarialScenarioSet and parameter RIMSTSolverIF::k
	 * that is a part of description of Incremental Minimum Spanning Tree problem that describes
	 * how the IMST solution can differ from given initialSolution (how many edges we can change).
	 *
	 * @param initialSolution spanning tree for IMST problem with parameter RIMSTSolverIF::k which
	 * forbids any solutions that have more than <b>k</b> edges, that are not a part of initialSolution, to be returned.
	 * @return solution for Adversarial Incremental Minimum Spanning Tree problem for given set
	 * of adversarial scenarios and incremental parameter.
	 */
	AIMSTSolution getSolutionForTree(EdgeSetIF* initialSolution);

	/** @brief Calculates and returns the best neighbor solution for spanningTree.
	 *
	 * @details Generates every possible neighbor for given spanningTree
	 * and returns one that results in the highest value of calculateMoveValue().
	 * Neighbor of given spanningTree is a spanning tree that differs from the first one
	 * exactly by one edge - one move. In other words, solution that will be returned
	 * determinates the best move that TabuSearch algorithm can make, based on current
	 * solution's cost (tmpSolutionCost) and number of passed iterations - globalIterationCount.
	 *
	 * @param spanningTree set of edges that represents current solution
	 * and determinates a set of neighbor spanning trees from which this method
	 * will have to chose one tree that is the best in terms of calculateMoveValue().
	 * @param tmpSolutionCost solution cost for RRIMST problem for spanningTree
	 * @param globalIterationCount number of current TabuSearch iteration
	 * @return New spanning tree that is a neighbor of given spanningTree
	 * and it seems to be the best solution to chose according to calculateMoveValue().
	 */
	NeighborSolution findMinimumInNeighborhood(EdgeSetIF* const spanningTree,
			EdgeCost const tmpSolutionCost,
			TabuIterationCount const globalIterationCount);

	/** @brief Calculates value of a move from one spanning tree to other solution.
	 *
	 * @details Based on given set of parameters, this function will return a value
	 * for move that consists of adding arc \f$\left( i, j \right)\f$ and removing
	 * edge \f$\left( k, l \right)\f$, where all indexes of mentioned pairs are
	 * described by variables from addEdgeIdxI to dropEdgeIdxL. Default implementation
	 * uses global parameters such TabuSearch::r or TabuSearch::mr along with
	 * costs of both old and new solutions.
	 *
	 * @param oldSolutionCost RRIMST solution cost for old spanning tree
	 * @param newSolutionCost RRIMST solution cost for new spanning tree that was created
	 * by replacing edge \f$\left( k, l \right)\f$ with arc \f$\left( i, j \right)\f$.
	 * @param addEdgeIdxI index of a vertex \f$ v_{i} \f$ that is connected by edge \f$\left( i, j \right)\f$
	 * @param addEdgeIdxJ index of a vertex \f$ v_{j} \f$ that is connected by edge \f$\left( i, j \right)\f$
	 * @param dropEdgeIdxK index of a vertex \f$ v_{k} \f$ that is connected by edge \f$\left( k, l \right)\f$
	 * @param dropEdgeIdxL index of a vertex \f$ v_{l} \f$ that is connected by edge \f$\left( k, l \right)\f$
	 * @param globalIterationCount number of current TabuSearch iteration
	 * @return value of given move. The higher the given move is rated, the better it is.
	 */
	TabuMoveValue calculateMoveValue(const EdgeCost oldSolutionCost,
			const EdgeCost newSolutionCost, const VertexIdx addEdgeIdxI,
			const VertexIdx addEdgeIdxJ, const VertexIdx dropEdgeIdxK,
			const VertexIdx dropEdgeIdxL,
			const TabuIterationCount globalIterationCount);

	/** @brief Inserts a given edge into selected tabu list for default number of iterations (TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD).
	 *
	 * @details Inserts an edge with a given index edgeIdx into one of selected tabu lists.
	 * Adding a given element with parameter TabuMapEnum::ADD will make
	 * any move that consists of adding edge with index edgeIdx forbidden
	 * for TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD number of iterations.
	 * Similarly adding a given element with parameter TabuMapEnum::DROP will make
	 * any move that consists of removing edge with index edgeIdx forbidden
	 * for TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD number of iterations.
	 *
	 * @param tabuListType type of tabu list into which we add an element
	 * @param edgeIdx edge index which will be forbidden in any moves
	 * for next TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD iterations.
	 */
	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx);

	/** @brief Inserts a given edge into selected tabu list for given number of iterations.
	 *
	 * @details Inserts an edge with a given index edgeIdx into one of selected tabu lists.
	 * Adding a given element with parameter TabuMapEnum::ADD will make
	 * any move that consists of adding edge with index edgeIdx forbidden
	 * for tabuPeriod iterations.
	 * Similarly adding a given element with parameter TabuMapEnum::DROP will make
	 * any move that consists of removing edge with index edgeIdx forbidden
	 * for tabuPeriod iterations.
	 *
	 * @param tabuListType type of tabu list into which we add an element
	 * @param edgeIdx edge index which will be forbidden in any moves
	 * for next tabuPeriod iterations
	 * @param tabuPeriod for how many iterations given edge will be forbidden
	 */
	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx,
			TabuIterationCount tabuPeriod);

	/** @brief Removes all entries from tabu list.
	 *
	 *	@details It removes all elements from both tabu lists: TabuMapEnum::ADD and TabuMapEnum::DROP.
	 */
	void eraseTabuList();

	/** @brief Updates tabu list with a given move.
	 *
	 * @details Updates all entries in tabu list by lowering their remaining cadence by one
	 * or removing an element from it in case it's cadence reaches 0. After that
	 * it inserts tabuMove into tabu list - edge that was added goes to tabu list of kind TabuMapEnum::ADD
	 * and edge that was removed by performing this move goes to tabu list of kind TabuMapEnum::DROP.
	 *
	 * @param tabuMove move to be added into tabu list
	 */
	void updateTabuList(SpanningTreeNeighbor && tabuMove);

	/** @brief Updates TabuSearch::r, TabuSearch::sr and TabuSearch::mr parameters.
	 *
	 * @details Each of those parameters holds information needed to calculation performed by calculateMoveValue().
	 * For every edge in given spanningTree values of appropriate parameters will be updated.
	 *
	 * @param spanningTree spanning tree that usually is another solution returned by TabuSearch algorithm after performing some move
	 * @param spanningTreeCost overall weight of all edges that creates given spanningTree
	 */
	void updateMValTables(EdgeSetIF*& spanningTree,
			const EdgeCost spanningTreeCost);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** @brief Runs main algorithm to solve given Recoverable Robust Incremental Minimum Spanning Tree problem.
	 *
	 * @details It is a core method that RIMSTSolverIF::getMST() will call, optionally with additional
	 * modifications around it (i.e. IMSTSolverIF::getMST(GraphEdgeCostsIF)).
	 *
	 * @return Spanning Tree that is the best found solution for given instance of the RRIMST problem.
	 */
	virtual EdgeSetIF * resolve();

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to take control of every single step that TabuSearch algorithm will perform,
	 * starting by selection of algorithms that will be used to solved related problems:
	 * 	- Adversarial Incremental Minimum Spanning Tree (allowed solver types are defined in AIMSTSolverEnum),
	 * 	- Incremental Minimum Spanning Tree (see MSTSolverEnum),
	 * 	- Minimum Spanning Tree ( MSTSolverEnum ).
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * Optional parameters that this method allows to set are TabuSearch specific properties:
	 * 	- tabuPeriod - how long each element that goes to tabu list will remain there (in number of iterations),
	 * 	- numberOfPathIterations - after how many iterations without decreasing value of found solution
	 * 	the entirely new solution will be picked,
	 * 	- numberOfIterations - number of overall iterations of TabuSearch algorithm after which it returns
	 * 	the best found solution so far.
	 *
	 * @param aimstSolverType - type of solver for Adversarial Incremental Minimum Spanning Tree
	 * which will use both solvers for Incremental and non-Incremental MST problem (SolverFactory::getAIMSTSolver()),
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 * @param tabuPeriod - number of iterations that any element inserted into tabu list will remain in it,
	 * @param numberOfPathIterations - number of iterations that algorithm will perform without improving solution's value
	 * before it restarts itself,
	 * @param numberOfIterations - overall number of iterations after which algorithm will return a solution
	 * that it managed to find in given time.
	 */
	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to take control of every single step that TabuSearch algorithm will perform,
	 * starting by selection of algorithms that will be used to solved related problems:
	 * 	- Adversarial Incremental Minimum Spanning Tree (allowed solver types are defined in AIMSTSolverEnum),
	 * 	- Incremental Minimum Spanning Tree (see MSTSolverEnum),
	 * 	- Minimum Spanning Tree ( MSTSolverEnum ).
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * TabuSearch specific properties are default (see TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
	 * TabuSearchUtils::PATH_ITER_NUM_DEFAULT and TabuSearchUtils::ITER_NUM_DEFAULT).
	 *
	 * @param aimstSolverType - type of solver for Adversarial Incremental Minimum Spanning Tree
	 * which will use both solvers for Incremental and non-Incremental MST problem (SolverFactory::getAIMSTSolver()),
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree.
	 */
	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to take control of given steps that TabuSearch algorithm will perform,
	 * starting by selection of algorithms that will be used to solved related problems:
	 * 	- Incremental Minimum Spanning Tree (see MSTSolverEnum),
	 * 	- Minimum Spanning Tree ( MSTSolverEnum ).
	 *
	 * Solver type for Adversarial Incremental Minimum Spanning Tree problem is selected by default - see AIMSTSolverEnum::DEFAULT.
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * Optional parameters that this method allows to set are TabuSearch specific properties:
	 * 	- tabuPeriod - how long each element that goes to tabu list will remain there (in number of iterations),
	 * 	- numberOfPathIterations - after how many iterations without decreasing value of found solution
	 * 	the entirely new solution will be picked,
	 * 	- numberOfIterations - number of overall iterations of TabuSearch algorithm after which it returns
	 * 	the best found solution so far.
	 *
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 * @param tabuPeriod - number of iterations that any element inserted into tabu list will remain in it,
	 * @param numberOfPathIterations - number of iterations that algorithm will perform without improving solution's value
	 * before it restarts itself,
	 * @param numberOfIterations - overall number of iterations after which algorithm will return a solution
	 * that it managed to find in given time.
	 */
	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to take control of given steps that TabuSearch algorithm will perform,
	 * starting by selection of algorithms that will be used to solved related problems:
	 * 	- Incremental Minimum Spanning Tree (see MSTSolverEnum),
	 * 	- Minimum Spanning Tree ( MSTSolverEnum ).
	 *
	 * Solver type for Adversarial Incremental Minimum Spanning Tree problem is selected by default - see AIMSTSolverEnum::DEFAULT.
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * TabuSearch specific properties are default (see TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
	 * TabuSearchUtils::PATH_ITER_NUM_DEFAULT and TabuSearchUtils::ITER_NUM_DEFAULT).
	 *
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree.
	 */
	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to chose base solver that will be used by TabuSearch.
	 *
	 * Solver type for Adversarial Incremental Minimum Spanning Tree problem is selected by default - see AIMSTSolverEnum::DEFAULT.
	 * Solver type for Incremental Minimum Spanning Tree problem is selected by default - see IMSTSolverEnum::DEFAULT.
	 * Solver type of mstSolverType is used for Minimum Spanning Tree problem.
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * Optional parameters that this method allows to set are TabuSearch specific properties:
	 * 	- tabuPeriod - how long each element that goes to tabu list will remain there (in number of iterations),
	 * 	- numberOfPathIterations - after how many iterations without decreasing value of found solution
	 * 	the entirely new solution will be picked,
	 * 	- numberOfIterations - number of overall iterations of TabuSearch algorithm after which it returns
	 * 	the best found solution so far.
	 *
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 * @param tabuPeriod - number of iterations that any element inserted into tabu list will remain in it,
	 * @param numberOfPathIterations - number of iterations that algorithm will perform without improving solution's value
	 * before it restarts itself,
	 * @param numberOfIterations - overall number of iterations after which algorithm will return a solution
	 * that it managed to find in given time.
	 */
	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This method allows to take control of every single step that TabuSearch algorithm will perform,
	 * starting by selection of algorithms that will be used to solved related problems:
	 * 	- Adversarial Incremental Minimum Spanning Tree (allowed solver types are defined in AIMSTSolverEnum),
	 * 	- Incremental Minimum Spanning Tree (see MSTSolverEnum),
	 * 	- Minimum Spanning Tree ( MSTSolverEnum ).
	 *
	 * It also allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * TabuSearch specific properties are default (see TabuSearchUtils::TABU_ELEMENT_DEFAULT_PERIOD,
	 * TabuSearchUtils::PATH_ITER_NUM_DEFAULT and TabuSearchUtils::ITER_NUM_DEFAULT).
	 *
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree.
	 */
	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details This is the base constructor for given class that allows to
	 * take full advantage of configuring all TabuSearch parameters without
	 * getting into implementation details - algorithm constructed by this method uses default solver instances like:
	 * AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT and MSTSolverEnum::DEFAULT.
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * Optional parameters that this method allows to set are TabuSearch specific properties:
	 * 	- tabuPeriod - how long each element that goes to tabu list will remain there (in number of iterations),
	 * 	- numberOfPathIterations - after how many iterations without decreasing value of found solution
	 * 	the entirely new solution will be picked,
	 * 	- numberOfIterations - number of overall iterations of TabuSearch algorithm after which it returns
	 * 	the best found solution so far.
	 *
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 * @param tabuPeriod - number of iterations that any element inserted into tabu list will remain in it,
	 * @param numberOfPathIterations - number of iterations that algorithm will perform without improving solution's value
	 * before it restarts itself,
	 * @param numberOfIterations - overall number of iterations after which algorithm will return a solution
	 * that it managed to find in given time.
	 */
	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	/** @brief Creates TabuSearch instance.
	 *
	 * @details It creates basic TabuSearch instance with default values of parameters.
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementations.
	 *
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 */
	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	virtual ~TabuSearch();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_ */
