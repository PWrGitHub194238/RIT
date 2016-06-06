/*
 * AIMSTUtils.cpp
 *
 *  Created on: 2 maj 2016
 *      Author: tomasz
 */

#include "../../include/utils/AIMSTUtils.hpp"

#include <tuple>

#include "../../include/structures/EdgeIF.hpp"
#include "../../include/structures/EdgeSetInclude.hpp"
#include "../../include/structures/GraphIF.hpp"
#include "../../include/structures/VertexIF.hpp"
#include "../../include/typedefs/primitive.hpp"
#include "../../include/typedefs/struct.hpp"

extern const bool AIMSTUtils::MULTITHREAD { true };

EdgeSetIF* AIMSTUtils::getEdgeSetCopy(GraphIF* const graph,
		EdgeSetIF* const edgeSet) {
	EdgeIF* edge { };
	EdgeSetIF* newEdgeSet = new EdgeSetImpl { edgeSet->size() };
	edgeSet->begin();
	while (edgeSet->hasNext()) {
		edge = edgeSet->next();
		newEdgeSet->push_back(
				graph->findEdge(edge->getSourceVertex()->getVertexIdx(),
						edge->getTargetVertex()->getVertexIdx()));
	}
	return newEdgeSet;
}

AIMSTSolution AIMSTUtils::createAIMSTSolution(EdgeSetIF* const solution,
		GraphEdgeCostsIF* const scenario, EdgeCost const solutionCost) {
	return std::make_tuple(solution, scenario, solutionCost);
}

EdgeSetIF* AIMSTUtils::getEdgeSet(AIMSTSolution aimstSolution) {
	return std::get<0>(aimstSolution);
}

GraphEdgeCostsIF* AIMSTUtils::getScenario(AIMSTSolution aimstSolution) {
	return std::get<1>(aimstSolution);
}

EdgeCost AIMSTUtils::getSolutionCost(AIMSTSolution aimstSolution) {
	return std::get<2>(aimstSolution);
}

