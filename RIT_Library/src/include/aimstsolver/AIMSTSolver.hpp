/*
 * AIMSTSolver.hpp
 *
 *  Created on: 14 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVER_HPP_
#define SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVER_HPP_

#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "AIMSTSolverIF.hpp"

class MSTSolverIF;

class GraphIF;
class IMSTSolverIF;

class AIMSTSolver: public AIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Zwraca rozwiązanie problemu IMST dla bazowego drzewa baseSolution po tym jak koszty grafu
	 * zmienią się zgodnie z danym scenariuszem scenario. Rozwiązanie nie będzie różniło się
	 * o więcej niż globalny paramtr k.
	 *
	 * @param threadId
	 * @param graph
	 * @param scenario
	 * @param baseSolution
	 * @param outputSolution
	 */
	void getIMST(ThreadId threadId, GraphIF* const graph,
			EdgeSetIF* const baseSolution, GraphEdgeCostsIF* scenario,
			EdgeSetIF*& outputSolution);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual AIMSTSolution resolve(EdgeSetIF* const baseSolution);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	AIMSTSolver(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	AIMSTSolver(IMSTSolverEnum imstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	AIMSTSolver(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	virtual ~AIMSTSolver() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVER_HPP_ */
