/*
 * RRIMSTSolverIF.hpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_RRIMSTSOLVER_RRIMSTSOLVERIF_HPP_
#define SRC_INCLUDE_RRIMSTSOLVER_RRIMSTSOLVERIF_HPP_

#include "../enums/AIMSTSolverEnum.hpp"
#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

#include "../exp/GraphExceptions.hpp"

class AIMSTSolverIF;
class MSTSolverIF;
class GraphIF;
class IMSTSolverIF;

/** @brief Interface for RRRIMST solvers.
 *
 * @details This class serves as the interface for any solvers that
 * deals with Recoverable Robust Incremental Minimum Spanning Tree problem that
 * can be described as follows:
 *
 *  \f[
	\min_{\mathclap{T \in \mathcal{T} \left( G \right)}} \; \left( \sum_{e \in T} c_{e}^{\textbf{s}} + \max_{\mathclap{\textbf{s}^{\prime} \in S}} \quad \quad \min_{\mathclap{T^{\prime} \in \mathcal{T} \left( G_{\textbf{s}^{\prime}}, T, k \right)}} \quad \quad \sum_{e^{\prime} \in T^{\prime}} c_{e^{\prime}}^{\textbf{s}^{\prime}} \right)
  \f]
 *
 * where:
 * 	- \f$ T \f$ - selected spanning tree for graph \f$ G \f$,
 * 	- \f$ \mathcal{T} \left( G \right) \f$ - set of spanning trees for graph \f$ G \f$,
 * 	- \f$ S \f$ - adversary scenarios set,
 * 	- \f$ G_{\textbf{s}^{\prime}} \f$ - graph with edge cost of scenario \f$ \textbf{s}^{\prime} \f$,
 * 	- \f$ k \f$ - number of edges that new solution \f$ T^{\prime} \f$ can differ from \f$ T \f$.
 * 	- \f$ \mathcal{T} \left( G_{\textbf{s}^{\prime}}, T, k \right) \f$ - set of spanning trees that satisfy conditions to be incremental trees for given spanning tree \f$ T \f$,
 *
 * General form for Robust Recoverable problem can be described as:
 *
 *  \f[
  	  \min_{\mathclap{\textbf{x} \in X}} \left( v \left( \textbf{x}, \textbf{s} \right) + \max_{\mathclap{\textbf{s}^{\prime} \in S}} \min_{\mathclap{\textbf{y} \in S_{\textbf{x}}}} v \left( \textbf{y}, \textbf{s}^{\prime} \right) \right)
  \f]
 *
 */
class RRIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	/// @brief Type of solver to use for AIMST problem solving.
	///
	/// @details To solve Recoverable Robust Incremental Minimum Spanning Tree problem
	/// solver for Adversary Incremental Minimum Spanning Tree is needed and this value
	/// determinates a type of that solver. By default (AIMSTSolverEnum::DEFAULT),
	/// AIMSTSolverImpl will be used (and can be changed in AIMSTSolverInclude.hpp file).
	/// In original version #AIMSTSolverImpl states for AIMSTSolver class.
	AIMSTSolverEnum aimstSolverType;

	/// @brief Type of solver to use for IMST problem solving.
	///
	/// @details To solve Recoverable Robust Incremental Minimum Spanning Tree problem
	/// solver for Incremental Minimum Spanning Tree is needed and this value
	/// determinates a type of that solver. By default (IMSTSolverEnum::DEFAULT),
	/// IMSTSolverImpl will be used (and can be changed in IMSTSolverInclude.hpp file).
	/// In original version #IMSTSolverImpl states for BinarySearch_v2 class.
	IMSTSolverEnum imstSolverType;

	/// @brief Type of solver to use for MST problem solving.
	///
	/// @details To solve Recoverable Robust Incremental Minimum Spanning Tree problem
	/// solver for Minimum Spanning Tree is needed and this value
	/// determinates a type of that solver. By default (MSTSolverEnum::DEFAULT),
	/// MSTSolverImpl will be used (and can be changed in MSTSolverInclude.hpp file).
	/// In original version #MSTSolverImpl states for PrimeHeap class.
	MSTSolverEnum mstSolverType;

	/// @brief Solver for AIMST problem that is used to solve RRRIMST problem.
	///
	/// @details Instance of solver that is created based on selected solver types:
	///  - RRIMSTSolverIF::aimstSolverType,
	///  - RRIMSTSolverIF::imstSolverType,
	///  - RRIMSTSolverIF::mstSolverType.
	///
	///	It will be used to solve Adversary Incremental Minimum Spanning Tree subproblem for RRRIMST.
	AIMSTSolverIF* aimstSolver;

	/// @brief Solver for MST problem that is used to solve RRRIMST problem.
	///
	/// @details Instance of this solver is created based on selected solver type - RRIMSTSolverIF::mstSolverType.
	///
	///	It will be used to solve Minimum Spanning Tree subproblem for RRRIMST.
	MSTSolverIF* mstSolver;

	/// @brief Input graph for algorithm.
	GraphIF * graph;

	/// @brief Backup scenario for inputed #graph.
	///
	/// @details During searching for solution, cost of given #graph will change many times
	/// (instead of creating new graph each time where is is required, algorithm will
	/// only change it's edge's costs i.e. to force MST solver to find different minimum spanning tree -
	/// see IMSTSolverIF description for details). To be able to restore original edge costs
	/// after returning a solution for given problem, they have to be stored in separated scenario.
	GraphEdgeCostsIF* graphCostBackup;

	/// @brief Set of adversarial scenarios to chose from during solving AIMST problem.
	///
	/// @details Adversarial Incremental Minimum Spanning Tree problem requires an adversary
	/// to chose the best scenario for him that results in the worst value of selected solution
	/// by other person that tries to solve inner problem (Incremental Minimum Spanning Tree).
	GraphEdgeCostsSet adversarialScenarioSet;

	/// @brief Mandatory parameter for IMST problem.
	///
	/// @details Inner problem for RRIMST and AIMST requires to know the number
	/// of edges that are allowed to be changed during solving given problems.
	/// Parameter k for Incremental Minimum Spanning Tree Problem specify
	/// the number of edges that output solution can differ from input spanning tree
	/// for given problem.
	IncrementalParam k;

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** @brief Saves scenario for input #graph to #graphCostBackup and set new one.
	 *
	 * @details It makes a copy of original cost of every edge in input #graph.
	 * Afterwards all of them are replaced by ones that are defined by newGraphCosts scenario.
	 * In case newGraphCosts is not defined then backup is made but all edge costs remain.
	 *
	 * @param newGraphCosts new edge costs
	 */
	void backupGraphCosts(GraphEdgeCostsIF* newGraphCosts);

	/** @brief When overridden, it returns a solution for RRIMST problem.
	 *
	 * @details This method has to be overridden by class that implements this interface
	 * which represents basic structure of Recoverable Robust Incremental Minimum Spanning Tree solver.
	 *
	 * For example implementation see TabuSearch class.
	 *
	 * @return Set of edges that represents a spanning tree for inputed #graph.
	 */
	virtual EdgeSetIF * resolve() = 0;

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	/** @brief Base constructor for RRIMST solver.
	 *
	 * @details This method allows to specify types of solvers that has to be used to solve following subproblems:
	 * 	- Adversarial Incremental Minimum Spanning Tree (allowed solver types are defined in #AIMSTSolverEnum),
	 * 	- Incremental Minimum Spanning Tree (see #IMSTSolverEnum),
	 * 	- Minimum Spanning Tree (#MSTSolverEnum).
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementation.
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
	 */
	RRIMSTSolverIF(AIMSTSolverEnum aimstSolverType,
			IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	/** @brief Base constructor for RRIMST solver.
	 *
	 * @details This method allows to specify types of solvers that has to be used to solve following subproblems:
	 * 	- Incremental Minimum Spanning Tree (see #IMSTSolverEnum),
	 * 	- Minimum Spanning Tree (#MSTSolverEnum).
	 *
	 * Default solver for Adversarial Incremental Minimum Spanning Tree problem will be used (AIMSTSolverEnum::DEFAULT).
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementation.
	 *
	 * @param imstSolverType - type of solver for Incremental Minimum Spanning Tree to be used by AIMST solver,
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 */
	RRIMSTSolverIF(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	/** @brief Base constructor for RRIMST solver.
	 *
	 * @details This method allows to specify types of solvers that has to be used to solve following subproblems:
	 * 	- Minimum Spanning Tree (#MSTSolverEnum).
	 *
	 * Default solver for Adversarial Incremental Minimum Spanning Tree problem will be used (AIMSTSolverEnum::DEFAULT).
	 * Default solver for Incremental Minimum Spanning Tree problem will be used (IMSTSolverEnum::DEFAULT).
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementation.
	 *
	 * @param mstSolverType - type of solver for Minimum Spanning Tree to be used both by AIMST solver
	 * (it will create it's own solver based on mstSolverType) and TabuSearch algorithm
	 * to find new starting points after each numberOfPathIterations iterations without improving solution's value,
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 */
	RRIMSTSolverIF(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	/** @brief Base constructor for RRIMST solver.
	 *
	 * @details Creates a solver for Recoverable Robust Incremental Minimum Spanning Tree problem with
	 * default solver types for related subproblems (AIMSTSolverEnum::DEFAULT, IMSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT).
	 *
	 * It allows to set basic mandatory fields like graph for which computation will take place,
	 * adversarialScenarioSet which contains various scenarios for adversary to chose from (AIMST problem)
	 * and parameter k (basic property of Incremental Minimum Spanning Tree Problem).
	 * For more information about all these problems see classes that represents their implementation.
	 *
	 * @param graph - input graph for algorithm,
	 * @param adversarialScenarioSet - set of adversarial scenarios to be used in AIMST problem,
	 * @param k - number of edges for IMST problem that this problem's solution can differ from original spanning tree,
	 */
	RRIMSTSolverIF(GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	virtual ~RRIMSTSolverIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** @brief Return a solution for Recoverable Robust Incremental Minimum Spanning Tree problem
	 * for #graph with given costs of it's edges initialGraphCosts.
	 *
	 * @param initialGraphCosts scenario that contains new cost of every edge for inputed #graph.
	 * @return solution for RRIMST problem in a form of set of edges that represents a spanning tree for the #graph.
	 */
	EdgeSetIF * getSolution(GraphEdgeCostsIF* initialGraphCosts)
			throw (GraphExceptions::DisconnectedGraphException);

	/** @brief Return a solution for Recoverable Robust Incremental Minimum Spanning Tree problem
	 * for #graph.
	 *
	 * @return solution for RRIMST problem in a form of set of edges that represents a spanning tree for the #graph.
	 */
	EdgeSetIF * getSolution() throw (GraphExceptions::DisconnectedGraphException);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_RRIMSTSOLVER_RRIMSTSOLVERIF_HPP_ */
