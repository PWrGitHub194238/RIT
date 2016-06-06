/*
 * VisibleElementIF.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_VISIBLEELEMENTIF_HPP_
#define SRC_INCLUDE_STRUCTURES_VISIBLEELEMENTIF_HPP_

#include "../enums/Visibility.hpp"

class VisibleElement {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	Visibility visibility;

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	virtual ~VisibleElement() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	void hide();

	void show();

	//*************************************** GETTERS & SETTERS ****************************************//

	bool isHidden() const;

	bool isVisible() const;

	bool isBoth() const;

	Visibility getVisibility() const;
};
#endif /* SRC_INCLUDE_STRUCTURES_VISIBLEELEMENTIF_HPP_ */
