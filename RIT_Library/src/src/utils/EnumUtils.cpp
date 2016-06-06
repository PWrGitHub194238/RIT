/*
 * EnumUtils.cpp
 *
 *  Created on: 8 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/EnumUtils.hpp"

#include <log4cxx/logger.h>

#include "../../include/enums/EdgeByVertexKey.hpp"
#include "../../include/enums/EdgeConnectionType.hpp"
#include "../../include/enums/Visibility.hpp"
#include "../../include/utils/enums/GraphVizEngine.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.EnumUtils"));

namespace EnumUtils {

namespace impl {

const char* edgeByVertexKey[] = { "incoming", "outgoing" };

const char* edgeConnectionType[] = { "undirected", "forward", "backward",
		"unconnected" };

const char* edgeVisibility[] = { "hidden", "visible", "all" };

const char* graphVizEngine[] = { "circo", "dot", "fdp", "neato", "osage",
		"sfdp", "twopi" };

const char* lpVariableType[] = { "boolean", "integer", "float" };

}  // namespace impl

}  // namespace EnumUtils

const char* EnumUtils::getEdgeByVertexKeyString(EdgeByVertexKey key) {
	return EnumUtils::impl::edgeByVertexKey[static_cast<unsigned int>(key)];
}

const char* EnumUtils::getEdgeConnectionTypeString(EdgeConnectionType key) {
	return EnumUtils::impl::edgeConnectionType[static_cast<unsigned int>(key)];
}

const char* EnumUtils::getVisibilityString(Visibility key) {
	return EnumUtils::impl::edgeVisibility[static_cast<unsigned int>(key)];
}

const char* EnumUtils::getGraphVizEngineString(GraphVizEngine key) {
	return EnumUtils::impl::graphVizEngine[static_cast<unsigned int>(key)];
}

const char* EnumUtils::getLPVariableTypeString(unsigned int key) {
	return EnumUtils::impl::lpVariableType[key];
}

