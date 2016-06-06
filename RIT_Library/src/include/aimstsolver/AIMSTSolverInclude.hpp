/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERINCLUDE_HPP_

#define AIMSTSolverIF_AIMSTSolver

#include "AIMSTSolverIF.hpp"

#ifdef AIMSTSolverIF_AIMSTSolver
#include "AIMSTSolver.hpp"
/*
 #elif defined(EdgeSetIF_EdgeList)
 #include "EdgeSet/EdgeList.hpp"
 */
#endif

typedef AIMSTSolver AIMSTSolverImpl;

#endif /* SRC_INCLUDE_AIMSTSOLVER_AIMSTSOLVERINCLUDE_HPP_ */
