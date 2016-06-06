/*
 * JSONIF.hpp
 *
 *  Created on: 8 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_JSONIF_HPP_
#define SRC_INCLUDE_STRUCTURES_JSONIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../utils/enums/JSONFormat.hpp"

class JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	rapidjson::Document getJSON(unsigned short depth);

	std::string toJSONString(unsigned short depth);

	std::string toPrettyJSONString(unsigned short depth);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	JSONIF();

	virtual ~JSONIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth) = 0;

	std::string toJSONString(JSONFormat format, unsigned short depth);

	virtual std::string toString();

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_STRUCTURES_JSONIF_HPP_ */
