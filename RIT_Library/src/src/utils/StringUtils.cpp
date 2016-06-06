/*
 * StringUtils.cpp
 *
 *  Created on: 7 sty 2016
 *      Author: tomasz
 */

#include "../../include/utils/StringUtils.hpp"

#include <log4cxx/logger.h>
#include <stddef.h>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>

#include "../../include/utils/FormatUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.StringUtils"));

const char StringUtils::SPECIAL_SIGN { '%' };

const int StringUtils::MESSAGE_BUFFER_SIZE { 10240 };

std::unique_ptr<char[]> StringUtils::parseStringFormatSpecifiers(
		const char* const formatString) {
	std::unique_ptr<char[]> c_str { };
	std::string str(formatString);
	size_t b { 0 }, e { 0 };
	do {
		b = str.find_first_of(StringUtils::SPECIAL_SIGN, e);
		if (b == std::string::npos) {
			break;
		}
		e = str.find_first_of(StringUtils::SPECIAL_SIGN, b + 1);
		if (e == std::string::npos) {
			break;
		}
		if (e - b == 1) {
			e += 1;
			continue;
		}
		e = StringUtils::impl::insertRightFormat(str, b + 1, e - 1);
	} while (e != std::string::npos);
	c_str = std::unique_ptr<char[]> { new char[str.size() + 1] { } }; // +1 for terminating NUL
	strcpy(c_str.get(), str.c_str());
	return c_str;
}

const char* StringUtils::formatMessage(const char* format, ...) {
	va_list va { };
	static char formatted[MESSAGE_BUFFER_SIZE] { };
	va_start(va, format);
	vsnprintf(formatted, MESSAGE_BUFFER_SIZE, format, va);
	va_end(va);
	return formatted;
}

size_t StringUtils::impl::insertRightFormat(std::string& sourceFormat,
		size_t const & beginIdx, size_t const & endIdx) {
	const size_t length = endIdx - beginIdx + 1;
	std::string subStr = sourceFormat.substr(beginIdx, length);

	if (subStr.compare(FormatUtils::Tokens::VertexCountToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::VertexCountFormat);
		return beginIdx + FormatUtils::Formats::Lengths::VertexCountLength;
	} else if (subStr.compare(FormatUtils::Tokens::VertexKeytToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::VertexKeyFormat);
		return beginIdx + FormatUtils::Formats::Lengths::VertexKeyLength;
	} else if (subStr.compare(FormatUtils::Tokens::EdgeCountToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::EdgeCountFormat);
		return beginIdx + FormatUtils::Formats::Lengths::EdgeCountLength;
	} else if (subStr.compare(FormatUtils::Tokens::VertexIdxToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::VertexIdxFormat);
		return beginIdx + FormatUtils::Formats::Lengths::VertexIdxLength;
	} else if (subStr.compare(FormatUtils::Tokens::EdgeIdxToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::EdgeIdxFormat);
		return beginIdx + FormatUtils::Formats::Lengths::EdgeIdxLength;
	} else if (subStr.compare(FormatUtils::Tokens::SignedEdgeIdxToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::SignedEdgeIdxFormat);
		return beginIdx + FormatUtils::Formats::Lengths::SignedEdgeIdxLength;
	} else if (subStr.compare(FormatUtils::Tokens::EdgeCostToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::EdgeCostFormat);
		return beginIdx + FormatUtils::Formats::Lengths::EdgeCostLength;
	} else if (subStr.compare(FormatUtils::Tokens::IOEdgeCostToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::IOEdgeCostFormat);
		return beginIdx + FormatUtils::Formats::Lengths::IOEdgeCostLength;
	} else if (subStr.compare(FormatUtils::Tokens::LambdaValueToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::LambdaValueFormat);
		return beginIdx + FormatUtils::Formats::Lengths::LambdaValueLength;
	} else if (subStr.compare(FormatUtils::Tokens::LambdaIdxToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::LambdaIdxFormat);
		return beginIdx + FormatUtils::Formats::Lengths::LambdaIdxLength;
	} else if (subStr.compare(FormatUtils::Tokens::LambdaCountToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::LambdaCountFormat);
		return beginIdx + FormatUtils::Formats::Lengths::LambdaCountLength;
	} else if (subStr.compare(FormatUtils::Tokens::IteratorIdToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::IteratorIdFormat);
		return beginIdx + FormatUtils::Formats::Lengths::IteratorIdLength;
	} else if (subStr.compare(FormatUtils::Tokens::TabuIterationCountToken)
			== 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::TabuIterationCountFormat);
		return beginIdx
				+ FormatUtils::Formats::Lengths::TabuIterationCountLength;
	} else if (subStr.compare(FormatUtils::Tokens::LPUIntValueToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::LPUIntValueFormat);
		return beginIdx + FormatUtils::Formats::Lengths::LPUIntValueLength;
	} else if (subStr.compare(FormatUtils::Tokens::LPFloatValueToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::LPFloatValueFormat);
		return beginIdx + FormatUtils::Formats::Lengths::LPFloatValueLength;
	} else if (subStr.compare(FormatUtils::Tokens::ThreadIdToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::ThreadIdFormat);
		return beginIdx + FormatUtils::Formats::Lengths::ThreadIdLength;
	} else if (subStr.compare(FormatUtils::Tokens::ScenarioIdxToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::ScenarioIdxFormat);
		return beginIdx + FormatUtils::Formats::Lengths::ScenarioIdxLength;
	} else if (subStr.compare(FormatUtils::Tokens::ScenarioCountToken) == 0) {
		sourceFormat.replace(beginIdx, length + 1,
				FormatUtils::Formats::ScenarioCountFormat);
		return beginIdx + FormatUtils::Formats::Lengths::ScenarioCountLength;
	} else {
		return endIdx;
	}
}
