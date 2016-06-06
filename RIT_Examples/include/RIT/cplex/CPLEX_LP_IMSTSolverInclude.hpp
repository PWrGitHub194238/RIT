/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_LP_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_CPLEX_LP_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_

#define CPLEX_LP_IMSTSolverIF_CPLEX_LP_IMSTSolver_v2

#include "CPLEX_LP_IMSTSolverIF.hpp"

#ifdef CPLEX_LP_IMSTSolverIF_CPLEX_LP_IMSTSolver_v2
#include "CPLEX_LP_IMSTSolver_v2.hpp"
/*
 #elif defined(EdgeSetIF_EdgeList)
 #include "EdgeSet/EdgeList.hpp"
 */
#endif

typedef CPLEX_LP_IMSTSolver_v2 CPLEX_LP_IMSTSolverImpl;

#endif /* SRC_INCLUDE_CPLEX_LP_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_ */
