/*
 * Vertex.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEX_VERTEX_HPP_
#define INCLUDE_STRUCTURES_VERTEX_VERTEX_HPP_

#include "../../typedefs/primitive.hpp"
#include "../VertexIF.hpp"

class Vertex: public VertexIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	Vertex(VertexIF* vertex);

	Vertex(VertexIdx vertexIdx, Visibility visibility);

	Vertex(VertexIdx vertexIdx);

	virtual ~Vertex();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_STRUCTURES_VERTEX_VERTEX_HPP_ */
