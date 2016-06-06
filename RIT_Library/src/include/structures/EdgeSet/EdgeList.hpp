/*
 * EdgeList.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_EDGESET_EDGELIST_HPP_
#define INCLUDE_STRUCTURES_EDGESET_EDGELIST_HPP_

#include <list>
#include <map>

#include "../../enums/Connectivity.hpp"
#include "../../enums/Visibility.hpp"
#include "../../typedefs/primitive.hpp"
#include "../EdgeIF.hpp"
#include "../EdgeSetIF.hpp"

class EdgeList: public EdgeSetIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	std::list<EdgeIF*> edges;

	std::list<EdgeIF*>::const_iterator edgeIteratorBegin;

	std::list<EdgeIF*>::const_iterator edgeIterator;

	std::map<IteratorId, std::list<EdgeIF*>::const_iterator> iteratorMap;

	std::list<EdgeIF*>::const_iterator edgeIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(std::list<EdgeIF*>::const_iterator & iterator);

	void end(std::list<EdgeIF*>::const_iterator & iterator);

	bool hasNext(std::list<EdgeIF*>::const_iterator & iterator);

	bool hasNext(std::list<EdgeIF*>::const_iterator & iterator,
			Visibility const visibility);

	bool hasNext(std::list<EdgeIF*>::const_iterator & iterator,
			Connectivity const connectivity);

	bool hasNext(std::list<EdgeIF*>::const_iterator & iterator,
			Connectivity const connectivity, Visibility const visibility);

	bool hasPrevious(std::list<EdgeIF*>::const_iterator & iterator);

	bool hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
			Visibility const visibility);

	bool hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
			Connectivity const connectivity);

	bool hasPrevious(std::list<EdgeIF*>::const_iterator & iterator,
			Connectivity const connectivity, Visibility const visibility);

	EdgeIF * next(std::list<EdgeIF*>::const_iterator & iterator);

	EdgeIF * current(std::list<EdgeIF*>::const_iterator & iterator);

	EdgeIF * previous(std::list<EdgeIF*>::const_iterator & iterator);

	EdgeIF * peek(std::list<EdgeIF*>::const_iterator & iterator, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	void createIteratorIfNotExists(IteratorId const iteratorId);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	EdgeList();

	EdgeList(EdgeSetIF * edgeList);

	EdgeList(EdgeSetIF * edgeList, bool deepCopy);

	EdgeList(EdgeCount numberOfEdges);

	~EdgeList();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	void push_back(EdgeIF * const & edge);

	void pop_back();

	void remove(EdgeIF * const & edge);

	EdgeIF * getElementAt(EdgeIdx const edgeIdx);

	EdgeCount size() const;

	void begin();

	void begin(IteratorId const iteratorId);

	void end();

	void end(IteratorId const iteratorId);

	bool hasNext();

	bool hasNext(IteratorId const iteratorId);

	bool hasNext(Visibility const visibility);

	bool hasNext(IteratorId const iteratorId, Visibility const visibility);

	bool hasNext(Connectivity const connectivity);

	bool hasNext(IteratorId const iteratorId, Connectivity const connectivity);

	bool hasNext(Connectivity const connectivity, Visibility const visibility);

	bool hasNext(IteratorId const iteratorId, Connectivity const connectivity,
			Visibility const visibility);

	bool hasPrevious();

	bool hasPrevious(IteratorId const iteratorId);

	bool hasPrevious(Visibility const visibility);

	bool hasPrevious(IteratorId const iteratorId, Visibility const visibility);

	bool hasPrevious(Connectivity const connectivity);

	bool hasPrevious(IteratorId const iteratorId,
			Connectivity const connectivity);

	bool hasPrevious(Connectivity const connectivity,
			Visibility const visibility);

	bool hasPrevious(IteratorId const iteratorId,
			Connectivity const connectivity, Visibility const visibility);

	EdgeIF * next();

	EdgeIF * next(IteratorId const iteratorId);

	EdgeIF * current();

	EdgeIF * current(IteratorId const iteratorId);

	EdgeIF * previous();

	EdgeIF * previous(IteratorId const iteratorId);

	EdgeIF * peek(int moveIndex) throw (LogicExceptions::EmptyIteratorException);

	EdgeIF * peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	IteratorId getIterator();

	void removeIterator(IteratorId const iteratorId);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* INCLUDE_STRUCTURES_EDGESET_EDGELIST_HPP_ */
