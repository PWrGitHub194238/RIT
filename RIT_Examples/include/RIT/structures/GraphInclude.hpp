/*
 * GraphInclude.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_GRAPHINCLUDE_HPP_
#define INCLUDE_STRUCTURES_GRAPHINCLUDE_HPP_

#define GraphIF_Graph

#include "GraphIF.hpp"

#ifdef GraphIF_Graph
#include "Graph/Graph.hpp"
#endif

typedef Graph GraphImpl;

#endif /* INCLUDE_STRUCTURES_GRAPHINCLUDE_HPP_ */
