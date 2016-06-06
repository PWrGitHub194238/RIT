/*
 * EnumUtils.cpp

 *
 *  Created on: 3 mar 2016
 *      Author: tomasz
 */

#include "../../../include/log/utils/LocaleEnum.hpp"

namespace EnumUtils {

namespace impl {

const char* localeEnum[] = { "en_US.utf8" };

}  // namespace impl

const char* getLocaleEnumString(LocaleEnum key) {
	return EnumUtils::impl::localeEnum[static_cast<unsigned int>(key)];
}

}  // namespace EnumUtils

