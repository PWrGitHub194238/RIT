/*
 * GraphEdgeCostsIF.cpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#include "../../include/structures/GraphEdgeCostsIF.hpp"

#include <log4cxx/logger.h>
#include <rapidjson/rapidjson.h>
#include <sstream>

#include "../../include/structures/EdgeSetIF.hpp"
#include "../../include/structures/GraphIF.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.GraphEdgeCostsIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

GraphEdgeCostsIF::GraphEdgeCostsIF() :
		GraphEdgeCostsIF(
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(std::string scenarioName) :
		numberOfEdges { 0 }, name { scenarioName } {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(GraphIF* const graph) :
		GraphEdgeCostsIF(graph,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {

}

GraphEdgeCostsIF::GraphEdgeCostsIF(GraphIF* const graph,
		std::string scenarioName) :
		numberOfEdges { graph->getNumberOfEdges() }, name { scenarioName } {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(EdgeSetIF* const edgeSet) :
		GraphEdgeCostsIF(edgeSet,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(EdgeSetIF* const edgeSet,
		std::string scenarioName) :
		numberOfEdges { edgeSet->size() }, name { scenarioName } {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(EdgeCount numberOfEdges) :
		GraphEdgeCostsIF(numberOfEdges,
				BundleUtils::getString(AppBundleKey::DEFAULT_SCENARIO_NAME)) {
}

GraphEdgeCostsIF::GraphEdgeCostsIF(EdgeCount numberOfEdges,
		std::string scenarioName) :
		numberOfEdges { numberOfEdges }, name { scenarioName } {
}

// Empty virtual destructor for proper cleanup
GraphEdgeCostsIF::~GraphEdgeCostsIF() {

}

//*************************************** PUBLIC FUNCTIONS *****************************************//

EdgeCost GraphEdgeCostsIF::getTotalEdgeCost() {
	EdgeCost summaryCost { 0 };
	begin();
	while (hasNext()) {
		summaryCost += next();
	}
	begin();
	return summaryCost;
}

void GraphEdgeCostsIF::fillJSON(rapidjson::Document& jsonDoc,
		rapidjson::Document::AllocatorType& allocator, unsigned short depth) {
	rapidjson::Value edgeCostSet(rapidjson::kArrayType);

	jsonDoc.AddMember("Scenario name", rapidjson::StringRef(name.c_str()),
			allocator);
	jsonDoc.AddMember("Number of edges", numberOfEdges, allocator);

	begin();
	while (hasNext()) {
		edgeCostSet.PushBack(next(), allocator);
	}

	jsonDoc.AddMember("Edge cost set", edgeCostSet, allocator);
}

std::string GraphEdgeCostsIF::toString() {
	std::ostringstream oss { };
	oss << "Set named '" << name << "' of edge costs has " << numberOfEdges
			<< " edges:" << std::endl;

	begin();
	while (hasNext()) {
		oss << "\t" << next() << std::flush;
		oss << (hasNext() ? ", " : "") << std::flush;
	}
	begin();
	return oss.str();
}

//*************************************** GETTERS & SETTERS ****************************************//

std::string GraphEdgeCostsIF::getName() const {
	return this->name;
}
