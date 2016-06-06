/*
 * MSTSolverInclude.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_MSTSOLVER_MSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_MSTSOLVER_MSTSOLVERINCLUDE_HPP_

#define MSTSolverIF_PrimeHeap

#include "MSTSolverIF.hpp"

#ifdef MSTSolverIF_PrimeHeap
#include "PrimeHeap.hpp"
/*
 #elif defined(EdgeSetIF_EdgeList)
 #include "EdgeSet/EdgeList.hpp"
 */
#endif

typedef PrimeHeap MSTSolverImpl;

#endif /* SRC_INCLUDE_MSTSOLVER_MSTSOLVERINCLUDE_HPP_ */
