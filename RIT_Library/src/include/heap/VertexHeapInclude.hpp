/*
 * VertexHeapInclude.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_HEAP_VERTEXHEAPINCLUDE_HPP_
#define SRC_INCLUDE_HEAP_VERTEXHEAPINCLUDE_HPP_

#define VertexHeapIF_FibonacciHeap

#include "VertexHeapIF.hpp"

#ifdef VertexHeapIF_FibonacciHeap
#include "FibonacciHeap.hpp"
/*
 #elif defined(VertexSetIF_VertexList)
 #include "VertexSet/VertexList.hpp"
 */
#endif

typedef FibonacciHeap VertexHeapImpl;

#endif /* SRC_INCLUDE_HEAP_VERTEXHEAPINCLUDE_HPP_ */
