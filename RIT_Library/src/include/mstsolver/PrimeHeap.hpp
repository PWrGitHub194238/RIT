/*
 * PrimeHeap.hpp
 *
 *  Created on: 1 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_MSTSOLVER_PRIMEHEAP_HPP_
#define INCLUDE_MSTSOLVER_PRIMEHEAP_HPP_

#include "MSTSolverIF.hpp"

class VertexIF;

class VertexHeapIF;

class PrimeHeap: public MSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Returns heap with all vertexes except first one in graph.
	 * It is assumed that it will be immediately pop from heap
	 * so there is no reason to push it in a first place.
	 *
	 * All neighbours of given initVertex will be added to heap with edge cost instead of infinity
	 *
	 * @param graph
	 * @return
	 */
	VertexHeapIF* createEdgeHeap(GraphIF * const graph,
			VertexIF * const initVertex);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	/** Chowa wszystkie krawędzie, bada po kolei wierzchołki, dodając krawędż do MST
	 * odsłania ją, nastepnie analizuje wyjściowe krawędzie nowych wierzchołków, które
	 * są ukryte (nie ma ich w MST).
	 *
	 * @param initialVertex
	 * @return
	 */
	EdgeSetIF * resolve(VertexIF * const initialVertex);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	PrimeHeap(GraphIF * graph);

	virtual ~PrimeHeap() {

	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_MSTSOLVER_PRIMEHEAP_HPP_ */
