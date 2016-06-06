/*
 * LogGraphUtils.cpp
 *
 *  Created on: 12 mar 2016
 *      Author: tomasz
 */

#include "../../../include/log/utils/LogGraphUtils.hpp"

#include "../../../include/exp/LogicExceptions.hpp"
#include "../../../include/structures/GraphIF.hpp"
#include "../../../include/structures/VertexIF.hpp"
#include "../../../include/typedefs/primitive.hpp"

VertexIdx LogGraphUtils::peekPreviousVertexIdx(GraphIF * const graph) {
	try {
		return graph->peekPreviousVertex()->getVertexIdx();
	} catch (LogicExceptions::EmptyIteratorException & e) {
		return 0;
	}
}
