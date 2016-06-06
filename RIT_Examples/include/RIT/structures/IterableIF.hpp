/*
 * Iterable.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_ITERABLEIF_HPP_
#define SRC_INCLUDE_STRUCTURES_ITERABLEIF_HPP_

#include "../typedefs/primitive.hpp"

namespace LogicExceptions {
class EmptyIteratorException;
} /* namespace LogicExceptions */

template<typename Item>
class Iterable {
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

	virtual ~Iterable() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Reset default iterator to the 1st element of iterable collection.
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin();
	 * 	while (hasNext()) {
	 *		// Do something with item from next()
	 *	}
	 * @endcode
	 *
	 */
	virtual void begin() = 0;

	/** Reset iterator with given id to the 1st element of iterable collection.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * If there is no iterator with given id, it will be created.
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin(iteratorId);
	 * 	while (hasNext(iteratorId)) {
	 *		// Do something with item from next(iteratorId)
	 *	}
	 * @endcode
	 *
	 */
	virtual void begin(IteratorId const iteratorId) = 0;

	/** Reset default iterator to the one past last element of iterable collection.
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end();
	 * 	while (hasPrevious()) {
	 *		// Do something with item from previous()
	 *	}
	 * @endcode
	 *
	 */
	virtual void end() = 0;

	/** Reset iterator with given id to the one past last element of iterable collection.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * If there is no iterator with given id, it will be created.
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end(iteratorId);
	 * 	while (hasPrevious(iteratorId)) {
	 *		// Do something with item from previous(iteratorId)
	 *	}
	 * @endcode
	 *
	 */
	virtual void end(IteratorId const iteratorId) = 0;

	/** Checks if there are still elements to iterate with default iterator
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin();
	 * 	while (hasNext()) {
	 *		// Do something with item from next()
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called next() function
	 * is not the last element of this set, false otherwise.
	 */
	virtual bool hasNext() = 0;

	/** Checks if there are still elements to iterate with iterator with given id.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by begin(iteratorId).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin(iteratorId);
	 * 	while (hasNext(iteratorId)) {
	 *		// Do something with item from next(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called next(iteratorId) function
	 * is not the last element of this set, false otherwise.
	 */
	virtual bool hasNext(IteratorId const iteratorId) = 0;

	/** Checks if there are still elements to iterate with default iterator.
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end();
	 * 	while (hasPrevious()) {
	 *		// Do something with item from previous()
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called previous() function
	 * is not the last element of this set, false otherwise.
	 */
	virtual bool hasPrevious() = 0;

	/** Checks if there are still elements to iterate with iterator with given id.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by end(iteratorId).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end(iteratorId);
	 * 	while (hasPrevious(iteratorId)) {
	 *		// Do something with item from previous(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called previous(iteratorId) function
	 * is not the last element of this set, false otherwise.
	 */
	virtual bool hasPrevious(IteratorId const iteratorId) = 0;

	/** Checks if there is any element to iterate. This method will use default iterator.
	 *
	 * This function checks if there is any element in iterable set by setting iterator
	 * to the beginning of the set and return value of hasNext() function
	 *
	 * @return true if actual element that is returned by previously called next() function
	 * is not the last element of this set, false otherwise.
	 */
	bool hasAny();

	/** Checks if there is any element to iterate. This method will use iterator with given id.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by begin(iteratorId).
	 *
	 * This function checks if there is any element in iterable set by setting iterator
	 * to the beginning of the set and return value of hasNext(iteratorId) function
	 *
	 * @return true if actual element that is returned by previously called next(iteratorId) function
	 * is not the last element of this set, false otherwise.
	 */
	bool hasAny(IteratorId const iteratorId);

	/** Returns next element of this set (it returns current element
	 * pointed by default iterator and then increase it).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin();
	 * 	while (hasNext()) {
	 *		// Do something with item from next()
	 *	}
	 * @endcode
	 *
	 * @return next element of set.
	 */
	virtual Item next() = 0;

	/** Returns next element of this set (it returns current element
	 * pointed by iterator with given id and then increase it).
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created
	 * by begin(iteratorId) / end(iteratorId).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin(iteratorId);
	 * 	while (hasNext(iteratorId)) {
	 *		// Do something with item from next(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return next element of set.
	 */
	virtual Item next(IteratorId const iteratorId) = 0;

	/** Returns element pointed by default iterator without increasing nor decreasing it.
	 *
	 * @return
	 */
	virtual Item current() = 0;

	/** Returns element pointed by iterator with given id without increasing nor decreasing it.
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by begin(iteratorId).
	 * Calling current(iteratorId) right after end(iteratorId) results in undefined behavior.
	 *
	 * @return
	 */
	virtual Item current(IteratorId const iteratorId) = 0;

	/** Returns previous element of this set (it returns current element pointed
	 * by default iterator and then decrease it).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end();
	 * 	while (hasPrevious()) {
	 *		// Do something with item from previous()
	 *	}
	 * @endcode
	 *
	 * @return next element of set.
	 */
	virtual Item previous() = 0;

	/** Returns previous element of this set (it returns current element pointed
	 * by iterator with given id and then decrease it).
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created
	 * by end(iteratorId) / begin(iteratorId).
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end(iteratorId);
	 * 	while (hasPrevious(iteratorId)) {
	 *		// Do something with item from previous(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return next element of set.
	 */
	virtual Item previous(IteratorId const iteratorId) = 0;

	/** Return given element without increasing/decreasing default iterator.
	 *
	 * @return element pointed by default iterator, temporary moved backward / forward by moveIndex
	 */
	virtual Item peek(int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	/** Return given element without increasing/decreasing iterator with given id.
	 *
	 * @return element pointed by iterator with given id, temporary moved backward / forward by moveIndex
	 */
	virtual Item peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	/** Returns id for iterator that is not used.
	 *
	 * Equivalent to this code:
	 *
	 * @code
	 * 	begin(iteratorId);
	 * 	while (hasNext(iteratorId)) {
	 *		// Do something with item from next(iteratorId)
	 *	}
	 * @endcode
	 *
	 * is code below:
	 *
	 * @code
	 * 	IteratorId id = getIterator();
	 * 	begin(id);
	 * 	while (hasNext(id)) {
	 *		// Do something with item from next(id)
	 *	}
	 * @endcode
	 *
	 * @return
	 */
	virtual IteratorId getIterator() = 0;

	/** Remove iterator with give id.
	 *
	 * Every time begin(id)/end(id) function is called, class that implements IterableIF
	 * will create new iterator with id 'id' if there is no such an iterator.
	 *
	 * @param iteratorId
	 * @return
	 */
	virtual void removeIterator(IteratorId const iteratorId) = 0;

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_STRUCTURES_ITERABLEIF_HPP_ */
