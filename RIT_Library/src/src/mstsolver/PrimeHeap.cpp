/*
 * PrimeHeap.cpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#include "../../include/mstsolver/PrimeHeap.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <iostream>
#include <memory>
#include <string>

#include "../../include/enums/Visibility.hpp"
#include "../../include/heap/VertexHeapInclude.hpp"
#include "../../include/heap/VertexHeapItem.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/primitive.hpp"
#include "../../include/typedefs/struct.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("mstsolver.PrimeHeap"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

VertexHeapIF* PrimeHeap::createEdgeHeap(GraphIF * const graph,
		VertexIF * const initVertex) {
	VertexHeapIF * vertexHeap = new VertexHeapImpl { };
	VertexHeapItem * heapItem { };
	VertexIF * vertex { };
	EdgeIF * outputEdge { };

	INFO(logger, LogBundleKey::PH_CREATE_EDGE_HEAP_INIT,
			initVertex->getVertexIdx(),
			LogStringUtils::vertexOutputEdges(initVertex, Visibility::VISIBLE,
					"\t\t").c_str());

	INFO(logger, LogBundleKey::PH_FILL_HEAP_WITH_INIT_VERTICES,
			initVertex->getNumberOfOutputEdges(Visibility::VISIBLE),
			graph->getNumberOfVertices(Visibility::VISIBLE),
			initVertex->getVertexIdx());
	graph->beginVertex();
	initVertex->beginOutputEdges();
	while (initVertex->hasNextOutputEdge(Visibility::VISIBLE)) { // add all vertices that are neighbors of initVertex as their keys (costs) are well known
		outputEdge = initVertex->nextOutputEdge();
		INFO(logger, LogBundleKey::PH_ADD_FULL_VERTEX_TO_HEAP,
				outputEdge->getOtherVertex(initVertex)->getVertexIdx(),
				outputEdge->getEdgeCost(), initVertex->getVertexIdx());
		vertexHeap->push(
				new VertexHeapItem { outputEdge->getOtherVertex(initVertex),
						outputEdge->getEdgeCost(), initVertex });
	}

	INFO(logger, LogBundleKey::PH_FILL_HEAP_WITH_VERTICES, vertexHeap->size());

	while (graph->hasNextVertex(Visibility::VISIBLE)) {
		vertex = graph->nextVertex();
		if (!vertexHeap->isVertexInsideHeap(vertex) && vertex != initVertex) {
			INFO(logger, LogBundleKey::PH_ADD_VERTEX_TO_HEAP_INF_COST,
					vertex->getVertexIdx());
			vertexHeap->push(new VertexHeapItem { vertex });
		}
	}
	return vertexHeap;
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

EdgeSetIF * PrimeHeap::resolve(VertexIF * const initialVertex) {
	EdgeIF * edge { };
	VertexIF * vertex { };
	VertexIF* predecessor { };
	VertexHeapItem * vetrexTarget { };
	VertexCount mstEdgeCount = this->graph->getNumberOfVertices() - 1;
	EdgeSetIF * mstEdgeSet = new EdgeSetImpl { mstEdgeCount };

	TRACE_NOARG(logger, LogBundleKey::PH_MST_CONSTRUCT_INIT);

	VertexHeapIF * vertexHeap = this->createEdgeHeap(graph, initialVertex);

	VisibilityList visibilityConfiguration = this->graph->storeEdgeVisibility();
	this->graph->hideAllEdges();

	while (mstEdgeSet->size() < mstEdgeCount) {
		predecessor = vertexHeap->peek()->getPredecessor();
		vertex = vertexHeap->pop();
		edge = vertex->findInputEdge(predecessor);
		edge->show();
		INFO(logger, LogBundleKey::PH_EDGE_TO_MST, vertex->getVertexIdx(),
				predecessor->getVertexIdx(),
				vertex->getNumberOfOutputEdges(Visibility::HIDDEN),
				LogStringUtils::edgeVisualization(edge, "\t").c_str());
		mstEdgeSet->push_back(edge);
		INFO(logger, LogBundleKey::PH_SCAN_OUTPUTS, vertex->getVertexIdx(),
				LogStringUtils::vertexOutputEdges(vertex, Visibility::HIDDEN,
						"\t").c_str());
		vertex->beginOutputEdges();
		while (vertex->hasNextOutputEdge(Visibility::HIDDEN)) {
			edge = vertex->nextOutputEdge();
			vetrexTarget = vertexHeap->getItem(
					edge->getOtherVertex(vertex)->getVertexIdx());
			if (vetrexTarget != nullptr
					&& vetrexTarget->getKey() > edge->getEdgeCost()) { // jest w kopcu i d(j) > c_{ij}
				INFO(logger, LogBundleKey::PH_UPDATE_VERTEX,
						vetrexTarget->getValue()->getVertexIdx(),
						LogStringUtils::wrapInfinity<VertexKey>(
								vetrexTarget->getKey()).c_str(),
						edge->getEdgeCost(), vertex->getVertexIdx());
				vetrexTarget->setPredecessor(vertex);
				vetrexTarget->setKey(edge->getEdgeCost());
				vertexHeap->decreaseKey(vetrexTarget, vetrexTarget->getKey());
			}
		}
	}
	INFO(logger, LogBundleKey::PH_MST_SOLUTION,
			LogStringUtils::edgeSetVisualization(mstEdgeSet, "\t").c_str(),
			mstEdgeSet->getTotalEdgeCost());
	this->graph->restoreVisibilityAllEdges(visibilityConfiguration);
	delete vertexHeap;
	return mstEdgeSet;
}

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

PrimeHeap::PrimeHeap(GraphIF * const graph) :
		MSTSolverIF(graph) {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

