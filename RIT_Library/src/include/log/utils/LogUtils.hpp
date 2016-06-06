/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 * Author: tomasz
 */

#ifndef LOG_UTILS_HPP_
#define LOG_UTILS_HPP_

#include <memory>

#include "../bundle/Bundle.hpp"

#include "../../utils/StringUtils.hpp"
#include "../utils/Logger.hpp"
#include "LocaleEnum.hpp"

class VertexIF;

#define TRACE(logger, bundleKey, ...) C11_LOG4CXX_TRACE( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define DEBUG(logger, bundleKey, ...) C11_LOG4CXX_DEBUG( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define INFO(logger, bundleKey, ...) C11_LOG4CXX_INFO( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define WARN(logger, bundleKey, ...) C11_LOG4CXX_WARN( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define ERROR(logger, bundleKey, ...) C11_LOG4CXX_ERROR( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define FATAL(logger, bundleKey, ...) C11_LOG4CXX_FATAL( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define TRACE(logger, bundleKey, ...) C11_LOG4CXX_TRACE( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define DEBUG(logger, bundleKey, ...) C11_LOG4CXX_DEBUG( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey).get(), \
		## __VA_ARGS__) \
	)

#define INFO_NOARG(logger, bundleKey) C11_LOG4CXX_INFO( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

#define WARN_NOARG(logger, bundleKey) C11_LOG4CXX_WARN( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

#define ERROR_NOARG(logger, bundleKey) C11_LOG4CXX_ERROR( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

#define FATAL_NOARG(logger, bundleKey) C11_LOG4CXX_FATAL( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

#define TRACE_NOARG(logger, bundleKey) C11_LOG4CXX_TRACE( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

#define DEBUG_NOARG(logger, bundleKey) C11_LOG4CXX_DEBUG( \
		logger, \
		LogUtils::impl::getBundle(bundleKey).get() \
	)

namespace LogUtils {

/** Loads XML configuration file using DOMConfigurator
 *
 * @param xmlFileConfig path to given XML log4cxx configuration file
 */
void configureLog(const char* xmlFileConfig);

void setLocale(LocaleEnum const locale);

namespace impl {

/** Returns parsed (@see parseBundle()) format from logging bundle file e.g. src/log/bundle/EN_US_Bundle.cpp
 *
 * Function gets string from extern dictionary in bundle file and returns its parsed version i.e.
 * it takes bundle raw format from e.g. EN_US_Bundle.cpp from it's dictionary (it looks for right 'bundleKey'),
 * it replaces every custom defined formatter to valid one.
 *
 * @param bundleKey key for dictionary in file containing bundles
 * @return unique pointer to final format - that format will be destroyed (thanks to smart pointer) as soon as it will be used.
 */
std::unique_ptr<char[]> getBundle(LogBundleKey bundleKey);

}  // namespace impl

}  // namespace LogUtils

#endif /* LOG_UTILS_HPP_ */
