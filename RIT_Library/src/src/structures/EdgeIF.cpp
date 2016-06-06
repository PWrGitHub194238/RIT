/*
 * EdgeIF.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../include/structures/EdgeIF.hpp"

#include <log4cxx/logger.h>
#include <sstream>
#include <utility>

#include "../../include/structures/VertexIF.hpp"
#include "../../include/utils/EnumUtils.hpp"
#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.EdgeIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void EdgeIF::connectUndirected() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->addInputEdge(this);
	v1->addOutputEdge(this);
	v2->addInputEdge(this);
	v2->addOutputEdge(this);
}

void EdgeIF::connectForward() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->addOutputEdge(this);
	v2->addInputEdge(this);
}

void EdgeIF::connectBackward() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->addInputEdge(this);
	v2->addOutputEdge(this);
}

void EdgeIF::disconnectUndirected() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->removeInputEdge(v2);
	v1->removeOutputEdge(v2);
	v2->removeInputEdge(v1);
	v2->removeOutputEdge(v1);
}

void EdgeIF::disconnectForward() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->removeOutputEdge(v2);
	v2->removeInputEdge(v1);
}

void EdgeIF::disconnectBackward() {
	VertexIF* v1 = this->edgeConnection.first;
	VertexIF* v2 = this->edgeConnection.second;
	v1->removeInputEdge(v2);
	v2->removeOutputEdge(v1);
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

EdgeIF::EdgeIF(EdgeIF * edge) :
		EdgeIF(edge->getEdgeIdx(),
				VertexPair(edge->getSourceVertex(), edge->getTargetVertex()),
				edge->getEdgeCost(), edge->getConnectionType(),
				edge->getVisibility()) {

}

EdgeIF::EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost,
				EdgeConnectionType::UNDIRECTED, Visibility::VISIBLE) {
}

EdgeIF::EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, Visibility visibility) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost,
				EdgeConnectionType::UNDIRECTED, visibility) {
}

EdgeIF::EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, EdgeConnectionType connectionType) :
		EdgeIF(edgeIdx, edgeConnections, edgeCost, connectionType,
				Visibility::VISIBLE) {
}

EdgeIF::EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
		EdgeCost const edgeCost, EdgeConnectionType connectionType,
		Visibility visibility) {
	this->edgeIdx = edgeIdx;
	this->edgeConnection = edgeConnections;
	this->edgeCost = edgeCost;
	this->visibility = visibility;
	connect(connectionType);
}

// Empty virtual destructor for proper cleanup
EdgeIF::~EdgeIF() {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void EdgeIF::connect(EdgeConnectionType connectionType) {
	this->connectionType = connectionType;
	switch (connectionType) {
	case EdgeConnectionType::UNDIRECTED:
		connectUndirected();
		break;
	case EdgeConnectionType::FORWARD:
		connectForward();
		break;
	case EdgeConnectionType::BACKWARD:
		connectBackward();
		break;
	default:
		break;
	}
}

void EdgeIF::disconnect() {
	switch (connectionType) {
	case EdgeConnectionType::UNDIRECTED:
		disconnectUndirected();
		break;
	case EdgeConnectionType::FORWARD:
		disconnectForward();
		break;
	case EdgeConnectionType::BACKWARD:
		disconnectBackward();
		break;
	default:
		break;
	}
	this->connectionType = EdgeConnectionType::UNCONNECTED;
}

bool EdgeIF::isInState(Connectivity connectivity) const {
	switch (connectivity) {
	case Connectivity::BOTH:
		return true;
	case Connectivity::CONNECTED:
		return connectionType == EdgeConnectionType::BACKWARD
				|| connectionType == EdgeConnectionType::FORWARD
				|| connectionType == EdgeConnectionType::UNDIRECTED;
	case Connectivity::DISCONNECTED:
		return connectionType == EdgeConnectionType::UNCONNECTED;
	default:
		return false;
	}
}

void EdgeIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	jsonDoc.AddMember("Edge cost", edgeCost, allocator);
	jsonDoc.AddMember("Connection type",
			rapidjson::StringRef(
					EnumUtils::getEdgeConnectionTypeString(connectionType)),
			allocator);
	jsonDoc.AddMember("Is hidden?",
			rapidjson::StringRef(EnumUtils::getVisibilityString(visibility)),
			allocator);

	jsonDoc.AddMember("u",
			JSONUtils::getDepthLimitedJSON(edgeConnection.first, allocator,
					"VertexIF", depth), allocator);
	jsonDoc.AddMember("v",
			JSONUtils::getDepthLimitedJSON(edgeConnection.second, allocator,
					"VertexIF", depth), allocator);
}

std::string EdgeIF::toString() const {
	std::ostringstream oss { };
	oss << getSourceVertex()->toString() << "\t"
			<< ((connectionType == EdgeConnectionType::UNDIRECTED
					|| connectionType == EdgeConnectionType::BACKWARD) ?
					"<" : "-") << ((isHidden()) ? "- - - -" : "-------")
			<< " [\t" << edgeCost << "\t] "
			<< ((isHidden()) ? "- - - -" : "-------")
			<< ((connectionType == EdgeConnectionType::UNDIRECTED
					|| connectionType == EdgeConnectionType::FORWARD) ?
					">" : "-") << "\t" << getTargetVertex()->toString()
			<< std::flush;

	return oss.str();
}

//*************************************** GETTERS & SETTERS ****************************************//

EdgeIdx EdgeIF::getEdgeIdx() const {
	return this->edgeIdx;
}

EdgeCost EdgeIF::getEdgeCost() const {
	return this->edgeCost;
}

void EdgeIF::setEdgeCost(EdgeCost const edgeCost) {
	this->edgeCost = edgeCost;
}

VertexIF * EdgeIF::getSourceVertex() const {
	return (connectionType != EdgeConnectionType::BACKWARD) ?
			this->edgeConnection.first : this->edgeConnection.second;
}

VertexIF * EdgeIF::getTargetVertex() const {
	return (connectionType != EdgeConnectionType::BACKWARD) ?
			this->edgeConnection.second : this->edgeConnection.first;
}

VertexIF * EdgeIF::getOtherVertex(VertexIF* vertex) const {
	return (this->getTargetVertex() == vertex) ?
			this->getSourceVertex() : this->getTargetVertex();
}

VertexIF * EdgeIF::getOtherVertex(VertexIdx vertexIdx) const {
	return (this->getTargetVertex()->getVertexIdx() == vertexIdx) ?
			this->getSourceVertex() : this->getTargetVertex();
}

EdgeConnectionType EdgeIF::getConnectionType() const {
	return connectionType;
}
