/*
 * EnumUtils.hpp
 *
 *  Created on: 8 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_ENUMUTILS_HPP_
#define SRC_INCLUDE_UTILS_ENUMUTILS_HPP_

#include "../enums/EdgeByVertexKey.hpp"
#include "../enums/EdgeConnectionType.hpp"
#include "../enums/Visibility.hpp"
#include "enums/GraphVizEngine.hpp"

namespace EnumUtils {

const char* getEdgeByVertexKeyString(EdgeByVertexKey key);

const char* getEdgeConnectionTypeString(EdgeConnectionType key);

const char* getVisibilityString(Visibility key);

const char* getGraphVizEngineString(GraphVizEngine key);

const char* getLPVariableTypeString(unsigned int key);

}  // namespace EnumUtils

#endif /* SRC_INCLUDE_UTILS_ENUMUTILS_HPP_ */
