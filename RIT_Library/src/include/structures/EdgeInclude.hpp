/*
 * EdgeInclude.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_EDGEINCLUDE_HPP_
#define INCLUDE_STRUCTURES_EDGEINCLUDE_HPP_

#define EdgeIF_Edge

#include "EdgeIF.hpp"

#ifdef EdgeIF_Edge
#include "Edge/Edge.hpp"
#endif

typedef Edge EdgeImpl;

#endif /* INCLUDE_STRUCTURES_EDGEINCLUDE_HPP_ */
