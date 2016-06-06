/*
 * Graph.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/Graph/Graph.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>

#include "../../../include/enums/EdgeConnectionType.hpp"
#include "../../../include/log/bundle/Bundle.hpp"
#include "../../../include/log/utils/LogUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.Graph"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

Graph::Graph(GraphIF * const graph) : GraphIF(graph) {

}

Graph::Graph(VertexCount const vertexCount, EdgeCount const edgeCount,
		GraphConstructMode constructorMode) :
		GraphIF(vertexCount, edgeCount, constructorMode) {
	INFO(logger, LogBundleKey::GRAPH_CONSTRUCTOR, vertexCount, edgeCount);
}

Graph::Graph(VertexCount const vertexCount, EdgeCount const edgeCount) :
		GraphIF(vertexCount, edgeCount) {
}

Graph::Graph(VertexSetIF * const & vertexSet, EdgeSetIF * const & edgeSet) :
		GraphIF(vertexSet, edgeSet) {
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void Graph::addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
		VertexIdx const vertexIdxV, EdgeCost const edgeCost) {
	INFO(logger, LogBundleKey::GRAPH_EDGE_ADDED, vertexIdxU, vertexIdxV,
			edgeCost);
	GraphIF::addEdge(edgeIdx, vertexIdxU, vertexIdxV, edgeCost,
			EdgeConnectionType::UNDIRECTED);
}

//*************************************** GETTERS & SETTERS ****************************************//

