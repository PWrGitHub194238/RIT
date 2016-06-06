/*
 * FibonacciHeap.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_HEAP_FIBONACCIHEAP_HPP_
#define SRC_INCLUDE_HEAP_FIBONACCIHEAP_HPP_

#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/policies.hpp>
#include <map>

#include "../typedefs/primitive.hpp"
#include "VertexHeapIF.hpp"
#include "VertexHeapItem.hpp"

struct compare_node {
	bool operator()(VertexHeapItem * const & vertexU,
			VertexHeapItem * const & vertexV) const {
		return vertexU->getKey() > vertexV->getKey();
	}
};

class FibonacciHeap: public VertexHeapIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	boost::heap::fibonacci_heap<VertexHeapItem *,
			boost::heap::compare<compare_node>> heap;
	std::map<VertexIdx,
			boost::heap::fibonacci_heap<VertexHeapItem *,
					boost::heap::compare<compare_node>>::handle_type> heapMap;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	FibonacciHeap();

	virtual ~FibonacciHeap();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	VertexCount size() const;

	void push(VertexHeapItem * const vertex);

	VertexIF * pop();

	VertexHeapItem * peek();

	void increaseKey(VertexHeapItem * heapItem, VertexKey newKey);

	void decreaseKey(VertexHeapItem * heapItem, VertexKey newKey);

	VertexHeapItem * getItem(VertexIdx const vertexId);

	bool isVertexInsideHeap(VertexIF * const vertex);

	bool isVertexInsideHeap(VertexIdx const vertexId);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_HEAP_FIBONACCIHEAP_HPP_ */
