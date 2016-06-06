/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_RIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_RIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_

#define RIMSTSolverIF_TabuSearch

#include "RIMSTSolverIF.hpp"

#ifdef RIMSTSolverIF_TabuSearch
#include "TabuSearch.hpp"
/*
 #elif defined(EdgeSetIF_EdgeList)
 #include "EdgeSet/EdgeList.hpp"
 */
#endif

typedef TabuSearch RIMSTSolverImpl;

#endif /* SRC_INCLUDE_RIMSTSOLVER_IMSTSOLVERINCLUDE_HPP_ */
