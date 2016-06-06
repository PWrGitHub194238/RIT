/*
 * VisibleElementIF.cpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#include "../../include/structures/VisibleElementIF.hpp"

#include <log4cxx/logger.h>

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("structures.VisibleElementIF"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

//*************************************** PUBLIC FUNCTIONS *****************************************//

void VisibleElement::hide() {
	this->visibility = Visibility::HIDDEN;
}

void VisibleElement::show() {
	this->visibility = Visibility::VISIBLE;
}

bool VisibleElement::isHidden() const {
	return (this->visibility == Visibility::HIDDEN);
}

bool VisibleElement::isVisible() const {
	return (this->visibility == Visibility::VISIBLE);
}

Visibility VisibleElement::getVisibility() const {
	return this->visibility;
}

//*************************************** GETTERS & SETTERS ****************************************//

