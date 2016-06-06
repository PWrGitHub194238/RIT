/*
 * CPLEXUtils.cpp
 *
 *  Created on: 24 kwi 2016
 *      Author: tomasz
 */

#include "../../include/cplex/CPLEXUtils.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include "../../include/cplex/CPLEX_Typedefs.hpp"
#include "../../include/enums/Connectivity.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetIF.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/primitive.hpp"

class CPLEX_LP_MSTSolverIF;

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("cplex.CPLEXUtils"));

IloEdgeValMap CPLEXUtils::transformEdgeSetToLP(GraphIF* graph,
		EdgeSetIF* selectedEdgeSet) {
	IloEdgeValMap rawForm { };
	EdgeIF* edge { };
	VertexIdx sourceVertexIdx { };
	VertexIdx targetVertexIdx { };

	INFO(logger, LogBundleKey::CPU_EDGESET_TO_RAW,
			LogStringUtils::edgeSetVisualization(selectedEdgeSet, "\t").c_str());

	graph->beginEdge();
	while (graph->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
		edge = graph->nextEdge();
		sourceVertexIdx = edge->getSourceVertex()->getVertexIdx();
		targetVertexIdx = edge->getTargetVertex()->getVertexIdx();

		if (rawForm.count(sourceVertexIdx) == 0) {
			rawForm.insert(
					std::make_pair(sourceVertexIdx,
							IloTargetVertexEdgeValMap { }));
		}

		rawForm.at(sourceVertexIdx).insert(
				std::make_pair(targetVertexIdx, false));
	}

	selectedEdgeSet->begin();
	while (selectedEdgeSet->hasNext(Visibility::VISIBLE)) {
		edge = selectedEdgeSet->next();
		sourceVertexIdx = edge->getSourceVertex()->getVertexIdx();
		targetVertexIdx = edge->getTargetVertex()->getVertexIdx();
		rawForm[sourceVertexIdx][targetVertexIdx] = true;
	}

	return rawForm;
}

