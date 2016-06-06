/*
 * GraphIF.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include "../../include/structures/GraphIF.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>
#include <sstream>
#include <utility>

#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeInclude.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/VertexInclude.hpp"
#include "../../include/structures/VertexSetInclude.hpp"
#include "../../include/utils/JSONUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.GraphIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

GraphIF::GraphIF(GraphIF * graph) {
	EdgeIF* edge { };
	EdgeCount edgeIdx { 0 };
	IteratorId vertexIterator = graph->getVertexIteratorId();
	IteratorId edgeIterator = graph->getEdgeIteratorId();

	this->vertexSet = new VertexSetImpl { graph->getNumberOfVertices() };
	graph->beginVertex(vertexIterator);
	while (graph->hasNextVertex(vertexIterator)) {
		this->vertexSet->push_back(
				new VertexImpl { graph->nextVertex(vertexIterator) });
	}

	this->edgeSet = new EdgeSetImpl { graph->getNumberOfEdges() };

	graph->beginEdge(edgeIterator);
	while (graph->hasNextEdge(edgeIterator)) {
		edge = graph->nextEdge(edgeIterator);
		this->edgeSet->push_back(
				new EdgeImpl { edgeIdx, VertexPair(
						vertexSet->getElementAt(
								edge->getSourceVertex()->getVertexIdx()),
						vertexSet->getElementAt(
								edge->getTargetVertex()->getVertexIdx())),
						edge->getEdgeCost(), edge->getConnectionType(),
						edge->getVisibility() });
		edgeIdx += 1;
	}
	graph->removeVertexIterator(vertexIterator);
	graph->removeEdgeIterator(edgeIterator);
}

GraphIF::GraphIF(VertexCount const vertexCount, EdgeCount const edgeCount,
		GraphConstructMode constructMode) {
	TRACE(logger, LogBundleKey::GRAPH_IF_CONSTRUCTOR, vertexCount, edgeCount);
	VertexCount i { 0 };
	this->vertexSet = new VertexSetImpl { vertexCount };
	this->edgeSet = new EdgeSetImpl { edgeCount };
	if (constructMode == GraphConstructMode::AUTO_CONSTRUCT_VERTEX) {
		for (; i < vertexCount; i += 1) {
			this->addVertex(new VertexImpl { i });
		}
	}
}

GraphIF::GraphIF(VertexCount const vertexCount, EdgeCount const edgeCount) :
		GraphIF(vertexCount, edgeCount, GraphConstructMode::RESERVE_SPACE_ONLY) {
}

GraphIF::GraphIF(VertexSetIF * const & vertexSet, EdgeSetIF * const & edgeSet) {
	this->vertexSet = vertexSet;
	this->edgeSet = edgeSet;
}

// USUWAĆ PRZEZ MemoryUtils::removeGraph(g);
GraphIF::~GraphIF() {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

void GraphIF::addVertex(VertexIF* const vertex) {
	INFO(logger, LogBundleKey::VERTEX_TO_GRAPH_IF_ADDED, vertex->getVertexIdx());
	vertexSet->push_back(vertex);
}

void GraphIF::addEdge(EdgeIF* const edge) {
	INFO(logger, LogBundleKey::EDGE_TO_GRAPH_IF_ADDED, edge->getEdgeIdx(),
			LogStringUtils::edgeVisualization(edge, "\t").c_str());
	edgeSet->push_back(edge);
}

void GraphIF::addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
		VertexIdx const vertexIdxV, EdgeCost const edgeCost,
		EdgeConnectionType connectionType, Visibility visibility) {
	VertexIF * vertexU = this->getVertexByIdx(vertexIdxU);
	VertexIF * vertexV = this->getVertexByIdx(vertexIdxV);
	EdgeIF * edge = new EdgeImpl { edgeIdx, VertexPair { vertexU, vertexV },
			edgeCost, connectionType, visibility };
	edgeSet->push_back(edge);
}

void GraphIF::addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
		VertexIdx const vertexIdxV, EdgeCost const edgeCost,
		EdgeConnectionType connectionType) {
	addEdge(edgeIdx, vertexIdxU, vertexIdxV, edgeCost, connectionType,
			Visibility::VISIBLE);
}

VertexIF * GraphIF::getVertexByIdx(VertexIdx const vertexIdx) {
	return this->vertexSet->getElementAt(vertexIdx);
}

EdgeIF * GraphIF::findEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * edge { };

	if ((edge = edgeSet->find(sourceVertex, targetVertex,
			EdgeConnectionType::UNDIRECTED)) != nullptr) {
		return edge;
	}

	throw LogicExceptions::EdgeNotFoundException();
}

EdgeIF * GraphIF::findEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::EdgeNotFoundException) {
	EdgeIF * edge { };

	if ((edge = edgeSet->find(sourceVertexIdx, targetVertexIdx,
			EdgeConnectionType::UNDIRECTED)) != nullptr) {
		return edge;
	}
	throw LogicExceptions::EdgeNotFoundException();
}

bool GraphIF::hasEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex) {
	return hasEdge(sourceVertex->getVertexIdx(), targetVertex->getVertexIdx());
}

bool GraphIF::hasEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx) {
	return edgeSet->find(sourceVertexIdx, targetVertexIdx,
			EdgeConnectionType::UNDIRECTED) != nullptr;
}

EdgeIF * GraphIF::findInputEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::VertexNotFoundException,
				LogicExceptions::EdgeNotFoundException) {
	VertexIF * vertex { };
	EdgeIF * edge { };

	if ((vertex = this->getVertexByIdx(sourceVertex->getVertexIdx()))
			!= nullptr) {
		if ((edge = vertex->findInputEdge(targetVertex)) != nullptr) {
			return edge;
		}
		throw LogicExceptions::EdgeNotFoundException();
	}
	throw LogicExceptions::VertexNotFoundException();
}

EdgeIF * GraphIF::findInputEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::VertexNotFoundException,
				LogicExceptions::EdgeNotFoundException) {
	VertexIF * vertex { };
	EdgeIF * edge { };

	if ((vertex = this->getVertexByIdx(sourceVertexIdx)) != nullptr) {
		if ((edge = vertex->findInputEdge(targetVertexIdx)) != nullptr) {
			return edge;
		}
		throw LogicExceptions::EdgeNotFoundException();
	}
	throw LogicExceptions::VertexNotFoundException();
}

bool GraphIF::hasInputEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex) {
	return hasInputEdge(sourceVertex->getVertexIdx(),
			targetVertex->getVertexIdx());
}

bool GraphIF::hasInputEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx) {
	VertexIF * vertex { };

	if ((vertex = this->getVertexByIdx(sourceVertexIdx)) != nullptr) {
		return vertex->findInputEdge(targetVertexIdx) != nullptr;
	}
	return false;
}

EdgeIF * GraphIF::findOutputEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex)
				throw (LogicExceptions::VertexNotFoundException,
				LogicExceptions::EdgeNotFoundException) {
	VertexIF * vertex { };
	EdgeIF * edge { };

	if ((vertex = this->getVertexByIdx(sourceVertex->getVertexIdx()))
			!= nullptr) {
		if ((edge = vertex->findOutputEdge(targetVertex)) != nullptr) {
			return edge;
		}
		throw LogicExceptions::EdgeNotFoundException();
	}
	throw LogicExceptions::VertexNotFoundException();
}

EdgeIF * GraphIF::findOutputEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx)
				throw (LogicExceptions::VertexNotFoundException,
				LogicExceptions::EdgeNotFoundException) {
	VertexIF * vertex { };
	EdgeIF * edge { };

	if ((vertex = this->getVertexByIdx(sourceVertexIdx)) != nullptr) {
		if ((edge = vertex->findOutputEdge(targetVertexIdx)) != nullptr) {
			return edge;
		}
		throw LogicExceptions::EdgeNotFoundException();
	}
	throw LogicExceptions::VertexNotFoundException();
}

bool GraphIF::hasOutputEdge(VertexIF * const sourceVertex,
		VertexIF * const targetVertex) {
	return hasOutputEdge(sourceVertex->getVertexIdx(),
			targetVertex->getVertexIdx());
}

bool GraphIF::hasOutputEdge(VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx) {
	VertexIF * vertex { };

	if ((vertex = this->getVertexByIdx(sourceVertexIdx)) != nullptr) {
		return vertex->findOutputEdge(targetVertexIdx) != nullptr;
	}
	return false;
}

EdgeCost GraphIF::getTotalEdgeCost() {
	return this->edgeSet->getTotalEdgeCost();
}

void GraphIF::beginVertex() {
	this->vertexSet->begin();
}

void GraphIF::beginVertex(IteratorId const iteratorId) {
	this->vertexSet->begin(iteratorId);
}

bool GraphIF::hasNextVertex() {
	return this->vertexSet->hasNext();
}

bool GraphIF::hasNextVertex(IteratorId const iteratorId) {
	return this->vertexSet->hasNext(iteratorId);
}

bool GraphIF::hasNextVertex(Visibility const visibility) {
	return this->vertexSet->hasNext(visibility);
}

bool GraphIF::hasNextVertex(IteratorId const iteratorId,
		Visibility const visibility) {
	return this->vertexSet->hasNext(iteratorId, visibility);
}

bool GraphIF::hasAnyVertex() {
	return this->vertexSet->Iterable::hasAny();
}

bool GraphIF::hasAnyVertex(IteratorId const iteratorId) {
	return this->vertexSet->Iterable::hasAny(iteratorId);
}

bool GraphIF::hasAnyVertex(Visibility const visibility) {
	return this->vertexSet->hasAny(visibility);
}

bool GraphIF::hasAnyVertex(IteratorId const iteratorId,
		Visibility const visibility) {
	return this->vertexSet->hasAny(iteratorId, visibility);
}

VertexIF * GraphIF::nextVertex() {
	return this->vertexSet->next();
}

VertexIF * GraphIF::nextVertex(IteratorId const iteratorId) {
	return this->vertexSet->next(iteratorId);
}

VertexIF * GraphIF::nextVertex(Visibility visibility) {
	return this->vertexSet->hasNext(visibility) ?
			this->vertexSet->next() : nullptr;
}

VertexIF * GraphIF::nextVertex(IteratorId const iteratorId,
		Visibility visibility) {
	return this->vertexSet->hasNext(iteratorId, visibility) ?
			this->vertexSet->next() : nullptr;
}

VertexIF * GraphIF::currentVertex() {
	return this->vertexSet->current();
}

VertexIF * GraphIF::peekPreviousVertex()
		throw (LogicExceptions::EmptyIteratorException) {
	return this->vertexSet->peek(-1);
}

IteratorId GraphIF::getVertexIteratorId() {
	return this->vertexSet->getIterator();
}

void GraphIF::removeVertexIterator(IteratorId const iteratorId) {
	return this->vertexSet->removeIterator(iteratorId);
}

void GraphIF::beginEdge() {
	this->edgeSet->begin();
}

void GraphIF::beginEdge(IteratorId const iteratorId) {
	this->edgeSet->begin(iteratorId);
}

bool GraphIF::hasNextEdge() {
	return this->edgeSet->hasNext();
}

bool GraphIF::hasNextEdge(IteratorId const iteratorId) {
	return this->edgeSet->hasNext(iteratorId);
}

bool GraphIF::hasNextEdge(Visibility const visibility) {
	return this->edgeSet->hasNext(visibility);
}

bool GraphIF::hasNextEdge(IteratorId const iteratorId,
		Visibility const visibility) {
	return this->edgeSet->hasNext(iteratorId, visibility);
}

bool GraphIF::hasNextEdge(Connectivity const connectivity) {
	return this->edgeSet->hasNext(connectivity);
}

bool GraphIF::hasNextEdge(IteratorId const iteratorId,
		Connectivity const connectivity) {
	return this->edgeSet->hasNext(iteratorId, connectivity);
}

bool GraphIF::hasNextEdge(Connectivity const connectivity,
		Visibility const visibility) {
	return this->edgeSet->hasNext(connectivity, visibility);
}

bool GraphIF::hasNextEdge(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	return this->edgeSet->hasNext(iteratorId, connectivity, visibility);
}

bool GraphIF::hasAnyEdge() {
	return this->edgeSet->Iterable::hasAny();
}

bool GraphIF::hasAnyEdge(IteratorId const iteratorId) {
	return this->edgeSet->Iterable::hasAny(iteratorId);
}

bool GraphIF::hasAnyEdge(Visibility const visibility) {
	return this->edgeSet->hasAny(visibility);
}

bool GraphIF::hasAnyEdge(IteratorId const iteratorId,
		Visibility const visibility) {
	return this->edgeSet->hasAny(iteratorId, visibility);
}

bool GraphIF::hasAnyEdge(Connectivity const connectivity) {
	this->edgeSet->begin();
	return this->edgeSet->hasNext(connectivity);
}

bool GraphIF::hasAnyEdge(IteratorId const iteratorId,
		Connectivity const connectivity) {
	this->edgeSet->begin();
	return this->edgeSet->hasNext(iteratorId, connectivity);
}

bool GraphIF::hasAnyEdge(Connectivity const connectivity,
		Visibility const visibility) {
	this->edgeSet->begin();
	return this->edgeSet->hasNext(connectivity, visibility);
}

bool GraphIF::hasAnyEdge(IteratorId const iteratorId,
		Connectivity const connectivity, Visibility const visibility) {
	this->edgeSet->begin();
	return this->edgeSet->hasNext(iteratorId, connectivity, visibility);
}

EdgeIF * GraphIF::nextEdge() {
	return this->edgeSet->next();
}

EdgeIF * GraphIF::nextEdge(IteratorId const iteratorId) {
	return this->edgeSet->next(iteratorId);
}

EdgeIF * GraphIF::currentEdge() {
	return this->edgeSet->current();
}

IteratorId GraphIF::getEdgeIteratorId() {
	return this->edgeSet->getIterator();
}

void GraphIF::removeEdgeIterator(IteratorId const iteratorId) {
	return this->edgeSet->removeIterator(iteratorId);
}

void GraphIF::showAllEdges() {
	this->edgeSet->showAll();
}

void GraphIF::showAllEdges(IteratorId const iteratorId) {
	this->edgeSet->showAll(iteratorId);
}

void GraphIF::hideAllEdges() {
	this->edgeSet->hideAll();
}

void GraphIF::hideAllEdges(IteratorId const iteratorId) {
	this->edgeSet->hideAll(iteratorId);
}

void GraphIF::disconnectAllEdges() {
	this->edgeSet->disconnectAll();
}

void GraphIF::disconnectAllEdges(IteratorId const iteratorId) {
	this->edgeSet->disconnectAll(iteratorId);
}

ConnectivityList GraphIF::storeEdgeConnectivity() {
	return this->edgeSet->storeConnectivity();
}

ConnectivityList GraphIF::storeEdgeConnectivity(IteratorId const iteratorId) {
	return this->edgeSet->storeConnectivity(iteratorId);
}

VisibilityList GraphIF::storeEdgeVisibility() {
	return this->edgeSet->storeVisibility();
}

VisibilityList GraphIF::storeEdgeVisibility(IteratorId const iteratorId) {
	return this->edgeSet->storeVisibility(iteratorId);
}

void GraphIF::restoreConnectivityAllEdges(
		ConnectivityList const & connectivityList) {
	this->edgeSet->restoreConnectivityAll(connectivityList);
}

void GraphIF::restoreConnectivityAllEdges(
		ConnectivityList const & connectivityList,
		IteratorId const iteratorId) {
	this->edgeSet->restoreConnectivityAll(connectivityList, iteratorId);
}

void GraphIF::restoreVisibilityAllEdges(VisibilityList const & visibilityList) {
	this->edgeSet->restoreVisibilityAll(visibilityList);
}

void GraphIF::restoreVisibilityAllEdges(VisibilityList const & visibilityList,
		IteratorId const iteratorId) {
	this->edgeSet->restoreVisibilityAll(visibilityList, iteratorId);
}

void GraphIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	jsonDoc.AddMember("Number of vertices", vertexSet->size(), allocator);
	jsonDoc.AddMember("Number of edges", edgeSet->size(), allocator);
	jsonDoc.AddMember("Vertex set",
			JSONUtils::getDepthLimitedJSON(vertexSet, allocator, "VertexSetIF",
					depth), allocator);
	jsonDoc.AddMember("Edge set",
			JSONUtils::getDepthLimitedJSON(edgeSet, allocator, "EdgeSetIF",
					depth), allocator);
}

std::string GraphIF::toString() {
	std::ostringstream oss { };
	oss << "Graph with " << vertexSet->size() << " number of vertices and "
			<< edgeSet->size() << " number of edges:" << std::endl;
	oss << vertexSet->toString() << std::endl;
	oss << edgeSet->toString() << std::endl;
	return oss.str();
}

std::string GraphIF::edgeSetToString() {
	return this->edgeSet->toString();
}

std::string GraphIF::edgeSetToString(Visibility edgeVisibility) {
	return this->edgeSet->toString(edgeVisibility);
}

//*************************************** GETTERS & SETTERS ****************************************//

EdgeCount GraphIF::getNumberOfEdges() const {
	return this->edgeSet->size();
}

EdgeCount GraphIF::getNumberOfEdges(Connectivity const connectivity) const {
	return this->edgeSet->size(connectivity);
}

EdgeCount GraphIF::getNumberOfEdges(Visibility const visibility) const {
	return this->edgeSet->size(visibility);
}

VertexCount GraphIF::getNumberOfVertices() const {
	return this->vertexSet->size();
}

VertexCount GraphIF::getNumberOfVertices(Visibility const visibility) const {
	return this->vertexSet->size(visibility);
}
