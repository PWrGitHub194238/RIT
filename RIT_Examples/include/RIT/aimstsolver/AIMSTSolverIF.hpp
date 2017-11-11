/*
 * AIMSTSolverIF.hpp
 *
 *  Created on: 13 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERIF_HPP_
#define SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERIF_HPP_

#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"

#include "../exp/GraphExceptions.hpp"

class MSTSolverIF;
class GraphIF;
class IMSTSolverIF;

/** @brief Interface for AIMST solvers.
 *
 * @details This class serves as the interface for any solvers that
 * deals with Adversarial Incremental Minimum Spanning Tree problem that
 * can be described as follows:
 *
 *  \f[
	\max_{\mathclap{\textbf{s}^{\prime} \in S}} \quad \quad \min_{\mathclap{T^{\prime} \in \mathcal{T} \left( G_{\textbf{s}^{\prime}}, T, k \right)}} \quad \quad \sum_{e^{\prime} \in T^{\prime}} c_{e^{\prime}}^{\textbf{s}^{\prime}}
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
 * General form for Adversarial problem can be described as:
 *
 *  \f[
  	  \max_{\mathclap{\textbf{s}^{\prime} \in S}} \min_{\mathclap{\textbf{y} \in S_{\textbf{x}}}} v \left( \textbf{y}, \textbf{s}^{\prime} \right)
  \f]
 *
 * Adversarial Incremental Minimum Spanning Tree problem is the main subproblem of Recoverable Robust Incremental Minimum Spanning Tree problem
 * and can be interpreted as follows:
 *
 * 	Given an adversarial scenario set: \f$ S \f$ from which the adversarial have to choose one scenario that results
 * 	in the highest value of AIMST subproblem (that is Incremental Minimum Spanning Tree problem), regardless
 * 	of chosen spanning tree.
 *
 */
class AIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	AIMSTSolution resolve(EdgeSetIF* const baseSolution);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	IMSTSolverEnum imstSolverType;
	MSTSolverEnum mstSolverType;
	GraphIF * graph;

	GraphEdgeCostsSet adversarialScenarioSet;
	IncrementalParam k;

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual AIMSTSolution resolveWithMultThreads(EdgeSetIF* const baseSolution) = 0;

	virtual AIMSTSolution resolveWithSingleThread(EdgeSetIF* const baseSolution) = 0;

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	AIMSTSolverIF(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	AIMSTSolverIF(IMSTSolverEnum imstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	AIMSTSolverIF(GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	virtual ~AIMSTSolverIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Maximum Spanning Tree
	 *
	 * @param initialGraphCosts
	 * @return
	 */
	AIMSTSolution getSolution(EdgeSetIF* const baseSolution,
			GraphEdgeCostsIF* initialGraphCosts)
					throw (GraphExceptions::DisconnectedGraphException);

	AIMSTSolution getSolution(EdgeSetIF* const baseSolution)
			throw (GraphExceptions::DisconnectedGraphException);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERIF_HPP_ */
