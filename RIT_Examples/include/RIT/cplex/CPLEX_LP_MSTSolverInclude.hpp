/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_LP_MSTSOLVER_IMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_CPLEX_LP_MSTSOLVER_IMSTSOLVERINCLUDE_HPP_

#define CPLEX_LP_MSTSolverIF_CPLEX_LP_MSTSolver_v3

#include "CPLEX_LP_MSTSolverIF.hpp"

#ifdef CPLEX_LP_MSTSolverIF_CPLEX_LP_MSTSolver_v3
#include "CPLEX_LP_MSTSolver_v3.hpp"
#elif defined(CPLEX_LP_MSTSolverIF_CPLEX_LP_MSTSolver_v2)
#include "CPLEX_LP_MSTSolver_v2.hpp"
#endif

typedef CPLEX_LP_MSTSolver_v3 CPLEX_LP_MSTSolverImpl;

#endif /* SRC_INCLUDE_CPLEX_LP_MSTSOLVER_IMSTSOLVERINCLUDE_HPP_ */
