/*
 * JSONUtils.hpp
 *
 *  Created on: 9 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_JSONUTILS_HPP_
#define SRC_INCLUDE_UTILS_JSONUTILS_HPP_

#include <rapidjson/document.h>

class JSONIF;

namespace JSONUtils {

rapidjson::Document getDepthLimitedJSON(JSONIF* const jsonClass,
		rapidjson::Document::AllocatorType& allocator, const char* overdepthKey,
		unsigned short depth);

}  // namespace JSONUtils

#endif /* SRC_INCLUDE_UTILS_JSONUTILS_HPP_ */
