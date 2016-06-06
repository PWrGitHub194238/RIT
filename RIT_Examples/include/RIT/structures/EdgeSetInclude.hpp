/*
 * EdgeSetInclude.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_EDGESETINCLUDE_HPP_
#define INCLUDE_STRUCTURES_EDGESETINCLUDE_HPP_

#define EdgeSetIF_EdgeArray

#include "EdgeSetIF.hpp"

#ifdef EdgeSetIF_EdgeArray
#include "EdgeSet/EdgeArray.hpp"
#elif defined(EdgeSetIF_EdgeList)
#include "EdgeSet/EdgeList.hpp"
#endif

typedef EdgeArray EdgeSetImpl;

#endif /* INCLUDE_STRUCTURES_EDGESETINCLUDE_HPP_ */
