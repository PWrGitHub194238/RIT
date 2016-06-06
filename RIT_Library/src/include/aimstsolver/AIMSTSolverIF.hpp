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

class AIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	IMSTSolverEnum imstSolverType;
	MSTSolverEnum mstSolverType;
	GraphIF * graph;

	GraphEdgeCostsSet adversarialScenarioSet;
	IncrementalParam k;

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual AIMSTSolution resolve(EdgeSetIF* const baseSolution) = 0;

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
	AIMSTSolution getMST(EdgeSetIF* const baseSolution,
			GraphEdgeCostsIF* initialGraphCosts)
					throw (GraphExceptions::DisconnectedGraphException);

	AIMSTSolution getMST(EdgeSetIF* const baseSolution)
			throw (GraphExceptions::DisconnectedGraphException);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERIF_HPP_ */
