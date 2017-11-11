/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_RRIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_RRIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_

#define RRIMSTSolverIF_TabuSearch

#include "RRIMSTSolverIF.hpp"

#ifdef RRIMSTSolverIF_TabuSearch
#include "../rrimstsolver/TabuSearch.hpp"
/*
 #elif defined(EdgeSetIF_EdgeList)
 #include "EdgeSet/EdgeList.hpp"
 */
#endif

typedef TabuSearch RRIMSTSolverImpl;

#endif /* SRC_INCLUDE_RRIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_ */
