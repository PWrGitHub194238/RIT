/*
 * CPLEX_LP_MSTSolverIF.hpp
 *
 *  Created on: 22 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolverIF_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolverIF_HPP_

#include <ilconcert/iloenv.h>
#include <ilconcert/iloexpression.h>
#include <ilconcert/ilomodel.h>
#include <ilcplex/ilocplexi.h>
#include <string>

#include "../mstsolver/MSTSolverIF.hpp"
#include "../typedefs/primitive.hpp"
#include "CPLEX_Typedefs.hpp"

#include "../exp/ModelExceptions.hpp"

class EdgeIF;

/**
 * Z uwagi na metodę konstrukcji modelu, nie da się odpalić kilka razy getMST() na tym samym obiekcie, musi być za każdym rzem tworzony od nowa.
 */
class CPLEX_LP_MSTSolverIF: public MSTSolverIF {

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	virtual void buildModel() = 0;

	virtual void generateGoal() = 0;

	virtual EdgeSetIF* LP_Solve() throw (IloException) = 0;

	virtual IloEdgeValMap LP_RAW_Solve() throw (IloException) = 0;

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	IloEnv env;
	IloModel model;
	IloCplex cplex;

	IloEdgeVarMap edgeVariableMap;
	IloNumVarArray edgeVariableArray;

	VertexCount numberOfVertices;
	EdgeCount numberOfEdges;

	EdgeSetIF* solution;

	//************************************** PROTECTED FUNCTIONS ***************************************//

	EdgeSetIF* transformSolutionToEdgeSet();

	IloEdgeValMap transformSolutionToRawMap();

	EdgeIF* getEdge(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getEdgeVariable(const VertexIdx sourceVertexIdx,
			const VertexIdx targetVertexIdx);

	IloNumVar getEdgeVariable(VertexIF* const sourceVertex,
			VertexIF* const targetVertex);

	IloNumVar getEdgeVariable(const EdgeIF* edge);

	std::string getVariableName(IloNumVar const & variable);

	std::string getCPLEXStatus(IloCplex& cplex);

	/** inital vertex może być nullem, LP i tak na to nie patrzy
	 * W LP_* przeciążamy LP_Solve, a nie resolve.
	 *
	 * @param initialVertex
	 * @return
	 */
	EdgeSetIF * resolve(VertexIF * const initialVertex)
			throw (ModelExceptions::GeneralConcertException);

	/** Zwraca mapę 0/1.
	 *
	 * @param initialVertex
	 * @return
	 */
	IloEdgeValMap resolveRAW() throw (ModelExceptions::GeneralConcertException);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	CPLEX_LP_MSTSolverIF(GraphIF * graph, IloNumVar::Type edgeVariablesType);

	CPLEX_LP_MSTSolverIF(GraphIF * graph);

	virtual ~CPLEX_LP_MSTSolverIF() {
		env.end();
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_CPLEX_CPLEX_LP_MSTSolverIF_HPP_ */
