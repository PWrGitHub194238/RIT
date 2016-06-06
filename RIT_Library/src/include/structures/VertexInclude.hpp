/*
 * VertexInclude.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXINCLUDE_HPP_
#define INCLUDE_STRUCTURES_VERTEXINCLUDE_HPP_

#define VertexIF_Vertex

#include "VertexIF.hpp"

#ifdef VertexIF_Vertex
#include "Vertex/Vertex.hpp"
#endif

typedef Vertex VertexImpl;

#endif /* INCLUDE_STRUCTURES_VERTEXINCLUDE_HPP_ */
