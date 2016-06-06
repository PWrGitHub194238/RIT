/*
 * GraphVizUtils.hpp
 *
 *  Created on: 27 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_GRAPHVIZUTILS_HPP_
#define SRC_INCLUDE_UTILS_GRAPHVIZUTILS_HPP_

#include <iostream>
#include <utility>
#include <vector>

#include "../typedefs/primitive.hpp"
#include "enums/GraphVizEngine.hpp"

class GraphIF;

namespace GraphVizUtils {

typedef double GraphDimmention;
typedef double VertexCoordinate;
typedef double GraphScale;
typedef std::pair<VertexCoordinate, VertexCoordinate> VertexCoordinates;
typedef std::vector<VertexCoordinates> VerticesCoordinates;

VerticesCoordinates getVerticesCoordinates(GraphIF * graph,
		GraphVizEngine layoutEngine, GraphDimmention graphMaxWidth,
		GraphDimmention graphMaxHeight);

namespace impl {

VerticesCoordinates getVerticesCoordinatesAsNeato(std::stringstream& dataStream,
		VertexCount const numberOfVertices, GraphDimmention graphMaxWidth,
		GraphDimmention graphMaxHeight);

}  // namespace impl

}  // namespace GraphVizUtils

#endif /* SRC_INCLUDE_UTILS_GRAPHVIZUTILS_HPP_ */
