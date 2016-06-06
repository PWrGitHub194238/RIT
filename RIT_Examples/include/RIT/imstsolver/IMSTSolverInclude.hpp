/*
 * IMSTSolverInclude.hpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_
#define SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_

#define IMSTSolverIF_BinarySearch_v2

#include "IMSTSolverIF.hpp"

#ifdef IMSTSolverIF_BinarySearch_v1
#include "BinarySearch_v1.hpp"
#elif defined(IMSTSolverIF_BinarySearch_v2)
#include "BinarySearch_v2.hpp"
#endif

typedef BinarySearch_v2 IMSTSolverImpl;

#endif /* SRC_INCLUDE_IMSTSOLVER_IMSTSOLVERINCLUDE_HPP_ */
