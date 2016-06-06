/*
 * VisibleIterableIF.hpp
 *
 *  Created on: 13 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_VISIBLEITERABLEIF_HPP_
#define SRC_INCLUDE_STRUCTURES_VISIBLEITERABLEIF_HPP_

#include "../enums/Visibility.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "IterableIF.hpp"

template<typename Item>
class VisibleIterable: public Iterable<Item> {
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

	virtual ~VisibleIterable() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Checks if there are still elements to iterate by default iterator
	 * that are visible or not visible in given collection
	 *
	 * It will automatically iterate to 1st element that meet condition
	 * and then it can be get by calling next() / current()
	 *
	 * hasNext(Visibility:BOTH) is equivalent to hasNext()
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin();
	 * 	while (hasNext(Visibility::VISIBLE)) {
	 *		// Do something with item from next()
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called
	 * next() function is not the last element of this set, false otherwise.
	 */
	virtual bool hasNext(Visibility const visibility) = 0;

	/** Checks if there are still elements to iterate by iterator with given id
	 * that are visible or not visible in given collection
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by begin(iteratorId).
	 *
	 * It will automatically iterate to 1st element that meet condition
	 * and then it can be get by calling next() / current()
	 *
	 * hasNext(iteratorId,Visibility:BOTH) is equivalent to hasNext(iteratorId)
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	begin(iteratorId);
	 * 	while (hasNext(iteratorId, Visibility::VISIBLE)) {
	 *		// Do something with item from next(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called
	 * next(iteratorId) function is not the last element of this set, false otherwise.
	 */
	virtual bool hasNext(IteratorId const iteratorId,
			Visibility const visibility) = 0;

	/** Checks if there are still elements to iterate by default iterator
	 * that are visible or not visible in given collection
	 *
	 * It will automatically iterate to 1st element that meet condition
	 * and then it can be get by calling previous() / current()
	 *
	 * hasPrevious(Visibility:BOTH) is equivalent to hasPrevious()
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end();
	 * 	while (hasPrevious(Visibility::VISIBLE)) {
	 *		// Do something with item from previous()
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called
	 * previous() function is not the last element of this set, false otherwise.
	 */
	virtual bool hasPrevious(Visibility const visibility) = 0;

	/** Checks if there are still elements to iterate by iterator with given id
	 * that are visible or not visible in given collection
	 *
	 *
	 * Every implementation of this interface along with default iterator
	 * has additional structure that manage additional iterators.
	 *
	 * This method assumes that iterator with given id already was created by end(iteratorId).
	 *
	 * It will automatically iterate to 1st element that meet condition
	 * and then it can be get by calling previous() / current()
	 *
	 * hasPrevious(Visibility:BOTH) is equivalent to hasPrevious()
	 *
	 * This function is usually used to simulate loop behavior:
	 *
	 * @code
	 * 	end(iteratorId);
	 * 	while (hasPrevious(iteratorId, Visibility::VISIBLE)) {
	 *		// Do something with item from previous(iteratorId)
	 *	}
	 * @endcode
	 *
	 * @return true if actual element that is returned by previously called
	 * previous(iteratorId) function is not the last element of this set, false otherwise.
	 */
	virtual bool hasPrevious(IteratorId const iteratorId,
			Visibility const visibility) = 0;

	/** Checks if there is any element to iterate. This method will use default iterator.
	 *
	 * This function checks if there is any element in iterable set by setting iterator
	 * to the beginning of the set and return value of hasNext(visibility) function
	 *
	 * @return true if actual element that is returned by previously called
	 * next() function is not the last element of this set, false otherwise.
	 */
	bool hasAny(Visibility const visibility);

	/** Checks if there is any element to iterate. This method will use iterator with given id.
	 *
	 * This function checks if there is any element in iterable set by setting iterator
	 * to the beginning of the set and return value of hasNext(iteratorId,visibility) function
	 *
	 * @return true if actual element that is returned by previously called
	 * next(iteratorId) function is not the last element of this set, false otherwise.
	 */
	bool hasAny(IteratorId const iteratorId, Visibility const visibility);

	/** Hide all visible elements. This method will use default iterator.
	 *
	 */
	void hideAll();

	/** Hide all elements and return an array that stores visibility of all elements before this operation
	 * allowing to restore their state by calling restoreVisibilityAll().
	 *
	 * @return
	 */
	VisibilityList storeVisibility();

	/** Hide all visible elements. This method will use iterator with given id.
	 *
	 */
	void hideAll(IteratorId const iteratorId);

	/** Hide all elements and return an array that stores visibility of all elements before this operation
	 * allowing to restore their state by calling restoreVisibilityAll(iteratorId).
	 *
	 */
	VisibilityList storeVisibility(IteratorId const iteratorId);

	/** Restore visibility of all elements from given list
	 *
	 */
	void restoreVisibilityAll(VisibilityList const & visibilityList);

	/** Make visible all hidden elements. This method will use default iterator.
	 *
	 */
	void showAll();

	/** Restore visibility of all elements from given list by using iterator with given id.
	 *
	 */
	void restoreVisibilityAll(VisibilityList const & visibilityList,
			IteratorId const iteratorId);

	/** Make visible all hidden elements. This method will use iterator with given id.
	 *
	 */
	void showAll(IteratorId const iteratorId);

	//*************************************** GETTERS & SETTERS ****************************************//

};
#endif /* SRC_INCLUDE_STRUCTURES_VISIBLEITERABLEIF_HPP_ */
