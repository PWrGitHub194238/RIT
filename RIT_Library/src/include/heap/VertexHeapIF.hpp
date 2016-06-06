/*
 * VertexHeapIF.hpp
 *
 *  Created on: 1 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_VERTEX_HEAP_HEAPIF_HPP_
#define INCLUDE_VERTEX_HEAP_HEAPIF_HPP_

#include "../typedefs/primitive.hpp"

class VertexHeapItem;

class VertexIF;

class VertexHeapIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	virtual ~VertexHeapIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual bool compare(VertexHeapItem * const vertexU,
			VertexHeapItem * const vertexV);

	virtual VertexCount size() const = 0;

	virtual void push(VertexHeapItem * const vertex) = 0;

	virtual VertexIF * pop() = 0;

	virtual VertexHeapItem * peek() = 0;

	virtual void increaseKey(VertexHeapItem * heapItem, VertexKey newKey) = 0;

	virtual void decreaseKey(VertexHeapItem * heapItem, VertexKey newKey) = 0;

	/** Zwraca item w kompcu lub nullptr, jeśli nie ma
	 *
	 * @param vertexId
	 * @return
	 */
	virtual VertexHeapItem * getItem(VertexIdx const vertexId) = 0;

	virtual bool isVertexInsideHeap(VertexIF * const vertex) = 0;

	virtual bool isVertexInsideHeap(VertexIdx const vertexId) = 0;

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_VERTEX_HEAP_HEAPIF_HPP_ */
