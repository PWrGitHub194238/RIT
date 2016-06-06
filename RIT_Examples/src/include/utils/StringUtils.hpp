/*
 * StringUtils.hpp
 *
 *  Created on: 7 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_UTILS_STRINGUTILS_HPP_
#define INCLUDE_UTILS_STRINGUTILS_HPP_

#include <stddef.h>
#include <string>

class GraphIF;

namespace StringUtils {

const static char SPECIAL_SIGN = { '%' };

const static int MESSAGE_BUFFER_SIZE { 1024 };

/** @brief Function replaces all occurrences of user-defined formatters with valid ones.
 *
 * @details Example with defined extern fields in bundle file e.g. EN_US_Bundle.cpp:
 * @code
 * 	const char* varIdxToken = "VarIdx";
 *	extern const std::string varIdxFormat("u");
 * @endcode
 *
 * To make user-defined formatters recognizable, they have to be surrounded by SPECIAL_SIGN:
 *
 * @code
 * 	"This is an example value: %VarIdx% that will be presented as: %u."
 * 	will be translated to:
 * 	"This is an example value: %u that will be presented as: %u."
 * @endcode
 *
 * @param formatString format string to be formatted
 * @return formatted string with all user-defined formatters replaced by formatters that were assigned to them.
 */
const char* parseStringFormatSpecifiers(const char* const formatString);

/** @brief Puts all passed arguments in given format and return final string.
 *
 * @param format
 * @return
 */
const char* formatMessage(const char* format, ...);

namespace Impl {

/** @brief Replaces one of user-defined formatter to valid format that were assigned to it.
 *
 * @param bundle bundle to be parsed
 * @param beginIdx index of the first character after SPECIAL_SIGN of user-defined formatter
 * @param endIdx index of the last character before SPECIAL_SIGN of user-defined formatter
 * @return index of a character after second SPECIAL_SIGN (or endIdx if there is no match)
 */
size_t insertRightFormat(std::string& sourceFormat, size_t const & beginIdx,
		size_t const & endIdx);

}  // namespace Impl

}  // namespace StringUtils

#endif /* INCLUDE_UTILS_STRINGUTILS_HPP_ */
