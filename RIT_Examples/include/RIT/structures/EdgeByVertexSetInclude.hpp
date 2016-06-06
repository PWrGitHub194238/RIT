/*
 * EdgeByVertexSetInclude.hpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSETINCLUDE_HPP_
#define SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSETINCLUDE_HPP_

#define EdgeByVertexSetIF_EdgeByVertexMap

#include "EdgeByVertexSetIF.hpp"

#ifdef EdgeByVertexSetIF_EdgeByVertexMap
#include "EdgeByVertexSet/EdgeByVertexMap.hpp"
/*
#elif defined(EdgeByVertexSetIF_EdgeList)
#include "EdgeByVertexSet/EdgeList.hpp"
*/
#endif

typedef EdgeByVertexMap EdgeByVertexSetImpl;

#endif /* SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSETINCLUDE_HPP_ */
