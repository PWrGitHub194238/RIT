/*
 * BundleUtils.cpp
 *
 *  Created on: 9 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/BundleUtils.hpp"

#include <log4cxx/logger.h>
#include <memory>
#include <string>

#include "../../include/bundle/Bundle.hpp"
#include "../../include/utils/StringUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.BundleUtils"));

std::string BundleUtils::getString(AppBundleKey key) {
	return StringUtils::formatMessage(BundleUtils::impl::getBundle(key).get());
}

std::unique_ptr<char[]> BundleUtils::impl::getBundle(AppBundleKey bundleKey) {
	return StringUtils::parseStringFormatSpecifiers(appDictionary[bundleKey]);
}
