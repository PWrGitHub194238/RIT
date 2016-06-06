/*
 * VertexSetInclude.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXSETINCLUDE_HPP_
#define INCLUDE_STRUCTURES_VERTEXSETINCLUDE_HPP_

#define VertexSetIF_VertexArray

#include "VertexSetIF.hpp"

#ifdef VertexSetIF_VertexArray
#include "VertexSet/VertexArray.hpp"
#elif defined(VertexSetIF_VertexList)
#include "VertexSet/VertexList.hpp"
#endif

typedef VertexArray VertexSetImpl;

#endif /* INCLUDE_STRUCTURES_VERTEXSETINCLUDE_HPP_ */
