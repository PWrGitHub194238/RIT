/*
 * JSONIF.cpp
 *
 *  Created on: 8 lut 2016
 *      Author: tomasz
 */

#include "../../include/structures/JSONIF.hpp"

#include <log4cxx/logger.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "../../include/bundle/Bundle.hpp"
#include "../../include/utils/BundleUtils.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.JSONIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

rapidjson::Document JSONIF::getJSON(unsigned short depth) {
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();
	fillJSON(jsonDoc, jsonDoc.GetAllocator(), depth);
	return jsonDoc;
}

std::string JSONIF::toJSONString(unsigned short depth) {
	rapidjson::StringBuffer buffer;
	rapidjson::Document jsonDoc = getJSON(depth);
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jsonDoc.Accept(writer);
	return buffer.GetString();
}

std::string JSONIF::toPrettyJSONString(unsigned short depth) {
	rapidjson::StringBuffer buffer;
	rapidjson::Document jsonDoc;
	jsonDoc.SetObject();
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	jsonDoc = getJSON(depth);
	jsonDoc.Accept(writer);

	return buffer.GetString();
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

JSONIF::JSONIF() {
}

// Empty virtual destructor for proper cleanup
JSONIF::~JSONIF() {
}

//*************************************** PUBLIC FUNCTIONS *****************************************//

std::string JSONIF::toJSONString(JSONFormat format, unsigned short depth) {
	switch (format) {
	case JSONFormat::NONE:
		return toJSONString(depth);
	case JSONFormat::PRETTY:
		return toPrettyJSONString(depth);
	default:
		return toJSONString(depth);
	}
}

std::string JSONIF::toString() {
	return BundleUtils::getString(AppBundleKey::JSONIF_TO_STRING_UNIMPLEMENTED);
}
//*************************************** GETTERS & SETTERS ****************************************//

