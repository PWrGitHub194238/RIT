/*
 * VertexSetIF.hpp
 *
 *  Created on: 1 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXSETIF_HPP_
#define INCLUDE_STRUCTURES_VERTEXSETIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../exp/LogicExceptions.hpp"
#include "../enums/Visibility.hpp"
#include "../typedefs/primitive.hpp"
#include "VertexIF.hpp"
#include "VisibleIterableIF.hpp"

class VertexIF;

class VertexSetIF: public VisibleIterable<VertexIF*>, public JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	VertexCount numberOfVertices;

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual void createIteratorIfNotExists(IteratorId const iteratorId) = 0;

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	VertexSetIF();

	VertexSetIF(VertexCount numberOfVertices);

	virtual ~VertexSetIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual void push_back(VertexIF * const & vertex) = 0;

	virtual VertexIF * getElementAt(VertexIdx const vertexIdx) = 0;

	virtual VertexCount size() const = 0;

	VertexCount size(Visibility const visibility);

	VertexCount size(IteratorId const iteratorId, Visibility const visibility);

	virtual void begin() = 0;

	virtual void begin(IteratorId const iteratorId) = 0;

	virtual void end() = 0;

	virtual void end(IteratorId const iteratorId) = 0;

	virtual bool hasNext() = 0;

	virtual bool hasNext(IteratorId const iteratorId) = 0;

	virtual bool hasNext(Visibility const visibility) = 0;

	virtual bool hasNext(IteratorId const iteratorId,
			Visibility const visibility) = 0;

	virtual bool hasPrevious() = 0;

	virtual bool hasPrevious(IteratorId const iteratorId) = 0;

	virtual bool hasPrevious(Visibility const visibility) = 0;

	virtual bool hasPrevious(IteratorId const iteratorId,
			Visibility const visibility) = 0;

	/**
	 *
	 * @return element wskazany przez iterator, zwiększy iterator
	 */
	virtual VertexIF * next() = 0;

	virtual VertexIF * next(IteratorId const iteratorId) = 0;

	virtual VertexIF * current() = 0;

	virtual VertexIF * current(IteratorId const iteratorId) = 0;

	virtual VertexIF * previous() = 0;

	virtual VertexIF * previous(IteratorId const iteratorId) = 0;

	virtual VertexIF * peek(int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	virtual VertexIF * peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	virtual IteratorId getIterator() = 0;

	virtual void removeIterator(IteratorId const iteratorId) = 0;

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	virtual std::string toString();

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_STRUCTURES_VERTEXSETIF_HPP_ */
