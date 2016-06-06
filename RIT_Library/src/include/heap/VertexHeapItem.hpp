/*
 * VertexHeapItem.hpp
 *
 *  Created on: 11 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_HEAP_VERTEXHEAPITEM_HPP_
#define SRC_INCLUDE_HEAP_VERTEXHEAPITEM_HPP_

#include <rapidjson/document.h>

#include "../structures/JSONIF.hpp"
#include "../typedefs/primitive.hpp"

class VertexIF;

class VertexHeapItem: public JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	VertexKey key;
	VertexIF * value;
	VertexIF * predecessor;

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	VertexHeapItem();

	VertexHeapItem(VertexIF * vertex);

	VertexHeapItem(VertexIF * vertex, VertexKey key);

	VertexHeapItem(VertexIF * vertex, VertexKey const key,
			VertexIF * predecessor);

	virtual ~VertexHeapItem();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	//*************************************** GETTERS & SETTERS ****************************************//

	VertexKey getKey() const;

	void setKey(VertexKey key);

	VertexIF * getValue() const;

	VertexIF * getPredecessor() const;

	void setPredecessor(VertexIF * predecessor);

};

#endif /* SRC_INCLUDE_HEAP_VERTEXHEAPITEM_HPP_ */
