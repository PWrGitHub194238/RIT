/*
 * FormatUtils.hpp
 *
 *  Created on: 7 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_UTILS_FORMATUTILS_HPP_
#define INCLUDE_UTILS_FORMATUTILS_HPP_

namespace FormatUtils {

namespace Tokens {

const char * VertexCountToken { "VertexCount" };
const char* EdgeCountToken { "EdgeCount" };
const char* VertexIdxToken { "VertexIdx" };
const char* EdgeIdxToken { "EdgeIdx" };
const char* EdgeCostToken { "EdgeCost" };

}  // namespace Tokens

namespace Formats {

const char * VertexCountFormat { "u" };
const char * EdgeCountFormat { "u" };
const char * VertexIdxFormat { "u" };
const char * EdgeIdxFormat { "u" };
const char * EdgeCostFormat { "u" };

namespace Lengths {

const unsigned short VertexCountLength { 1 };
const unsigned short EdgeCountLength { 1 };
const unsigned short VertexIdxLength { 1 };
const unsigned short EdgeIdxLength { 1 };
const unsigned short EdgeCostLength { 1 };

}  // namespace Lengths

}  // namespace Formats

}  // namespace FormatUtils

#endif /* INCLUDE_UTILS_FORMATUTILS_HPP_ */
