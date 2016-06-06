/*
 * GraphUtils.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/GraphUtils.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <cstdlib>
#include <limits>
#include <list>
#include <memory>
#include <set>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>

#include "../../include/enums/Connectivity.hpp"
#include "../../include/enums/EdgeConnectionType.hpp"
#include "../../include/enums/GraphConstructMode.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/exp/GraphExceptions.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogGraphUtils.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphEdgeCostsIF.hpp"
#include "../../include/structures/GraphInclude.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/structures/VertexSetInclude.hpp"
#include "../../include/typedefs/primitive.hpp"
#include "../../include/typedefs/struct.hpp"
#include "../../include/utils/MemoryUtils.hpp"
#include "../../include/utils/TabuSearchUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.GraphUtils"));

const EdgeCost GraphUtils::MIN_EDGE_COST { std::numeric_limits<EdgeCost>::min() };
const EdgeCost GraphUtils::MAX_EDGE_COST { std::numeric_limits<EdgeCost>::max() };

bool GraphUtils::isGraphConnected(GraphIF* const graph,
		EdgeSetIF* const notHiddenEdges) {
	graph->hideAllEdges();
	notHiddenEdges->begin();
	while (notHiddenEdges->hasNext()) {
		notHiddenEdges->next()->show();
	}
	INFO(logger, LogBundleKey::GU_CHECK_GRAPH_CONNECTIVITY_WITH_EDGE_SET,
			LogStringUtils::graphBasicDescription(graph, "\t").c_str(),
			LogStringUtils::edgeSetVisualization(notHiddenEdges, "\t").c_str());
	return isGraphConnected(graph);
}

bool GraphUtils::isGraphConnected(GraphIF* const graph) {
	VertexCount numberOfVertices = graph->getNumberOfVertices();
	std::vector<bool> discoveryArray(numberOfVertices, false);
	std::list<VertexIF*> vertexStack { };
	VertexIF* vertex { };
	VertexIdx idx { };

	INFO(logger, LogBundleKey::GU_CHECK_GRAPH_CONNECTIVITY_INIT,
			LogStringUtils::graphBasicDescription(graph, "\t").c_str());

	graph->beginVertex();
	while (graph->hasNextVertex(Visibility::VISIBLE)) {
		if (!graph->nextVertex()->hasAnyInputEdge()) {
			WARN(logger, LogBundleKey::GU_VERTEX_INPUT_DEGREE_ZERO,
					LogGraphUtils::peekPreviousVertexIdx(graph));
			return false;
		}
	}

	if (graph->hasAnyVertex(Visibility::VISIBLE)) {
		INFO(logger, LogBundleKey::GU_INIT_VERTEX_STACK,
				graph->currentVertex()->getVertexIdx());
		vertexStack.push_back(graph->nextVertex());
		while (!vertexStack.empty()) {
			vertex = vertexStack.front();
			vertexStack.pop_front();
			INFO(logger, LogBundleKey::GU_POP_VERTEX, vertex->getVertexIdx(),
					LogStringUtils::ifStatement(
							discoveryArray[vertex->getVertexIdx()],
							LogBundleKey::GU_VERTEX_VISITED,
							LogBundleKey::GU_VERTEX_NOT_VISITED).c_str());

			if (vertex->isVisible()
					&& discoveryArray[vertex->getVertexIdx()] == false) {
				discoveryArray[vertex->getVertexIdx()] = true;
				INFO(logger, LogBundleKey::GU_ITERATE_VERTEX_EDGES,
						vertex->getVertexIdx(),
						LogStringUtils::vertexOutputEdges(vertex,
								Visibility::VISIBLE, "\t").c_str());
				vertex->beginOutputEdges();
				while (vertex->hasNextOutputEdge(Visibility::VISIBLE)) {
					INFO(logger, LogBundleKey::GU_PUSH_VERTEX,
							vertex->currentOutputEdgeTarget()->getVertexIdx());
					vertexStack.push_back(vertex->nextOutputEdgeTarget());
				}
			}
		}
	} else {
		WARN(logger, LogBundleKey::GU_NO_VERTEX_GRAPH,
				LogStringUtils::graphBasicDescription(graph, "\t").c_str());
		return true;
	}

	INFO(logger, LogBundleKey::GU_CHECK_DISCOVERY_ARRAY,
			graph->getNumberOfVertices(Visibility::VISIBLE));

	for (idx = 0; idx < numberOfVertices; idx += 1) {
		if (graph->getVertexByIdx(idx)->isVisible()
				&& discoveryArray[idx] == false) {
			WARN(logger, LogBundleKey::GU_CHECK_VERTEX_NOT_VISITED, idx);
			return false;
		}
	}
	INFO_NOARG(logger, LogBundleKey::GU_CHECK_GRAPH_CONNECTIVITY_POSITIVE);
	return true;
}

void GraphUtils::changeGraphCosts(GraphIF* graph,
		GraphEdgeCostsIF* const newGraphCosts) {
	IteratorId edgeIterator { };
	if (newGraphCosts->size() == graph->getNumberOfEdges()) {
		edgeIterator = graph->getEdgeIteratorId();
		graph->beginEdge(edgeIterator);
		newGraphCosts->begin();
		while (graph->hasNextEdge(edgeIterator)) {
			graph->nextEdge(edgeIterator)->setEdgeCost(newGraphCosts->next());
		}
	}
	graph->removeEdgeIterator(edgeIterator);
}

bool GraphUtils::changeGraphCostsWithCheck(GraphIF* graph,
		GraphEdgeCostsIF* const newGraphCosts) {
	IteratorId edgeIterator { };
	EdgeCost oldEdgeCost { };
	EdgeCost newEdgeCost { };
	EdgeIF* edge { };
	bool isAnyCostChanged { false };
	if (newGraphCosts->size() == graph->getNumberOfEdges()) {
		edgeIterator = graph->getEdgeIteratorId();
		graph->beginEdge(edgeIterator);
		newGraphCosts->begin();
		while (graph->hasNextEdge(edgeIterator)) {
			edge = graph->nextEdge(edgeIterator);
			oldEdgeCost = edge->getEdgeCost();
			newEdgeCost = newGraphCosts->next();
			if (oldEdgeCost != newEdgeCost) {
				isAnyCostChanged = true;
				edge->setEdgeCost(newEdgeCost);
			}
		}
		graph->removeEdgeIterator(edgeIterator);
	}
	return isAnyCostChanged;
}

ConnectivityList GraphUtils::shrinkConnectivityToSet(GraphIF* const graph,
		EdgeSetIF* const visibleSet) {
	IteratorId edgeIterator = graph->getEdgeIteratorId();
	ConnectivityList connectivityList = graph->storeEdgeConnectivity(
			edgeIterator);
	ConnectivityList visibleSetConnectivityList = visibleSet->storeConnectivity(
			edgeIterator);
	ConnectivityList::const_iterator itBegin =
			visibleSetConnectivityList.begin();

	graph->disconnectAllEdges(edgeIterator);
	visibleSet->begin(edgeIterator);
	while (visibleSet->hasNext(edgeIterator)) {
		visibleSet->next(edgeIterator)->connect(*itBegin);
		++itBegin;
	}
	graph->removeEdgeIterator(edgeIterator);
	return connectivityList;
}

VisibilityList GraphUtils::shrinkVisibilityToSet(GraphIF* const graph,
		EdgeSetIF* const visibleSet) {
	IteratorId edgeIterator = graph->getEdgeIteratorId();
	VisibilityList visibilityList = graph->storeEdgeVisibility(edgeIterator);
	graph->hideAllEdges(edgeIterator);
	visibleSet->begin(edgeIterator);
	while (visibleSet->hasNext(edgeIterator)) {
		visibleSet->next(edgeIterator)->show();
	}
	graph->removeEdgeIterator(edgeIterator);
	return visibilityList;
}

EdgeSetIF* GraphUtils::getRandomSpanningTree(GraphIF* const graph,
		EdgeSetIF* const visibleSet)
				throw (GraphExceptions::DisconnectedGraphException) {
	VisibilityList visibilityList = GraphUtils::shrinkVisibilityToSet(graph,
			visibleSet);
	EdgeSetIF* randomSpanningTree = getRandomSpanningTree(graph);
	graph->restoreVisibilityAllEdges(visibilityList);
	return randomSpanningTree;
}

EdgeSetIF* GraphUtils::getRandomSpanningTree(GraphIF* const graph)
		throw (GraphExceptions::DisconnectedGraphException) {
	EdgeSetIF * spanningTreeEdges { };
	VertexCount numberOfVertices { };
	EdgeCount numberOfEdgesInMST { };
	std::vector<bool> discoveryArray { };
	std::list<VertexIF*> vertexStack { };
	VertexIF* vertex { };
	VertexIdx idx { };

	INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_INIT,
			LogStringUtils::graphBasicDescription(graph, "\t").c_str());

	if (isGraphConnected(graph)) {
		spanningTreeEdges = new EdgeSetImpl { };
		numberOfVertices = graph->getNumberOfVertices();
		numberOfEdgesInMST = numberOfVertices - 1;
		discoveryArray = std::vector<bool>(numberOfVertices, false);
		idx = std::rand() % numberOfVertices;
		discoveryArray[idx] = true;	//root

		INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_ROOT, idx);

		for (idx = 0; idx < numberOfVertices; idx += 1) {
			if (spanningTreeEdges->size() == numberOfEdgesInMST) {
				INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_BREAK, idx);
				break;
			}

			INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_CONSTRUCT_PATH, idx);
			vertexStack.clear();
			vertex = graph->getVertexByIdx(idx);

			if (discoveryArray.at(vertex->getVertexIdx()) == false) {
				INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_NOT_DISCOVERED,
						idx);
				vertexStack.push_back(vertex);

				do {
					INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_BUILD_PATH,
							vertex->getVertexIdx(),
							LogStringUtils::vertexOutputEdges(vertex, "\t").c_str());
					vertex = vertex->getRandomSuccessor();
					INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_ADD_TO_PATH,
							vertex->getVertexIdx());
					vertexStack.push_back(vertex);
				} while (discoveryArray.at(vertex->getVertexIdx()) == false);

				INFO(logger,
						LogBundleKey::GU_RND_MST_SEARCH_ADD_EDGES_FROM_PATH,
						vertexStack.back()->getVertexIdx(), idx,
						vertexStack.size() - 1);
				vertex = vertexStack.back();
				vertexStack.pop_back();
				while (!vertexStack.empty()) {
					if (spanningTreeEdges->size() == numberOfEdgesInMST) {
						vertexStack.clear();
						break;
					}
					if (discoveryArray[vertexStack.back()->getVertexIdx()]
							== false) {
						discoveryArray[vertexStack.back()->getVertexIdx()] =
								true;
						INFO(logger,
								LogBundleKey::GU_RND_MST_SEARCH_ADD_EDGE_FROM_PATH,
								vertexStack.back()->getVertexIdx(),
								vertex->getVertexIdx(),
								LogStringUtils::edgeVisualization(
										vertexStack.back()->findOutputEdge(
												vertex), "\t").c_str());
						spanningTreeEdges->push_back(
								vertex->findOutputEdge(vertexStack.back()));
					}
					vertex = vertexStack.back();
					vertexStack.pop_back();
				}
			}
			INFO(logger, LogBundleKey::GU_RND_MST_SEARCH_CONSTRUCT_PATH_END,
					idx);

		}
		INFO(logger, LogBundleKey::GU_RND_MST_FOUND,
				LogStringUtils::edgeSetVisualization(spanningTreeEdges, "\t").c_str());
		return spanningTreeEdges;
	} else {
		WARN_NOARG(logger, LogBundleKey::GU_RND_MST_GRAPH_DISCONNECTED);
		throw GraphExceptions::DisconnectedGraphException();
	}
}

GraphIF* GraphUtils::getRandomGraph(VertexCount numberOfVertices,
		Density density, EdgeCost lowerEdgeCost, EdgeCost upperEdgeCost) {
	EdgeCount numberOfEdges { };
	if (density > 100) {
		density /= 100;
	}
	if (density > 1) {
		density = 1;
	}
	numberOfEdges = density * (numberOfVertices * (numberOfVertices - 1) / 2);
	return getRandomGraph(numberOfVertices, numberOfEdges, lowerEdgeCost,
			upperEdgeCost);
}

GraphIF* GraphUtils::getRandomGraph(VertexCount numberOfVertices,
		EdgeCount numberOfEdges, EdgeCost lowerEdgeCost,
		EdgeCost upperEdgeCost) {
	EdgeIdx edgeIdx { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };
	EdgeCost edgeCost { };
	GraphIF* graph { };
	EdgeCount maxNumberOfEdges = numberOfVertices * (numberOfVertices - 1) / 2;
	std::map<VertexIdx, std::unordered_set<VertexIdx>> avaliableEdges { };
	std::unordered_set<VertexIdx>::const_iterator edgeItBegin { };
	for (sourceVertexIdx = 0; sourceVertexIdx < numberOfVertices;
			sourceVertexIdx += 1) {
		avaliableEdges.insert(
				std::make_pair(sourceVertexIdx,
						std::unordered_set<VertexIdx> { }));
		for (targetVertexIdx = 0; targetVertexIdx < numberOfVertices;
				targetVertexIdx += 1) {
			avaliableEdges.at(sourceVertexIdx).insert(targetVertexIdx);
		}
	}

	for (sourceVertexIdx = 0; sourceVertexIdx < numberOfVertices;
			sourceVertexIdx += 1) {
		avaliableEdges.at(sourceVertexIdx).erase(sourceVertexIdx);
	}

	if (numberOfEdges > maxNumberOfEdges) {
		WARN(logger, LogBundleKey::GU_RND_GRAPH_MAX_EDGE_COUNT, numberOfEdges,
				maxNumberOfEdges);
		numberOfEdges = maxNumberOfEdges;
	}
	graph = new GraphImpl { numberOfVertices, numberOfEdges,
			GraphConstructMode::AUTO_CONSTRUCT_VERTEX };

	for (targetVertexIdx = 1; targetVertexIdx < numberOfVertices;
			targetVertexIdx += 1) {
		sourceVertexIdx = rand() % targetVertexIdx;
		edgeCost = rand()
				% ((unsigned int) upperEdgeCost - (unsigned int) lowerEdgeCost)
				+ lowerEdgeCost + 1;
		graph->addEdge(edgeIdx, sourceVertexIdx, targetVertexIdx, edgeCost,
				EdgeConnectionType::UNDIRECTED, Visibility::VISIBLE);
		avaliableEdges.at(sourceVertexIdx).erase(targetVertexIdx);
		edgeIdx += 1;
	}

	for (edgeIdx = numberOfVertices; edgeIdx <= numberOfEdges; edgeIdx += 1) {
		do {
			sourceVertexIdx = rand() % numberOfVertices;
		} while (avaliableEdges.at(sourceVertexIdx).size() == 0);

		edgeItBegin = avaliableEdges.at(sourceVertexIdx).begin();
		std::advance(edgeItBegin,
				rand() % avaliableEdges.at(sourceVertexIdx).size());
		targetVertexIdx = *edgeItBegin;
		edgeCost = rand()
				% ((unsigned int) upperEdgeCost - (unsigned int) lowerEdgeCost)
				+ lowerEdgeCost + 1;
		graph->addEdge(edgeIdx, sourceVertexIdx, targetVertexIdx, edgeCost,
				EdgeConnectionType::UNDIRECTED, Visibility::VISIBLE);
		avaliableEdges.at(sourceVertexIdx).erase(targetVertexIdx);
	}
	return graph;
}

EdgeSetIF* GraphUtils::getMSTEdgesBeetwenNodes(GraphIF* graph,
		EdgeSetIF* mstEdgeSet, VertexIdx const sourceVertexIdx,
		VertexIdx const targetVertexIdx) {
	return getMSTEdgesBeetwenNodes(graph, mstEdgeSet,
			graph->getVertexByIdx(sourceVertexIdx),
			graph->getVertexByIdx(targetVertexIdx));
}

EdgeSetIF* GraphUtils::getMSTEdgesBeetwenNodes(GraphIF* graph,
		EdgeSetIF* mstEdgeSet, VertexIF* sourceVertex, VertexIF* targetVertex) {
	VertexIF* vertex { };
	VertexIF* vertexFromQueue { };
	VertexSetIF* path { };
	VertexSetIF* pathFromQueue { };
	EdgeIF* edge { };
	bool isPathFound { false };
	EdgeSetIF* storedPath { };
	std::list<VertexIF*> vertexQueue { };
	std::list<VertexSetIF*> pathsQueue { };
	std::vector<bool> edgeDiscoveryArray(graph->getNumberOfVertices() - 1,
			false);
	IteratorId iteratorId = graph->getEdgeIteratorId();
	VisibilityList visibilityList = graph->storeEdgeVisibility(iteratorId);
	graph->hideAllEdges(iteratorId);
	mstEdgeSet->begin(iteratorId);
	while (mstEdgeSet->hasNext(iteratorId)) {
		mstEdgeSet->next(iteratorId)->show();
	}

	INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_INIT,
			sourceVertex->getVertexIdx(), targetVertex->getVertexIdx());

	edgeDiscoveryArray[sourceVertex->getVertexIdx()] = true;

	INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_SCAN_SRC,
			sourceVertex->getVertexIdx(),
			LogStringUtils::vertexOutputEdges(sourceVertex, "\t").c_str());

	sourceVertex->beginOutputEdges();
	while (sourceVertex->hasNextOutputEdge(Visibility::VISIBLE)) {
		path = new VertexSetImpl { graph->getNumberOfVertices() };
		INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_SRC_PUSH_1,
				LogStringUtils::edgeVisualization(
						sourceVertex->currentOutputEdge(), "\t").c_str());
		vertex = sourceVertex->nextOutputEdgeTarget();
		if (vertex == targetVertex) {
			isPathFound = true;
			storedPath = new EdgeSetImpl { };
			storedPath->push_back(sourceVertex->findOutputEdge(vertex));
		}
		path->push_back(vertex);
		vertexQueue.push_back(vertex);
		pathsQueue.push_back(path);
		INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_SRC_PUSH_2,
				vertex->getVertexIdx());
	}

	INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_SCAN_INIT,
			pathsQueue.size());

	while (!isPathFound && !pathsQueue.empty()) {
		vertexFromQueue = vertexQueue.front();
		pathFromQueue = pathsQueue.front();
		vertexQueue.pop_front();
		pathsQueue.pop_front();
		INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_POP,
				vertexFromQueue->getVertexIdx(),
				LogStringUtils::vertexSetFlatVisualization(pathFromQueue, "\t").c_str(),
				pathsQueue.size());

		INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_SCAN,
				vertexFromQueue->getVertexIdx(),
				LogStringUtils::vertexOutputEdges(vertexFromQueue, "\t").c_str());
		vertexFromQueue->beginOutputEdges();
		edgeDiscoveryArray[vertexFromQueue->getVertexIdx()] = true;
		while (vertexFromQueue->hasNextOutputEdge(Visibility::VISIBLE)) {
			vertex = vertexFromQueue->nextOutputEdgeTarget();
			INFO(logger,
					edgeDiscoveryArray[vertex->getVertexIdx()] == false ?
							LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_VERTEX_PROCESS :
							LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_VERTEX_VISITED,
					vertex->getVertexIdx());

			if (edgeDiscoveryArray[vertex->getVertexIdx()] == false) {
				path = new VertexSetImpl { graph->getNumberOfVertices() };
				pathFromQueue->begin(iteratorId);
				while (pathFromQueue->hasNext(iteratorId)) {
					path->push_back(pathFromQueue->next(iteratorId));
				}
				path->push_back(vertex);
				vertexQueue.push_back(vertex);
				pathsQueue.push_back(path);
				INFO(logger,
						LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_VERTEX_NOT_VISITED,
						vertex->getVertexIdx(),
						LogStringUtils::vertexSetFlatVisualization(path, "\t").c_str());

				if (vertex == targetVertex) {
					storedPath = new EdgeSetImpl { };
					if (path->hasAny(iteratorId, Visibility::VISIBLE)) {
						vertex = path->next(iteratorId);
						edge = sourceVertex->findOutputEdge(vertex);
						storedPath->push_back(edge);
						while (path->hasNext(iteratorId, Visibility::VISIBLE)) {
							edge = vertex->findOutputEdge(
									path->current(iteratorId));
							vertex = path->next(iteratorId);
							storedPath->push_back(edge);
						}
					}
					INFO(logger, LogBundleKey::GU_EDGES_ON_SP_TREE_PATH_STORED,
							sourceVertex->getVertexIdx(),
							targetVertex->getVertexIdx(),
							LogStringUtils::edgeSetVisualization(storedPath,
									"\t").c_str());
					isPathFound = true;
					break;
				}
			}
		}
		MemoryUtils::removeCollection(pathFromQueue, false);
	}

	while (!pathsQueue.empty()) {
		vertexFromQueue = vertexQueue.front();
		pathFromQueue = pathsQueue.front();
		vertexQueue.pop_front();
		pathsQueue.pop_front();
		MemoryUtils::removeCollection(pathFromQueue, false);
	}

	graph->restoreVisibilityAllEdges(visibilityList);
	graph->removeEdgeIterator(iteratorId);
	return storedPath;
}

SpanningTreeNeighborhood GraphUtils::getEdgeSetNeighbourhood(GraphIF * graph,
		EdgeSetIF* originalSet) {
	SpanningTreeNeighborhood neighborhoodSet { };
	EdgeSetIF* path { };
	IteratorId edgeIteratorId = graph->getEdgeIteratorId();
	IteratorId pathIteratorId = graph->getEdgeIteratorId();

	VisibilityList visibilityList = graph->storeEdgeVisibility();
	EdgeIF * noMSTedge { };
	EdgeIF * pathEdge { };

	INFO(logger, LogBundleKey::GU_FIND_NEIGHBORHOOD_INIT,
			LogStringUtils::edgeSetVisualization(originalSet, "\t").c_str());
	originalSet->begin(edgeIteratorId);
	while (originalSet->hasNext(edgeIteratorId)) {
		originalSet->next(edgeIteratorId)->hide();
	}
	graph->beginEdge(edgeIteratorId);
	while (graph->hasNextEdge(edgeIteratorId, Connectivity::CONNECTED,
			Visibility::VISIBLE)) {
		noMSTedge = graph->nextEdge(edgeIteratorId);
		INFO(logger, LogBundleKey::GU_FIND_NEIGHBORHOOD_ADD_EDGE,
				LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str());
		path = getMSTEdgesBeetwenNodes(graph, originalSet,
				noMSTedge->getSourceVertex(), noMSTedge->getTargetVertex());
		originalSet->push_back(noMSTedge);
		INFO(logger, LogBundleKey::GU_FIND_NEIGHBORHOOD_PATH_INSTEAD_EDGE,
				LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str(),
				noMSTedge->getSourceVertex()->getVertexIdx(),
				noMSTedge->getTargetVertex()->getVertexIdx(),
				LogStringUtils::edgeSetVisualization(path, "\t").c_str());
		path->begin(pathIteratorId);
		while (path->hasNext(pathIteratorId)) {
			pathEdge = path->next(pathIteratorId);
			originalSet->remove(pathEdge);
			neighborhoodSet.insert(
					TabuSearchUtils::createSpanningTreeNeighbor(
							noMSTedge->getSourceVertex()->getVertexIdx(),
							pathEdge->getSourceVertex()->getVertexIdx(),
							new EdgeSetImpl { originalSet, false }));
			INFO(logger, LogBundleKey::GU_FIND_NEIGHBORHOOD_STORE_SET,
					LogStringUtils::edgeSetVisualization(originalSet, "\t").c_str(),
					LogStringUtils::edgeVisualization(noMSTedge, "\t").c_str(),
					LogStringUtils::edgeVisualization(pathEdge, "\t").c_str());
			originalSet->push_back(pathEdge);
		}
		originalSet->remove(noMSTedge);
		MemoryUtils::removeCollection(path, false);
	}
	graph->restoreVisibilityAllEdges(visibilityList);
	INFO(logger, LogBundleKey::GU_FIND_NEIGHBORHOOD_END,
			LogStringUtils::edgeSetVisualization(originalSet, "\t").c_str(),
			neighborhoodSet.size());
	return neighborhoodSet;
}
