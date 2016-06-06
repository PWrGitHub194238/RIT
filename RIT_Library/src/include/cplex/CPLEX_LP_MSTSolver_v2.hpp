/*
 * CPLEX_LP_MSTSolver_v2.hpp
 *
 *  Created on: 2 mar 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolver_v2_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolver_v2_HPP_

#include <ilconcert/iloexpression.h>

#include "../typedefs/primitive.hpp"
#include "CPLEX_LP_MSTSolverIF.hpp"
#include "CPLEX_Typedefs.hpp"

class CPLEX_LP_MSTSolver_v2: public CPLEX_LP_MSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	IloFlowVarMap flowVariablesMap;
	IloNumVarArray flowVariables;

	VertexIdx initialVertexIdx;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void buildModel();

	void generateFlowConstraints();

	void generateFlowConstraint(VertexIF* const vertex, int cost);

	void generateFlowBoundsConstraints();

	void generateMSTConstraint();

	void generateGoal();

	EdgeSetIF* LP_Solve() throw (IloException);

	IloEdgeValMap LP_RAW_Solve() throw (IloException);

	void createEdgeVariables(GraphIF * const graph);

	void createEdgeVariables(GraphIF * const graph,
			IloNumVar::Type edgeVariablesType);

	void generateFlowVariables();

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	CPLEX_LP_MSTSolver_v2(GraphIF * graph);

	virtual ~CPLEX_LP_MSTSolver_v2() {

	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolver_v2_HPP_ */
