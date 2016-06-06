/*
 * CPLEX_LP_IMSTSolver_v2.hpp
 *
 *  Created on: 2 mar 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v2_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v2_HPP_

#include <ilconcert/iloexpression.h>

#include "../typedefs/primitive.hpp"
#include "CPLEX_LP_IMSTSolverIF.hpp"

class MSTSolverIF;

class CPLEX_LP_IMSTSolver_v2: public CPLEX_LP_IMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	IloFlowVarMap flowVariablesMap;
	IloNumVarArray flowVariables;

	VertexIdx initialVertexIdx;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void buildModel(IncrementalParam k);

	void generateFlowConstraints();

	void generateFlowConstraint(VertexIF* const vertex, int cost);

	void generateFlowBoundsConstraints();

	void generateMSTConstraint();

	void generateIncrementalSolutionConstraints(IncrementalParam k);

	void generateAddDropVariablesConstraint(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx);

	void generateIncrementalSolutionConstraint(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx);

	/** Sum[|z^{+}_{i} - z^{-}_{i}|, {i,1,|V|-1}] ==Sum[z^{+}_{i} + z^{-}_{i}, {i,1,|V|-1}]
	 * ---> Sum[z^{+}_{i}, {i,1,|V|-1}] + Sum[z^{-}_{i}, {i,1,|V|-1}]
	 *
	 */
	void generateIncrementalConstraint(IncrementalParam k);

	void generateGoal();

	EdgeSetIF* LP_Solve() throw (IloException);

	void createEdgeVariables(GraphIF * const graph);

	void createEdgeVariables(GraphIF * const graph,
			IloNumVar::Type edgeVariablesType);

	void createEdgeAddDropVariables(GraphIF * const graph,
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

	CPLEX_LP_IMSTSolver_v2(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph, EdgeSetIF * baseSolution,
			IloNumVar::Type edgeVariablesType);

	CPLEX_LP_IMSTSolver_v2(GraphIF* const graph, EdgeSetIF * baseSolution,
			IloNumVar::Type edgeVariablesType);

	CPLEX_LP_IMSTSolver_v2(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph, EdgeSetIF * baseSolution);

	CPLEX_LP_IMSTSolver_v2(GraphIF* const graph, EdgeSetIF * baseSolution);

	CPLEX_LP_IMSTSolver_v2(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph);

	CPLEX_LP_IMSTSolver_v2(GraphIF* const graph);

	virtual ~CPLEX_LP_IMSTSolver_v2() {

	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v2_HPP_ */
