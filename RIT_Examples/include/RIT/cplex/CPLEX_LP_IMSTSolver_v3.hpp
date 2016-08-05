/*
 * CPLEX_LP_IMSTSolver_v3.hpp
 *
 *  Created on: 2 mar 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v3_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v3_HPP_

#include <ilconcert/iloenv.h>
#include <ilconcert/iloexpression.h>

#include "../typedefs/primitive.hpp"
#include "CPLEX_LP_IMSTSolverIF.hpp"
#include "CPLEX_Typedefs.hpp"

class MSTSolverIF;

class CPLEX_LP_IMSTSolver_v3: public CPLEX_LP_IMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	static const IloNumVar::Type EDGE_VAR_DEFAULT_TYPE { IloNumVar::Type::Float };

	//************************************** PRIVATE CLASS FIELDS **************************************//

	IloCommodityFlowVarMap commodityBasedflowVariablesMap;
	IloArray<IloNumVarArray> commodityBasedflowVariables;

	VertexIdx initialVertexIdx;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void buildModel(IncrementalParam k);

	/** Z początkowego s dla każdego innego z s wychodzi o 1 więcej niż wchodzi, rozsyła pakiety
	 *
	 */
	void generateMoveCommodityFromSourceConstraints();

	void generateMoveCommodityFromSourceConstraint(
			IloFlowVarMap& edgeVariableMapForCommodity);

	void generatePassCommodityConstraints();

	void generatePassCommodityConstraint(
			IloFlowVarMap& edgeVariableMapForCommodity,
			VertexIdx nodeCommodityIdx);

	void generatePassCommodity(IloFlowVarMap& edgeVariableMapForCommodity,
			VertexIdx transitNodeIdx);

	void generateStoreCommodityConstraints();

	void generateStoreCommodityConstraint(
			IloFlowVarMap& edgeVariableMapForCommodity,
			VertexIdx nodeCommodityIdx);

	void generateFlowConstraints();

	void generateMSTConstraint();

	/** Suma krawędzi poza bazowy rozwiązaniem nie może być większa niż k.
	 *
	 * @param k
	 */
	void generateIncrementalSolutionConstraints(IncrementalParam k);

	void generateGoal();

	void createUndirectedEdgeVariables(GraphIF * const graph);

	void createUndirectedEdgeVariables(GraphIF * const graph,
			IloNumVar::Type edgeVariablesType);

	EdgeSetIF* LP_Solve() throw (IloException);

	void generateFlowVariables();

	IloNumVar getFlowVariable(IloFlowVarMapEntry& targetVertexFlowVarMap,
			const VertexIdx& targetVertexIdx);

	IloNumVar getFlowVariable(
			IloTargetVertexFlowVarEntry& targetVertexFlowVarEntry);

	bool isEdgeToSourceExists(
			IloTargetVertexFlowVarMap& targetVertexFlowVarEntry);

	bool isEdgeExists(IloTargetVertexFlowVarMap& targetVertexFlowVarEntry,
			VertexIdx targetVertexIdx);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	CPLEX_LP_IMSTSolver_v3(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph, EdgeSetIF * baseSolution,
			IloNumVar::Type edgeVariablesType);

	CPLEX_LP_IMSTSolver_v3(GraphIF* const graph, EdgeSetIF * baseSolution,
			IloNumVar::Type edgeVariablesType);

	CPLEX_LP_IMSTSolver_v3(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph, EdgeSetIF * baseSolution);

	CPLEX_LP_IMSTSolver_v3(GraphIF* const graph, EdgeSetIF * baseSolution);

	CPLEX_LP_IMSTSolver_v3(CPLEX_LP_MSTSolverIF* const mstSolver,
			GraphIF* const graph);

	CPLEX_LP_IMSTSolver_v3(GraphIF* const graph);

	virtual ~CPLEX_LP_IMSTSolver_v3() {

	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_CPLEX_CPLEX_LP_IMSTSolver_v3_HPP_ */
