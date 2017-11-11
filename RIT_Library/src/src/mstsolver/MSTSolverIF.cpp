/*
 * MSTSolverIF.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#include "../../include/mstsolver/MSTSolverIF.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <memory>

#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphEdgeCostsInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/struct.hpp"
#include "../../include/utils/GraphUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("mstsolver.MSTSolverIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

MSTSolverIF::MSTSolverIF(GraphIF * const graph) {
	this->graph = graph;
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeSetIF * MSTSolverIF::getSolution()
		throw (GraphExceptions::DisconnectedGraphException) {
	if (!graph->hasAnyVertex(Visibility::VISIBLE)) {
		WARN(logger, LogBundleKey::MSTS_IF_EMPTY_INPUT_GRAPH);
		return new EdgeSetImpl { };
	} else {
		return getSolution(graph->nextVertex(Visibility::VISIBLE));
	}
}

EdgeSetIF * MSTSolverIF::getSolution(GraphEdgeCostsIF * const & graphScenario)
		throw (GraphExceptions::DisconnectedGraphException) {
	GraphEdgeCostsIF* graphEdgeCostsBackup = new GraphEdgeCostsImpl { graph };
	GraphUtils::changeGraphCosts(graph, graphScenario);
	EdgeSetIF * mstForScenario = this->getSolution();
	GraphUtils::changeGraphCosts(graph, graphEdgeCostsBackup);
	delete graphEdgeCostsBackup;
	return mstForScenario;
}

EdgeSetIF * MSTSolverIF::getSolution(EdgeSetIF * const & visibleSet)
		throw (GraphExceptions::DisconnectedGraphException) {
	ConnectivityList connectivityList = GraphUtils::shrinkConnectivityToSet(
			graph, visibleSet);
	EdgeSetIF* minimumSpanningTree = getSolution();
	graph->restoreConnectivityAllEdges(connectivityList);
	return minimumSpanningTree;
}

EdgeSetIF * MSTSolverIF::getSolution(VertexIF * const & initialVertex)
		throw (GraphExceptions::DisconnectedGraphException) {
	if (initialVertex == nullptr) {
		return getSolution();
	}
	if (GraphUtils::isGraphConnected(graph) == false) {
		ERROR(logger, LogBundleKey::MSTS_IF_GRAPH_NOT_CONNECTED);
		throw GraphExceptions::DisconnectedGraphException();
	} else {
		INFO(logger, LogBundleKey::MSTS_IF_CONSTRUCT_FROM_SOURCE,
				graph->getNumberOfVertices(Visibility::VISIBLE),
				graph->getNumberOfEdges(Visibility::VISIBLE),
				initialVertex->getVertexIdx());
		return resolve(initialVertex);
	}
}

EdgeSetIF * MSTSolverIF::getSolution(VertexIF * const & initialVertex,
		EdgeSetIF * const & visibleSet)
				throw (GraphExceptions::DisconnectedGraphException) {
	VisibilityList visibilityList = GraphUtils::shrinkVisibilityToSet(graph,
			visibleSet);
	EdgeSetIF* minimumSpanningTree = getSolution(initialVertex);
	graph->restoreVisibilityAllEdges(visibilityList);
	return minimumSpanningTree;
}

//*************************************** GETTERS & SETTERS ****************************************//

void MSTSolverIF::setGraph(GraphIF* graph) {
	this->graph = graph;
}
