/*
 * VertexList.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXSET_VERTEXLIST_HPP_
#define INCLUDE_STRUCTURES_VERTEXSET_VERTEXLIST_HPP_

#include <list>
#include <map>

#include "../../enums/Visibility.hpp"
#include "../../typedefs/primitive.hpp"
#include "../VertexIF.hpp"
#include "../VertexSetIF.hpp"

class VertexIF;

class VertexList: public VertexSetIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	std::list<VertexIF*> vertices;

	std::list<VertexIF*>::const_iterator vertexIteratorBegin;

	std::list<VertexIF*>::const_iterator vertexIterator;

	std::map<IteratorId, std::list<VertexIF*>::const_iterator> iteratorMap;

	std::list<VertexIF*>::const_iterator vertexIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(std::list<VertexIF*>::const_iterator & iterator);

	void end(std::list<VertexIF*>::const_iterator & iterator);

	bool hasNext(std::list<VertexIF*>::const_iterator & iterator);

	bool hasNext(std::list<VertexIF*>::const_iterator & iterator,
			Visibility const visibility);

	bool hasPrevious(std::list<VertexIF*>::const_iterator & iterator);

	bool hasPrevious(std::list<VertexIF*>::const_iterator & iterator,
			Visibility const visibility);

	VertexIF * next(std::list<VertexIF*>::const_iterator & iterator);

	VertexIF * current(std::list<VertexIF*>::const_iterator & iterator);

	VertexIF * previous(std::list<VertexIF*>::const_iterator & iterator);

	VertexIF * peek(std::list<VertexIF*>::const_iterator & iterator,
			int moveIndex) throw (LogicExceptions::EmptyIteratorException);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	void createIteratorIfNotExists(IteratorId const iteratorId);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	VertexList(VertexSetIF const & vertexList);

	VertexList();

	VertexList(VertexCount numberOfVertices);

	~VertexList();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	void push_back(VertexIF * const & vertex);

	VertexIF * getElementAt(VertexIdx const vertexIdx);

	VertexCount size() const;

	void begin();

	void begin(IteratorId const iteratorId);

	void end();

	void end(IteratorId const iteratorId);

	bool hasNext();

	bool hasNext(IteratorId const iteratorId);

	bool hasNext(Visibility const visibility);

	bool hasNext(IteratorId const iteratorId, Visibility const visibility);

	bool hasPrevious();

	bool hasPrevious(IteratorId const iteratorId);

	bool hasPrevious(Visibility const visibility);

	bool hasPrevious(IteratorId const iteratorId, Visibility const visibility);

	VertexIF * next();

	VertexIF * next(IteratorId const iteratorId);

	VertexIF * current();

	VertexIF * current(IteratorId const iteratorId);

	VertexIF * previous();

	VertexIF * previous(IteratorId const iteratorId);

	VertexIF * peek(int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	VertexIF * peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	IteratorId getIterator();

	void removeIterator(IteratorId const iteratorId);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_STRUCTURES_VERTEXSET_VERTEXLIST_HPP_ */
