/*
 * Edge.cpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#include "../../../include/structures/Edge/Edge.hpp"

#include <log4cxx/logger.h>

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.Edge"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

Edge::Edge(EdgeIF * edge) :
		EdgeIF(edge) {

}

Edge::Edge(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, EdgeConnectionType connection,
		Visibility visibility) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost, connection, visibility) {
}

Edge::Edge(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, EdgeConnectionType connection) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost, connection) {
}

Edge::Edge(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, Visibility visibility) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost, visibility) {
	// TODO Auto-generated constructor stub
}

Edge::Edge(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost) {
	// TODO Auto-generated constructor stub
}

Edge::~Edge() {
	// TODO Auto-generated destructor stub
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

EdgeCost Edge::getEdgeCost() const {
	return this->edgeCost;
}
