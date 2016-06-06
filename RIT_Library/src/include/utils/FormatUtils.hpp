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
const char * VertexKeytToken { "VertexKey" };
const char * EdgeCountToken { "EdgeCount" };
const char * VertexIdxToken { "VertexIdx" };
const char * EdgeIdxToken { "EdgeIdx" };
const char * SignedEdgeIdxToken { "SEdgeIdx" };
const char * EdgeCostToken { "EdgeCost" };
const char * IOEdgeCostToken { "IOEdgeCost" };
const char * LambdaValueToken { "LambdaValue" };
const char * LambdaIdxToken { "LambdaIdx" };
const char * LambdaCountToken { "LambdaCount" };
const char * IteratorIdToken { "IteratorId" };
const char * TabuIterationCountToken { "TabuIterationCount" };
const char * LPUIntValueToken { "LPUIntValue" };
const char * LPFloatValueToken { "LPFloatValue" };
const char * ThreadIdToken { "ThreadId" };
const char * ScenarioIdxToken { "ScenarioIdx" };
const char * ScenarioCountToken { "ScenarioCount" };

}  // namespace Tokens

namespace Formats {

const char * VertexCountFormat { "u" };
const char * VertexKeyFormat { ".3lf" };
const char * EdgeCountFormat { "u" };
const char * VertexIdxFormat { "u" };
const char * EdgeIdxFormat { "u" };
const char * SignedEdgeIdxFormat { "d" };
const char * EdgeCostFormat { ".9lf" };
const char * IOEdgeCostFormat { "lf" };
const char * LambdaValueFormat { ".9lf" };
const char * LambdaIdxFormat { "u" };
const char * LambdaCountFormat { "u" };
const char * IteratorIdFormat { "u" };
const char * TabuIterationCountFormat { "u" };
const char * LPUIntValueFormat { "u" };
const char * LPFloatValueFormat { ".3lf" };
const char * ThreadIdFormat { "u" };
const char * ScenarioIdxFormat { "u" };
const char * ScenarioCountFormat { "u" };

namespace Lengths {

const unsigned short VertexCountLength { 1 };
const unsigned short VertexKeyLength { 4 };
const unsigned short EdgeCountLength { 1 };
const unsigned short VertexIdxLength { 1 };
const unsigned short EdgeIdxLength { 1 };
const unsigned short SignedEdgeIdxLength { 1 };
const unsigned short EdgeCostLength { 4 };
const unsigned short IOEdgeCostLength { 2 };
const unsigned short LambdaValueLength { 4 };
const unsigned short LambdaIdxLength { 1 };
const unsigned short LambdaCountLength { 1 };
const unsigned short IteratorIdLength { 1 };
const unsigned short TabuIterationCountLength { 1 };
const unsigned short LPUIntValueLength { 1 };
const unsigned short LPFloatValueLength { 4 };
const unsigned short ThreadIdLength { 1 };
const unsigned short ScenarioIdxLength { 1 };
const unsigned short ScenarioCountLength { 1 };

}  // namespace Lengths

}  // namespace Formats

}  // namespace FormatUtils

#endif /* INCLUDE_UTILS_FORMATUTILS_HPP_ */
