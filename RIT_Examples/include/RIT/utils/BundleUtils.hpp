/*
 * BundleUtils.hpp
 *
 *  Created on: 9 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_BUNDLEUTILS_HPP_
#define SRC_INCLUDE_UTILS_BUNDLEUTILS_HPP_

#include <memory>
#include <string>

#include "../bundle/Bundle.hpp"

namespace BundleUtils {

std::string getString(AppBundleKey key);

namespace impl {

/** Returns parsed (@see parseBundle()) format from application bundle file e.g. src/bundle/EN_US_Bundle.cpp
 *
 * Function gets string from extern dictionary in bundle file and returns its parsed version i.e.
 * it takes bundle raw format from e.g. EN_US_Bundle.cpp from it's dictionary (it looks for right 'bundleKey'),
 * it replaces every custom defined formatter to valid one.
 *
 * @param bundleKey key for dictionary in file containing bundles
 * @return unique pointer to final format - that format will be destroyed (thanks to smart pointer) as soon as it will be used.
 */
std::unique_ptr<char[]> getBundle(AppBundleKey bundleKey);

}  // namespace impl

}  // namespace BundleUtils

#endif /* SRC_INCLUDE_UTILS_BUNDLEUTILS_HPP_ */
