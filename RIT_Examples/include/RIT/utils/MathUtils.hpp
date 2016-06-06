/*
 * MathUtils.hpp
 *
 *  Created on: 19 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_MATHUTILS_HPP_
#define SRC_INCLUDE_UTILS_MATHUTILS_HPP_

#include <cstddef>

#include "../exp/LogicExceptions.hpp"

namespace MathUtils {

template<class RandAccessIter>
double median(RandAccessIter begin, RandAccessIter end)
		throw (LogicExceptions::EmptyStructuredException) {
	if (begin == end) {
		throw LogicExceptions::EmptyStructuredException();
	}
	std::size_t size = end - begin;
	std::size_t middleIdx = size / 2;
	RandAccessIter target = begin + middleIdx;
	std::nth_element(begin, target, end);

	if (size % 2 != 0) { //Odd number of elements
		return *target;
	} else {            //Even number of elements
		return (*target + *std::max_element(begin, target)) / 2.0;
	}
}

}  // namespace MathUtils

#endif /* SRC_INCLUDE_UTILS_MATHUTILS_HPP_ */
