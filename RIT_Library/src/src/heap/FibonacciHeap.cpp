/*
 * FibonacciHeap.cpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#include "../../include/heap/FibonacciHeap.hpp"

#include <boost/heap/detail/stable_heap.hpp>
#include <memory>
#include <string>
#include <utility>

#include "../../include/structures/VertexIF.hpp"

class VertexIF;

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>

#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("heap.FibonacciHeap"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

FibonacciHeap::FibonacciHeap() :
		VertexHeapIF() {
}

FibonacciHeap::~FibonacciHeap() {
	while (!this->heap.empty()) {
		pop();
	}
	heapMap.clear();
	heap.clear();
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

VertexCount FibonacciHeap::size() const {
	return (VertexCount) this->heap.size();
}

void FibonacciHeap::push(VertexHeapItem * const vertex) {
	TRACE(logger, LogBundleKey::VERTEX_PUSHED_INTO_FIB_HEAP,
			vertex->toString().c_str());
	this->heapMap.insert(
			std::make_pair(vertex->getValue()->getVertexIdx(),
					this->heap.push(vertex)));
}

VertexIF * FibonacciHeap::pop() {
	VertexHeapItem * item = this->heap.top();
	VertexIF * vertex = item->getValue();
	this->heapMap.erase(item->getValue()->getVertexIdx());
	delete item;
	this->heap.pop();
	return vertex;
}

VertexHeapItem * FibonacciHeap::peek() {
	return this->heap.top();
}

void FibonacciHeap::increaseKey(VertexHeapItem * heapItem, VertexKey newKey) {
	this->heap.increase(this->heapMap.at(heapItem->getValue()->getVertexIdx()));
}

void FibonacciHeap::decreaseKey(VertexHeapItem * heapItem, VertexKey newKey) {
	this->heap.decrease(this->heapMap.at(heapItem->getValue()->getVertexIdx()));
}

VertexHeapItem * FibonacciHeap::getItem(VertexIdx const vertexId) {
	return (isVertexInsideHeap(vertexId)) ?
			*(this->heapMap.at(vertexId)) : nullptr;
}

bool FibonacciHeap::isVertexInsideHeap(VertexIF * const vertex) {
	return isVertexInsideHeap(vertex->getVertexIdx());
}

bool FibonacciHeap::isVertexInsideHeap(VertexIdx const vertexId) {
	return this->heapMap.count(vertexId);
}

//*************************************** GETTERS & SETTERS ****************************************//

