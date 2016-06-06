/*
 * EdgeSetIF.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../include/structures/EdgeSetIF.hpp"

#include <log4cxx/logger.h>
#include <rapidjson/rapidjson.h>
#include <list>
#include <sstream>

#include "../../include/structures/VertexIF.hpp"
#include "../../include/utils/EnumUtils.hpp"
#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.EdgeSetIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

EdgeIF* EdgeSetIF::findUnconnected(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if ((returnedEdge->getSourceVertex()->getVertexIdx() == sourceVertexIdx
				&& returnedEdge->getTargetVertex()->getVertexIdx()
						== targetVertexIdx)
				|| (returnedEdge->getSourceVertex()->getVertexIdx()
						== targetVertexIdx
						&& returnedEdge->getTargetVertex()->getVertexIdx()
								== sourceVertexIdx)) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findUnconnected(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if ((returnedEdge->getSourceVertex() == sourceVertex
				&& returnedEdge->getTargetVertex() == targetVertex)
				|| (returnedEdge->getSourceVertex() == targetVertex
						&& returnedEdge->getTargetVertex() == sourceVertex)) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findUndirected(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if ((returnedEdge->getSourceVertex()->getVertexIdx() == sourceVertexIdx
				&& returnedEdge->getTargetVertex()->getVertexIdx()
						== targetVertexIdx)
				|| (returnedEdge->getSourceVertex()->getVertexIdx()
						== targetVertexIdx
						&& returnedEdge->getTargetVertex()->getVertexIdx()
								== sourceVertexIdx)) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findUndirected(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if ((returnedEdge->getSourceVertex() == sourceVertex
				&& returnedEdge->getTargetVertex() == targetVertex)
				|| (returnedEdge->getSourceVertex() == targetVertex
						&& returnedEdge->getTargetVertex() == sourceVertex)) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findForward(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if (returnedEdge->getSourceVertex()->getVertexIdx() == sourceVertexIdx
				&& returnedEdge->getTargetVertex()->getVertexIdx()
						== targetVertexIdx) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findForward(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if (returnedEdge->getSourceVertex() == sourceVertex
				&& returnedEdge->getTargetVertex() == targetVertex) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF* EdgeSetIF::findBackward(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if (returnedEdge->getSourceVertex()->getVertexIdx() == targetVertexIdx
				&& returnedEdge->getTargetVertex()->getVertexIdx()
						== sourceVertexIdx) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();

}

EdgeIF* EdgeSetIF::findBackward(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if (returnedEdge->getSourceVertex() == targetVertex
				&& returnedEdge->getTargetVertex() == sourceVertex) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

EdgeSetIF::EdgeSetIF() :
		numberOfEdges { 0 } {
}

EdgeSetIF::EdgeSetIF(EdgeSetIF * edgeSetIF) :
		numberOfEdges { edgeSetIF->size() } {
}

EdgeSetIF::EdgeSetIF(EdgeCount numberOfEdges) {
	this->numberOfEdges = numberOfEdges;
}

// Empty virtual destructor for proper cleanup
EdgeSetIF::~EdgeSetIF() {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeIF* EdgeSetIF::find(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx, EdgeConnectionType connectionType)
				throw (LogicExceptions::EdgeNotFoundException) {
	switch (connectionType) {
	case EdgeConnectionType::UNCONNECTED:
		return findUnconnected(sourceVertexIdx, targetVertexIdx);
	case EdgeConnectionType::UNDIRECTED:
		return findUndirected(sourceVertexIdx, targetVertexIdx);
	case EdgeConnectionType::FORWARD:
		return findForward(sourceVertexIdx, targetVertexIdx);
	case EdgeConnectionType::BACKWARD:
		return findBackward(sourceVertexIdx, targetVertexIdx);
	default:
		throw LogicExceptions::EdgeNotFoundException();
	}
}

EdgeIF* EdgeSetIF::find(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	return find(sourceVertexIdx, targetVertexIdx,
			EdgeConnectionType::UNDIRECTED);
}

EdgeIF* EdgeSetIF::find(VertexIF* const sourceVertex,
		VertexIF* const targetVertex, EdgeConnectionType connectionType)
				throw (LogicExceptions::EdgeNotFoundException) {
	switch (connectionType) {
	case EdgeConnectionType::UNCONNECTED:
		return findUnconnected(sourceVertex, targetVertex);
	case EdgeConnectionType::UNDIRECTED:
		return findUndirected(sourceVertex, targetVertex);
	case EdgeConnectionType::FORWARD:
		return findForward(sourceVertex, targetVertex);
	case EdgeConnectionType::BACKWARD:
		return findBackward(sourceVertex, targetVertex);
	default:
		throw LogicExceptions::EdgeNotFoundException();
	}
}

EdgeIF* EdgeSetIF::find(VertexIF* const sourceVertex,
		VertexIF* const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	return find(sourceVertex, targetVertex, EdgeConnectionType::UNDIRECTED);
}

EdgeCount EdgeSetIF::size(Visibility const visibility) {
	EdgeCount setSize { 0 };
	begin();
	while (hasNext(visibility)) {
		setSize += 1;
		next();
	}
	return setSize;
}

EdgeCount EdgeSetIF::size(IteratorId const iteratorId,
		Visibility const visibility) {
	EdgeCount setSize { 0 };
	begin(iteratorId);
	while (hasNext(iteratorId, visibility)) {
		setSize += 1;
		next(iteratorId);
	}
	return setSize;
}

EdgeCount EdgeSetIF::size(Connectivity const connectivity) {
	EdgeCount setSize { 0 };
	begin();
	while (hasNext(connectivity)) {
		setSize += 1;
		next();
	}
	return setSize;
}

EdgeCount EdgeSetIF::size(IteratorId const iteratorId,
		Connectivity const connectivity) {
	EdgeCount setSize { 0 };
	begin(iteratorId);
	while (hasNext(iteratorId, connectivity)) {
		setSize += 1;
		next(iteratorId);
	}
	return setSize;
}

EdgeIF* EdgeSetIF::getEdgeByIdx(EdgeIdx const edgeIdx)
		throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * returnedEdge { };
	IteratorId edgeSetIterator = getIterator();
	begin(edgeSetIterator);
	while (hasNext(edgeSetIterator)) {
		returnedEdge = next(edgeSetIterator);
		if (returnedEdge->getEdgeIdx() == edgeIdx) {
			removeIterator(edgeSetIterator);
			return returnedEdge;
		}
	}
	removeIterator(edgeSetIterator);
	throw LogicExceptions::EdgeNotFoundException();
}

EdgeCost EdgeSetIF::getTotalEdgeCost(Visibility const visibility) {
	EdgeCost totalCost { 0 };
	begin();
	while (hasNext(visibility)) {
		totalCost += next()->getEdgeCost();
	}
	return totalCost;
}

ConnectivityList EdgeSetIF::storeConnectivity() {
	ConnectivityList connectivityList { };
	begin();
	while (hasNext()) {
		connectivityList.push_back(next()->getConnectionType());
	}
	return connectivityList;
}

ConnectivityList EdgeSetIF::storeConnectivity(IteratorId const iteratorId) {
	ConnectivityList connectivityList { };
	begin(iteratorId);
	while (hasNext(iteratorId)) {
		connectivityList.push_back(next(iteratorId)->getConnectionType());
	}
	return connectivityList;
}

void EdgeSetIF::restoreConnectivityAll(
		ConnectivityList const & connectivityList) {
	ConnectivityList::const_iterator itBegin = connectivityList.begin();
	this->begin();
	while (this->hasNext()) {
		this->next()->connect(*itBegin);
		++itBegin;
	}
}

void EdgeSetIF::restoreConnectivityAll(
		ConnectivityList const & connectivityList,
		IteratorId const iteratorId) {
	ConnectivityList::const_iterator itBegin = connectivityList.begin();
	this->begin(iteratorId);
	while (this->hasNext(iteratorId)) {
		this->next(iteratorId)->connect(*itBegin);
		++itBegin;
	}
}

void EdgeSetIF::disconnectAll() {
	begin();
	while (hasNext()) {
		next()->disconnect();
	}
}

void EdgeSetIF::disconnectAll(IteratorId const iteratorId) {
	begin(iteratorId);
	while (hasNext(iteratorId)) {
		next(iteratorId)->disconnect();
	}
}

EdgeCost EdgeSetIF::getTotalEdgeCost() {
	return getTotalEdgeCost(Visibility::BOTH);
}

void EdgeSetIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	rapidjson::Value edgeSet(rapidjson::kArrayType);

	jsonDoc.AddMember("Number of edges", numberOfEdges, allocator);

	begin();
	while (hasNext()) {
		edgeSet.PushBack(
				JSONUtils::getDepthLimitedJSON(next(), allocator, "EdgeIF",
						depth), allocator);
	}

	jsonDoc.AddMember("Edge set", edgeSet, allocator);
}

std::string EdgeSetIF::toString() {
	return toString(Visibility::BOTH);
}

std::string EdgeSetIF::toString(Visibility edgeVisibility) {
	std::ostringstream oss { };
	oss << "Set of edges have " << numberOfEdges << " edges (print "
			<< EnumUtils::getVisibilityString(edgeVisibility) << " edges):"
			<< std::endl;

	begin();
	while (hasNext(edgeVisibility)) {
		oss << "\t" << next()->toString() << std::endl;
	}
	begin();
	return oss.str();
}

//*************************************** GETTERS & SETTERS ****************************************//

