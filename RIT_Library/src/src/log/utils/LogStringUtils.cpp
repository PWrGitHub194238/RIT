/*
 * LogStringUtils.cpp
 *
 *  Created on: 14 lut 2016
 *      Author: tomasz
 */

#include "../../../include/log/utils/LogStringUtils.hpp"

#include <limits>
#include <memory>
#include <sstream>
#include <string>

#include "../../../include/enums/Connectivity.hpp"
#include "../../../include/enums/EdgeConnectionType.hpp"
#include "../../../include/enums/Visibility.hpp"
#include "../../../include/log/bundle/Bundle.hpp"
#include "../../../include/log/utils/LogUtils.hpp"
#include "../../../include/structures/EdgeIF.hpp"
#include "../../../include/structures/EdgeSetIF.hpp"
#include "../../../include/structures/GraphEdgeCostsIF.hpp"
#include "../../../include/structures/GraphIF.hpp"
#include "../../../include/structures/VertexIF.hpp"
#include "../../../include/structures/VertexSetIF.hpp"
#include "../../../include/typedefs/primitive.hpp"
#include "../../../include/typedefs/struct.hpp"
#include "../../../include/utils/StringUtils.hpp"

class EdgeIF;
class VertexIF;
class VertexSetIF;

class EdgeSetIF;
class GraphEdgeCostsIF;
class GraphIF;

const char* LogStringUtils::impl::VERTEX_OUTPUT_ARROW { "-->" };

const char* LogStringUtils::impl::EDGE_INPUT_ARROW { "<" };

const char* LogStringUtils::impl::EDGE_OUTPUT_ARROW { ">" };

const char* LogStringUtils::impl::EDGE_CHANGE_COST { "==>" };

const char* LogStringUtils::impl::EDGE_LINE_VISIBLE { "-------" };

const char* LogStringUtils::impl::EDGE_LINE_HIDDEN { "- - - -" };

