/*
 * JSONUtils.cpp
 *
 *  Created on: 9 lut 2016
 *      Author: tomasz
 */

#include "../../include/utils/JSONUtils.hpp"

#include <log4cxx/logger.h>
#include <rapidjson/document.h>

#include "../../include/structures/JSONIF.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("utils.JSONUtils"));

rapidjson::Document JSONUtils::getDepthLimitedJSON(JSONIF* const jsonClass,
		rapidjson::Document::AllocatorType& allocator, const char* overdepthKey,
		unsigned short depth) {
	rapidjson::Document jsonDoc(&allocator);
	jsonDoc.SetObject();
	if (depth > 0) {
		jsonClass->fillJSON(jsonDoc, allocator, depth - 1);
		return jsonDoc;
	} else {
		rapidjson::Value key { };
		key.SetString(overdepthKey, allocator);
		jsonDoc.AddMember(key, "{ ... }", allocator);
		return jsonDoc;
	}
}
