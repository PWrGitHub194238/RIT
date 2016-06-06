/*
 * LogStringUtils.hpp
 *
 *  Created on: 14 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_LOG_UTILS_LOGSTRINGUTILS_HPP_
#define SRC_INCLUDE_LOG_UTILS_LOGSTRINGUTILS_HPP_

#include <string>

#include "../../enums/Connectivity.hpp"
#include "../../enums/Visibility.hpp"
#include "../../typedefs/primitive.hpp"
#include "../bundle/Bundle.hpp"

class VertexSetIF;

class GraphEdgeCostsIF;

class EdgeIF;

class EdgeSetIF;

class GraphIF;

class VertexIF;

namespace LogStringUtils {

std::string vertexOutputEdges(VertexIF* const vertex,
		Visibility const visibility, const char* newLinePrefix = "");

std::string vertexOutputEdges(VertexIF* const vertex,
		const char* newLinePrefix = "");

std::string graphBasicDescription(GraphIF* const graph,
		const char* newLinePrefix = "");

std::string graphDescription(GraphIF* const graph, const char* newLinePrefix =
		"");

std::string edgeCostSetDescription(GraphEdgeCostsIF * const egeCostSet,
		const char* newLinePrefix = "");

std::string vertexBasicVisualization(VertexIF* const vertex,
		const char* newLinePrefix = "");

std::string vertexVisualization(VertexIF* const vertex,
		const char* newLinePrefix = "");

std::string vertexSetVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix = "");

std::string vertexSetVisualization(GraphIF* const graph,
		const char* newLinePrefix = "");

std::string vertexSetVisualization(VertexSetIF* const vertexSet,
		Visibility const visibility, const char* newLinePrefix = "");

std::string vertexSetVisualization(VertexSetIF* const vertexSet,
		const char* newLinePrefix = "");

std::string vertexSetFlatVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix = "");

std::string vertexSetFlatVisualization(GraphIF* const graph,
		const char* newLinePrefix = "");

std::string vertexSetFlatVisualization(VertexSetIF* const vertexSet,
		Visibility const visibility, const char* newLinePrefix = "");

std::string vertexSetFlatVisualization(VertexSetIF* const vertexSet,
		const char* newLinePrefix = "");

std::string edgeVisualization(EdgeIF* const edge,
		const char* newLinePrefix = "");

std::string edgeSetVisualization(GraphIF* const graph,
		Connectivity const connectivity, Visibility const visibility,
		const char* newLinePrefix = "");

std::string edgeSetVisualization(GraphIF* const graph,
		Connectivity const connectivity, const char* newLinePrefix = "");

std::string edgeSetVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix = "");

std::string edgeSetVisualization(GraphIF* const graph,
		const char* newLinePrefix = "");

std::string edgeSetVisualization(EdgeSetIF* const edgeSet,
		Visibility const visibility, const char* newLinePrefix = "");

std::string edgeSetVisualization(EdgeSetIF* const edgeSet,
		const char* newLinePrefix = "");

std::string mstEdgeDifference(EdgeSetIF* const baseMSTSolution,
		EdgeSetIF* const newMSTSolution, const char* newLinePrefix = "");

std::string edgeCostChanged(EdgeIF* edge, EdgeCost oldCost, EdgeCost newCost,
		bool forceShowChange, const char* newLinePrefix = "");

std::string edgeCostChanged(EdgeIF* edge, EdgeCost newCost,
		bool forceShowChange, const char* newLinePrefix = "");

std::string edgeCostChanged(EdgeIF* edge, EdgeCost newCost,
		const char* newLinePrefix = "");

std::string edgeSetCostChanged(GraphIF* graph, GraphEdgeCostsIF* newGraphCosts,
		bool onlyChanged, const char* newLinePrefix = "");

std::string edgeSetCostChanged(GraphIF* graph, GraphEdgeCostsIF* newGraphCosts,
		const char* newLinePrefix = "");

std::string edgeSetCostChanged(EdgeSetIF* edgeSet,
		GraphEdgeCostsIF* newGraphCosts, bool onlyChanged,
		const char* newLinePrefix = "");

std::string edgeSetCostChanged(EdgeSetIF* edgeSet,
		GraphEdgeCostsIF* newGraphCosts, const char* newLinePrefix = "");

template<typename ValueType>
std::string wrapInfinity(ValueType value);

std::string ifStatement(bool condition, LogBundleKey itTrueKey,
		LogBundleKey ifFalseKey);

namespace impl {

extern const char* VERTEX_OUTPUT_ARROW;

extern const char* EDGE_INPUT_ARROW;

extern const char* EDGE_OUTPUT_ARROW;

extern const char* EDGE_CHANGE_COST;

extern const char* EDGE_LINE_VISIBLE;

extern const char* EDGE_LINE_HIDDEN;

std::string visibleVertexCount(VertexCount overallCount,
		VertexCount visibleCount);

std::string connectedEdgeCount(EdgeCount overallCount,
		EdgeCount connectedCount);

std::string visibleEdgeCount(EdgeCount overallCount, EdgeCount visibleCount);

}  // namespace impl

}  // namespace LogStringUtils

#endif /* SRC_INCLUDE_LOG_UTILS_LOGSTRINGUTILS_HPP_ */