std::string LogStringUtils::vertexOutputEdges(VertexIF* const vertex,
		Visibility const visibility, const char* newLinePrefix) {
	std::ostringstream oss { };
	EdgeIF * outputEdge { };

	if (vertex->hasAnyOutputEdge(visibility)) {
		while (vertex->hasNextOutputEdge(visibility)) {
			outputEdge = vertex->nextOutputEdge();
			oss << newLinePrefix << LogStringUtils::impl::VERTEX_OUTPUT_ARROW
					<< " "
					<< LogStringUtils::vertexBasicVisualization(
							outputEdge->getOtherVertex(vertex))
					<< LogStringUtils::edgeVisualization(outputEdge, "\t");
			if (vertex->hasNextOutputEdge(visibility)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_OUTPUT_EDGE_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexOutputEdges(VertexIF* const vertex,
		const char* newLinePrefix) {
	std::ostringstream oss { };
	return LogStringUtils::vertexOutputEdges(vertex, Visibility::BOTH,
			newLinePrefix);
}

std::string LogStringUtils::graphBasicDescription(GraphIF* const graph,
		const char* newLinePrefix) {
	std::ostringstream oss { };
	oss << newLinePrefix << "Number of vertices\t:\t"
			<< graph->getNumberOfVertices();
	oss
			<< LogStringUtils::impl::visibleVertexCount(
					graph->getNumberOfVertices(),
					graph->getNumberOfVertices(Visibility::VISIBLE))
			<< std::endl;
	oss << newLinePrefix << "Number of edges\t\t:\t"
			<< graph->getNumberOfEdges();
	oss
			<< LogStringUtils::impl::connectedEdgeCount(
					graph->getNumberOfEdges(),
					graph->getNumberOfEdges(Connectivity::CONNECTED))
			<< std::flush;
	oss
			<< LogStringUtils::impl::visibleEdgeCount(graph->getNumberOfEdges(),
					graph->getNumberOfEdges(Visibility::VISIBLE)) << std::flush;
	return oss.str();
}

std::string LogStringUtils::graphDescription(GraphIF* const graph,
		const char* newLinePrefix) {
	std::ostringstream oss { };
	oss << newLinePrefix << "Number of vertices\t:\t"
			<< graph->getNumberOfVertices();
	oss
			<< LogStringUtils::impl::visibleVertexCount(
					graph->getNumberOfVertices(),
					graph->getNumberOfVertices(Visibility::VISIBLE))
			<< std::endl;
	oss
			<< LogStringUtils::vertexSetVisualization(graph,
					std::string(newLinePrefix).append("\t").c_str())
			<< std::endl;

	oss << newLinePrefix << "Number of edges\t:\t" << graph->getNumberOfEdges();
	oss
			<< LogStringUtils::impl::connectedEdgeCount(
					graph->getNumberOfEdges(),
					graph->getNumberOfEdges(Connectivity::CONNECTED))
			<< std::flush;
	oss
			<< LogStringUtils::impl::visibleEdgeCount(graph->getNumberOfEdges(),
					graph->getNumberOfEdges(Visibility::VISIBLE)) << std::endl;

	oss
			<< LogStringUtils::edgeSetVisualization(graph,
					std::string(newLinePrefix).append("\t").c_str())
			<< std::flush;
	return oss.str();
}

std::string LogStringUtils::edgeCostSetDescription(
		GraphEdgeCostsIF * const egeCostSet, const char* newLinePrefix) {
	std::ostringstream oss { };
	egeCostSet->begin();
	while (egeCostSet->hasNext()) {
		oss << newLinePrefix << egeCostSet->next() << std::flush;
		oss << (egeCostSet->hasNext() ? ", " : "") << std::flush;
	}
	return oss.str();
}

std::string LogStringUtils::vertexBasicVisualization(VertexIF* const vertex,
		const char* newLinePrefix) {
	std::ostringstream oss { };
	oss << newLinePrefix << "(" << vertex->getVertexIdx() << ")" << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexVisualization(VertexIF* const vertex,
		const char* newLinePrefix) {
	std::ostringstream oss { };
	oss << newLinePrefix << "Vertex ID\t\t\t:\t" << vertex->getVertexIdx()
			<< std::endl;
	oss << newLinePrefix << "Number of input edges\t:\t"
			<< vertex->getNumberOfInputEdges();
	oss
			<< LogStringUtils::impl::visibleEdgeCount(
					vertex->getNumberOfInputEdges(),
					vertex->getNumberOfInputEdges(Visibility::VISIBLE))
			<< std::endl;
	oss << newLinePrefix << "Number of output edges\t:\t"
			<< vertex->getNumberOfInputEdges();
	oss
			<< LogStringUtils::impl::visibleEdgeCount(
					vertex->getNumberOfOutputEdges(),
					vertex->getNumberOfOutputEdges(Visibility::VISIBLE))
			<< std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexSetVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix) {
	std::ostringstream oss { };

	if (graph->hasAnyVertex(visibility)) {
		while (graph->hasNextVertex(visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::vertexBasicVisualization(
							graph->nextVertex());
			if (graph->hasNextVertex(visibility)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_VERTEX_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexSetVisualization(GraphIF* const graph,
		const char* newLinePrefix) {
	return LogStringUtils::vertexSetVisualization(graph, Visibility::BOTH,
			newLinePrefix);
}

std::string LogStringUtils::vertexSetVisualization(VertexSetIF* const vertexSet,
		Visibility const visibility, const char* newLinePrefix) {
	std::ostringstream oss { };

	if (vertexSet->hasAny(visibility)) {
		while (vertexSet->hasNext(visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::vertexBasicVisualization(
							vertexSet->next());
			if (vertexSet->hasNext(visibility)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_VERTEX_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexSetVisualization(VertexSetIF* const vertexSet,
		const char* newLinePrefix) {
	return LogStringUtils::vertexSetVisualization(vertexSet, Visibility::BOTH,
			newLinePrefix);
}

std::string LogStringUtils::vertexSetFlatVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix) {
	std::ostringstream oss { };

	if (graph->hasAnyVertex(visibility)) {
		while (graph->hasNextVertex(visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::vertexBasicVisualization(
							graph->nextVertex());
			if (graph->hasNextVertex(visibility)) {
				oss << "\t" << std::flush;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_VERTEX_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexSetFlatVisualization(GraphIF* const graph,
		const char* newLinePrefix) {
	return LogStringUtils::vertexSetVisualization(graph, Visibility::BOTH,
			newLinePrefix);
}

std::string LogStringUtils::vertexSetFlatVisualization(
		VertexSetIF* const vertexSet, Visibility const visibility,
		const char* newLinePrefix) {
	std::ostringstream oss { };

	if (vertexSet->hasAny(visibility)) {
		while (vertexSet->hasNext(visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::vertexBasicVisualization(
							vertexSet->next());
			if (vertexSet->hasNext(visibility)) {
				oss << "\t" << std::flush;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_VERTEX_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::vertexSetFlatVisualization(
		VertexSetIF* const vertexSet, const char* newLinePrefix) {
	return LogStringUtils::vertexSetFlatVisualization(vertexSet,
			Visibility::BOTH, newLinePrefix);
}

std::string LogStringUtils::edgeVisualization(EdgeIF* const edge,
		const char* newLinePrefix) {
	std::ostringstream oss { };

	oss << newLinePrefix
			<< LogStringUtils::vertexBasicVisualization(edge->getSourceVertex())
			<< "\t";

	switch (edge->getConnectionType()) {
	case EdgeConnectionType::UNDIRECTED:
		oss << LogStringUtils::impl::EDGE_INPUT_ARROW;
		break;
	case EdgeConnectionType::BACKWARD:
		oss << LogStringUtils::impl::EDGE_INPUT_ARROW;
		break;
	default:
		break;
	}

	switch (edge->getVisibility()) {
	case Visibility::BOTH:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::VISIBLE:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::HIDDEN:
		oss << LogStringUtils::impl::EDGE_LINE_HIDDEN;
		break;
	default:
		break;
	}

	oss << " [\t" << edge->getEdgeCost() << "\t] ";

	switch (edge->getVisibility()) {
	case Visibility::BOTH:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::VISIBLE:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::HIDDEN:
		oss << LogStringUtils::impl::EDGE_LINE_HIDDEN;
		break;
	default:
		break;

	}
	switch (edge->getConnectionType()) {
	case EdgeConnectionType::UNDIRECTED:
		oss << LogStringUtils::impl::EDGE_OUTPUT_ARROW;
		break;

	case EdgeConnectionType::FORWARD:
		oss << LogStringUtils::impl::EDGE_OUTPUT_ARROW;
		break;
	default:
		break;
	}

	oss << "\t"
			<< LogStringUtils::vertexBasicVisualization(edge->getTargetVertex())
			<< std::flush;

	return oss.str();
}

std::string LogStringUtils::edgeSetVisualization(GraphIF* const graph,
		Connectivity const connectivity, Visibility const visibility,
		const char* newLinePrefix) {
	std::ostringstream oss { };

	if (graph->hasAnyEdge(connectivity, visibility)) {
		while (graph->hasNextEdge(connectivity, visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::edgeVisualization(graph->nextEdge());
			if (graph->hasNextEdge(connectivity, visibility)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_EDGE_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::edgeSetVisualization(GraphIF* const graph,
		Connectivity const connectivity, const char* newLinePrefix) {
	return LogStringUtils::edgeSetVisualization(graph, connectivity,
			Visibility::BOTH, newLinePrefix);
}

std::string LogStringUtils::edgeSetVisualization(GraphIF* const graph,
		Visibility const visibility, const char* newLinePrefix) {
	return LogStringUtils::edgeSetVisualization(graph, Connectivity::CONNECTED,
			visibility, newLinePrefix);
}

std::string LogStringUtils::edgeSetVisualization(GraphIF* const graph,
		const char* newLinePrefix) {
	return LogStringUtils::edgeSetVisualization(graph, Connectivity::CONNECTED,
			Visibility::BOTH, newLinePrefix);
}

std::string LogStringUtils::edgeSetVisualization(EdgeSetIF* const edgeSet,
		Visibility const visibility, const char* newLinePrefix) {
	std::ostringstream oss { };

	if (edgeSet->hasAny(visibility)) {
		while (edgeSet->hasNext(visibility)) {
			oss << newLinePrefix
					<< LogStringUtils::edgeVisualization(edgeSet->next());
			if (edgeSet->hasNext(visibility)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_EDGE_SET).get());
	}

	oss << std::flush;
	return oss.str();
}

std::string LogStringUtils::edgeSetVisualization(EdgeSetIF* const edgeSet,
		const char* newLinePrefix) {
	return LogStringUtils::edgeSetVisualization(edgeSet, Visibility::BOTH,
			newLinePrefix);
}

std::string LogStringUtils::mstEdgeDifference(EdgeSetIF* const baseMSTSolution,
		EdgeSetIF* const newMSTSolution, const char* newLinePrefix) {
	std::ostringstream oss { };
	VisibilityList newMSTSolutionVisibilityList =
			newMSTSolution->storeVisibility();
	VisibilityList baseMSTSolutionVisibilityList =
			baseMSTSolution->storeVisibility();

	newMSTSolution->showAll();
	baseMSTSolution->hideAll();

	if (newMSTSolution->hasAny(Visibility::VISIBLE)) {
		while (newMSTSolution->hasNext(Visibility::VISIBLE)) {
			oss << newLinePrefix
					<< LogStringUtils::edgeVisualization(
							newMSTSolution->next());
			if (newMSTSolution->hasNext(Visibility::VISIBLE)) {
				oss << std::endl;
			}
		}
	} else {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_DIFF_EDGE_SET).get());
	}

	oss << std::flush;
	newMSTSolution->restoreVisibilityAll(newMSTSolutionVisibilityList);
	baseMSTSolution->restoreVisibilityAll(baseMSTSolutionVisibilityList);
	return oss.str();
}

std::string LogStringUtils::edgeCostChanged(EdgeIF* edge, EdgeCost oldCost,
		EdgeCost newCost, bool forceShowChange, const char* newLinePrefix) {
	std::ostringstream oss { };

	oss << newLinePrefix
			<< LogStringUtils::vertexBasicVisualization(edge->getSourceVertex())
			<< "\t";

	switch (edge->getConnectionType()) {
	case EdgeConnectionType::UNDIRECTED:
		oss << LogStringUtils::impl::EDGE_INPUT_ARROW;
		break;
	case EdgeConnectionType::BACKWARD:
		oss << LogStringUtils::impl::EDGE_INPUT_ARROW;
		break;
	default:
		break;
	}

	switch (edge->getVisibility()) {
	case Visibility::BOTH:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::VISIBLE:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::HIDDEN:
		oss << LogStringUtils::impl::EDGE_LINE_HIDDEN;
		break;
	default:
		break;
	}

	if (forceShowChange || oldCost != newCost) {
		oss << " [\t" << oldCost << "\t"
				<< LogStringUtils::impl::EDGE_CHANGE_COST << "\t" << newCost
				<< "\t] ";
	} else {
		oss << " [\t\t" << oldCost << "\t\t] ";
	}

	switch (edge->getVisibility()) {
	case Visibility::BOTH:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::VISIBLE:
		oss << LogStringUtils::impl::EDGE_LINE_VISIBLE;
		break;
	case Visibility::HIDDEN:
		oss << LogStringUtils::impl::EDGE_LINE_HIDDEN;
		break;
	default:
		break;

	}
	switch (edge->getConnectionType()) {
	case EdgeConnectionType::UNDIRECTED:
		oss << LogStringUtils::impl::EDGE_OUTPUT_ARROW;
		break;

	case EdgeConnectionType::FORWARD:
		oss << LogStringUtils::impl::EDGE_OUTPUT_ARROW;
		break;
	default:
		break;
	}

	oss << "\t"
			<< LogStringUtils::vertexBasicVisualization(edge->getTargetVertex())
			<< std::flush;

	return oss.str();
}

std::string LogStringUtils::edgeCostChanged(EdgeIF* edge, EdgeCost newCost,
		bool forceShowChange, const char* newLinePrefix) {
	return LogStringUtils::edgeCostChanged(edge, edge->getEdgeCost(), newCost,
			forceShowChange, newLinePrefix);
}

std::string LogStringUtils::edgeCostChanged(EdgeIF* edge, EdgeCost newCost,
		const char* newLinePrefix) {
	return LogStringUtils::edgeCostChanged(edge, newCost, false, newLinePrefix);
}

std::string LogStringUtils::edgeSetCostChanged(GraphIF* graph,
		GraphEdgeCostsIF* newGraphCosts, bool onlyChanged,
		const char* newLinePrefix) {
	std::ostringstream oss { };

	EdgeIF* edge { };
	EdgeCost newCost { };
	EdgeCount edgeCounter { 0 };
	std::string tmpString { };

	if (graph->hasAnyEdge()) {
		newGraphCosts->begin();
		while (graph->hasNextEdge()) {
			edge = graph->nextEdge();
			newCost = newGraphCosts->next();
			if (!onlyChanged || edge->getEdgeCost() != newCost) {
				edgeCounter += 1;
				oss << LogStringUtils::edgeCostChanged(edge, newCost, false)
						<< std::endl;
			}
		}
	}

	if (!graph->hasAnyEdge() || edgeCounter == 0) {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_CHANGE_COST_EDGE_SET).get());
		oss << std::flush;
		return oss.str();
	} else {
		tmpString = oss.str();
		tmpString.pop_back();
		return tmpString;
	}
}

std::string LogStringUtils::edgeSetCostChanged(GraphIF* graph,
		GraphEdgeCostsIF* newGraphCosts, const char* newLinePrefix) {
	return LogStringUtils::edgeSetCostChanged(graph, newGraphCosts, true,
			newLinePrefix);
}

std::string LogStringUtils::edgeSetCostChanged(EdgeSetIF* edgeSet,
		GraphEdgeCostsIF* newGraphCosts, bool onlyChanged,
		const char* newLinePrefix) {
	std::ostringstream oss { };

	EdgeIF* edge { };
	EdgeCost newCost { };
	EdgeCount edgeCounter { 0 };
	std::string tmpString { };

	edgeSet->begin();
	newGraphCosts->begin();
	while (edgeSet->hasNext()) {
		edge = edgeSet->next();
		newCost = newGraphCosts->next();
		if (!onlyChanged || edge->getEdgeCost() != newCost) {
			edgeCounter += 1;
			oss << LogStringUtils::edgeCostChanged(edge, newCost, false)
					<< std::endl;
		}
	}

	if (edgeCounter == 0) {
		oss << newLinePrefix
				<< StringUtils::formatMessage(
						LogUtils::impl::getBundle(
								LogBundleKey::LSU_EMPTY_CHANGE_COST_EDGE_SET).get());
		oss << std::flush;
		return oss.str();
	} else {
		tmpString = oss.str();
		tmpString.pop_back();
		return tmpString;
	}
}

std::string LogStringUtils::edgeSetCostChanged(EdgeSetIF* edgeSet,
		GraphEdgeCostsIF* newGraphCosts, const char* newLinePrefix) {
	return LogStringUtils::edgeSetCostChanged(edgeSet, newGraphCosts, true,
			newLinePrefix);
}

template<typename ValueType>
std::string LogStringUtils::wrapInfinity(ValueType value) {
	std::ostringstream oss { };
	if (std::numeric_limits<ValueType>::max() != value) {
		oss << value;
	} else {
		oss << "∞";
	}
	return oss.str();
}

std::string LogStringUtils::ifStatement(bool condition, LogBundleKey itTrueKey,
		LogBundleKey ifFalseKey) {
	return StringUtils::formatMessage(
			LogUtils::impl::getBundle(condition ? itTrueKey : ifFalseKey).get());
}

std::string LogStringUtils::impl::visibleVertexCount(VertexCount overallCount,
		VertexCount visibleCount) {
	std::ostringstream oss { };
	if (overallCount > visibleCount) {
		oss << " (" << visibleCount << " visible)";
	}
	return oss.str();
}

std::string LogStringUtils::impl::connectedEdgeCount(EdgeCount overallCount,
		EdgeCount connectedCount) {
	std::ostringstream oss { };
	if (overallCount > connectedCount) {
		oss << " (" << connectedCount << " connected)";
	}
	return oss.str();
}

std::string LogStringUtils::impl::visibleEdgeCount(EdgeCount overallCount,
		EdgeCount visibleCount) {
	std::ostringstream oss { };
	if (overallCount > visibleCount) {
		oss << " (" << visibleCount << " visible)";
	}
	return oss.str();
}

template std::string LogStringUtils::wrapInfinity<EdgeCost>(EdgeCost value);

