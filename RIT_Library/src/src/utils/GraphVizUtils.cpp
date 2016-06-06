/*
 * GraphVizUtils.cpp
 *
 *  Created on: 27 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/GraphVizUtils.hpp"

#include <graphviz/cgraph.h>
#include <graphviz/gvc.h>
#include <graphviz/gvcext.h>
#include <log4cxx/logger.h>
#include <cmath>
#include <string>
#include <sstream>

#include "../../include/structures/GraphIF.hpp"
#include "../../include/utils/EnumUtils.hpp"
#include "../../include/utils/IOUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.GraphVizUtils"));

GraphVizUtils::VerticesCoordinates GraphVizUtils::getVerticesCoordinates(
		GraphIF * graph, GraphVizEngine layoutEngine,
		GraphDimmention graphMaxWidth, GraphDimmention graphMaxHeight) {
	GVC_t *gvc { };
	Agraph_t *g { };
	std::string dotFormatedGraph { };
	char * graphRenderedData { };
	unsigned int length { };
	VerticesCoordinates verticesCoordinates { };

	std::stringstream ss { };

	gvc = gvContext();

	dotFormatedGraph = OutputUtils::impl::DOT::exportGraph(graph);
	g = agmemread(dotFormatedGraph.c_str());

	gvLayout(gvc, g, EnumUtils::getGraphVizEngineString(layoutEngine));
	gvRenderData(gvc, g, "plain", &graphRenderedData, &length);

	ss << graphRenderedData;

	switch (layoutEngine) {
	case GraphVizEngine::NEATO:
		verticesCoordinates =
				GraphVizUtils::impl::getVerticesCoordinatesAsNeato(ss,
						graph->getNumberOfVertices(), graphMaxWidth,
						graphMaxHeight);
		break;
	default:
		verticesCoordinates =
						GraphVizUtils::impl::getVerticesCoordinatesAsNeato(ss,
								graph->getNumberOfVertices(), graphMaxWidth,
								graphMaxHeight);
	}

	gvFreeRenderData(graphRenderedData);
	gvFreeLayout(gvc, g);
	agclose(g);
	gvFreeContext(gvc);

	return verticesCoordinates;
}

GraphVizUtils::VerticesCoordinates GraphVizUtils::impl::getVerticesCoordinatesAsNeato(
		std::stringstream& dataStream, VertexCount const numberOfVertices,
		GraphDimmention graphMaxWidth, GraphDimmention graphMaxHeight) {
	VerticesCoordinates verticesCoordinates { };
	std::string controlString { };

	unsigned int i { };

	VertexCoordinate vertexXPosition { };
	VertexCoordinate vertexYPosition { };

	GraphDimmention centerWidth { graphMaxWidth / 2 };
	GraphDimmention centerHeight { graphMaxHeight / 2 };

	GraphDimmention graphWidth { 0 };
	GraphDimmention graphHeight { 0 };

	verticesCoordinates.reserve(numberOfVertices);

	dataStream.ignore(255, '\n');

	dataStream >> controlString;
	while (controlString.compare("node") == 0) {
		dataStream.ignore(255, ' ');
		dataStream.ignore(255, ' ');
		dataStream >> vertexXPosition >> vertexYPosition;

		if (graphWidth < std::abs(vertexXPosition)) {
			graphWidth = std::abs(vertexXPosition);
		}
		if (graphHeight < std::abs(vertexYPosition)) {
			graphHeight = std::abs(vertexYPosition);
		}
		verticesCoordinates.push_back(
				std::make_pair(vertexXPosition, vertexYPosition));

		dataStream.ignore(255, '\n');
		dataStream >> controlString;
	}

	for (i = 0; i < numberOfVertices; i += 1) {
		verticesCoordinates[i].first = centerWidth
				+ verticesCoordinates[i].first / graphWidth * centerWidth;
		verticesCoordinates[i].second = graphMaxHeight
				- (centerHeight
						+ verticesCoordinates[i].second / graphHeight
								* centerHeight);
	}
	return verticesCoordinates;
}
