/*
 * VertexArray.hpp
 *
 *  Created on: 1 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXSET_VERTEXARRAY_HPP_
#define INCLUDE_STRUCTURES_VERTEXSET_VERTEXARRAY_HPP_

#include <map>
#include <vector>

#include "../../enums/Visibility.hpp"
#include "../../typedefs/primitive.hpp"
#include "../VertexIF.hpp"
#include "../VertexSetIF.hpp"

class VertexArray: public VertexSetIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	std::vector<VertexIF*> vertices;

	std::vector<VertexIF*>::const_iterator vertexIteratorBegin;

	std::vector<VertexIF*>::const_iterator vertexIterator;

	std::map<IteratorId, std::vector<VertexIF*>::const_iterator> iteratorMap;

	std::vector<VertexIF*>::const_iterator vertexIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(std::vector<VertexIF*>::const_iterator & iterator);

	void end(std::vector<VertexIF*>::const_iterator & iterator);

	bool hasNext(std::vector<VertexIF*>::const_iterator & iterator);

	bool hasNext(std::vector<VertexIF*>::const_iterator & iterator,
			Visibility const visibility);

	bool hasPrevious(std::vector<VertexIF*>::const_iterator & iterator);

	bool hasPrevious(std::vector<VertexIF*>::const_iterator & iterator,
			Visibility const visibility);

	VertexIF * next(std::vector<VertexIF*>::const_iterator & iterator);

	VertexIF * current(std::vector<VertexIF*>::const_iterator & iterator);

	VertexIF * previous(std::vector<VertexIF*>::const_iterator & iterator);

	VertexIF * peek(std::vector<VertexIF*>::const_iterator & iterator,
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

	VertexArray(VertexSetIF const & vertexArray);

	VertexArray();

	VertexArray(VertexCount numberOfVertices);

	~VertexArray();

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

#endif /* INCLUDE_STRUCTURES_VERTEXSET_VERTEXARRAY_HPP_ */
