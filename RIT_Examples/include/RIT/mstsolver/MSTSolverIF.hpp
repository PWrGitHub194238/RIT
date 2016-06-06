/*
 * MSTSolverIF.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_MSTSOLVER_MSTSOLVERIF_HPP_
#define SRC_INCLUDE_MSTSOLVER_MSTSOLVERIF_HPP_

class VertexIF;
class EdgeSetIF;
class GraphIF;

#include "../exp/GraphExceptions.hpp"

class MSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	GraphIF * graph;

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** Zwraca zbiór krawędzi tworzących MST. Zaczyna konstruować je od wierzchołka initialVertex
	 *
	 * @param
	 * @return
	 */
	virtual EdgeSetIF * resolve(VertexIF * const initialVertex) = 0;

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	MSTSolverIF(GraphIF * const graph);

	// Empty virtual destructor for proper cleanup
	virtual ~MSTSolverIF() {

	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	EdgeSetIF * getMST() throw (GraphExceptions::DisconnectedGraphException);

	/** Temporary disconnects every edge that is not in given visibleSet
	 *
	 * @param visibleSet
	 * @return
	 */
	EdgeSetIF * getMST(EdgeSetIF* visibleSet)
			throw (GraphExceptions::DisconnectedGraphException);

	EdgeSetIF * getMST(VertexIF * const initialVertex)
			throw (GraphExceptions::DisconnectedGraphException);

	EdgeSetIF * getMST(VertexIF * const initialVertex, EdgeSetIF* visibleSet)
			throw (GraphExceptions::DisconnectedGraphException);

	//*************************************** GETTERS & SETTERS ****************************************//

	void setGraph(GraphIF* graph);
};

#endif /* SRC_INCLUDE_MSTSOLVER_MSTSOLVERIF_HPP_ */
