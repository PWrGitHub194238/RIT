/*
 * StringUtils.h
 *
 *  Created on: 3 wrz 2015
 * Author: tomasz
 */

#ifndef LOG_UTILS_HPP_
#define LOG_UTILS_HPP_

#include "Logger.hpp"
#include "../bundle/Bundle.hpp"
#include "../../utils/StringUtils.hpp"

#define TRACE(logger, bundleKey, ...) C11_LOG4CXX_TRACE( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define DEBUG(logger, bundleKey, ...) C11_LOG4CXX_DEBUG( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define INFO(logger, bundleKey, ...) C11_LOG4CXX_INFO( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define WARN(logger, bundleKey, ...) C11_LOG4CXX_WARN( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define ERROR(logger, bundleKey, ...) C11_LOG4CXX_ERROR( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define FATAL(logger, bundleKey, ...) C11_LOG4CXX_FATAL( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define TRACE(logger, bundleKey, ...) C11_LOG4CXX_TRACE( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define DEBUG(logger, bundleKey, ...) C11_LOG4CXX_DEBUG( \
		logger, \
		StringUtils::formatMessage( \
			LogUtils::impl::getBundle(bundleKey), \
		## __VA_ARGS__) \
	)

#define INFO_NOARG(logger, bundleKey) C11_LOG4CXX_INFO( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

#define WARN_NOARG(logger, bundleKey) C11_LOG4CXX_WARN( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

#define ERROR_NOARG(logger, bundleKey) C11_LOG4CXX_ERROR( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

#define FATAL_NOARG(logger, bundleKey) C11_LOG4CXX_FATAL( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

#define TRACE_NOARG(logger, bundleKey) C11_LOG4CXX_TRACE( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

#define DEBUG_NOARG(logger, bundleKey) C11_LOG4CXX_DEBUG( \
		logger, \
		LogUtils::impl::getBundle(bundleKey) \
	)

namespace LogUtils {

/** Loads XML configuration file using DOMConfigurator
 *
 * @param xmlFileConfig path to given XML log4cxx configuration file
 */
void configureLog(const char* xmlFileConfig);

namespace impl {

/** Returns parsed (@see parseBundle()) format bundle file e.g. EN_US_Bundle.cpp
 *
 * Function gets string from extern dictionary in bundle file and returns its parsed version.
 *
 * @param bundleKey
 * @return
 */
const char* getBundle(BundleKey bundleKey);

}

}

#endif /* LOG_UTILS_HPP_ */
