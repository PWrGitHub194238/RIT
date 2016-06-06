/*
 * LogGraphUtils.hpp
 *
 *  Created on: 12 mar 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_LOG_UTILS_LOGGRAPHUTILS_HPP_
#define SRC_INCLUDE_LOG_UTILS_LOGGRAPHUTILS_HPP_

#include "../../typedefs/primitive.hpp"

class GraphIF;

namespace LogGraphUtils {

VertexIdx peekPreviousVertexIdx(GraphIF * const graph);

}  // namespace LogGraphUtils

#endif /* SRC_INCLUDE_LOG_UTILS_LOGGRAPHUTILS_HPP_ */
