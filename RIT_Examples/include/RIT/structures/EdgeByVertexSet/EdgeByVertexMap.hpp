/*
 * EdgeByVertexMap.hpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSET_EDGEBYVERTEXMAP_HPP_
#define SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSET_EDGEBYVERTEXMAP_HPP_

#include <map>
#include <set>

#include "../../enums/EdgeByVertexKey.hpp"
#include "../../enums/Visibility.hpp"
#include "../../typedefs/primitive.hpp"
#include "../../typedefs/struct.hpp"
#include "../EdgeByVertexSetIF.hpp"

class EdgeIF;
class VertexIF;

class EdgeByVertexMap: public EdgeByVertexSetIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	EdgeByVertexIdxMap edgeMap;

	EdgeByVertexIdxMap::const_iterator edgeIteratorBegin;

	EdgeByVertexIdxMap::const_iterator edgeIterator;

	std::map<IteratorId, EdgeByVertexIdxMap::const_iterator> iteratorMap;

	EdgeByVertexIdxMap::const_iterator edgeIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(EdgeByVertexIdxMap::const_iterator & iterator);

	void end(EdgeByVertexIdxMap::const_iterator & iterator);

	bool hasNext(EdgeByVertexIdxMap::const_iterator & iterator);

	bool hasNext(EdgeByVertexIdxMap::const_iterator & iterator,
			Visibility const visibility);

	bool hasPrevious(EdgeByVertexIdxMap::const_iterator & iterator);

	bool hasPrevious(EdgeByVertexIdxMap::const_iterator & iterator,
			Visibility const visibility);

	EdgeByVertexIdxPair next(EdgeByVertexIdxMap::const_iterator & iterator);

	EdgeByVertexIdxPair current(EdgeByVertexIdxMap::const_iterator & iterator);

	EdgeByVertexIdxPair previous(EdgeByVertexIdxMap::const_iterator & iterator);

	EdgeByVertexIdxPair peek(EdgeByVertexIdxMap::const_iterator & iterator,
			int moveIndex) throw (LogicExceptions::EmptyIteratorException);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	void createIteratorIfNotExists(IteratorId const iteratorId);

	void addUndirectedEdge(EdgeIF * const edge);

	void addForwardEdge(EdgeIF * const edge);

	void addBackwardEdge(EdgeIF * const edge);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	EdgeByVertexMap(VertexIF const * const vertex, EdgeByVertexKey const key);

	~EdgeByVertexMap();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	EdgeIF * findEdge(VertexIdx const vertexId);

	EdgeIF * findEdge(VertexIF * const vertex);

	void removeEdge(EdgeIF * const edge);

	void removeEdge(VertexIdx const vertexIdx);

	void removeEdge(VertexIF * const vertex);

	EdgeCount size() const;

	EdgeIF* getRandomEdge();

	EdgeIF* getRandomEdge(std::set<VertexIdx> excludedList);

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

	EdgeByVertexIdxPair next();

	EdgeByVertexIdxPair next(IteratorId const iteratorId);

	EdgeByVertexIdxPair current();

	EdgeByVertexIdxPair current(IteratorId const iteratorId);

	EdgeByVertexIdxPair previous();

	EdgeByVertexIdxPair previous(IteratorId const iteratorId);

	EdgeByVertexIdxPair peek(int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	EdgeByVertexIdxPair peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	IteratorId getIterator();

	void removeIterator(IteratorId const iteratorId);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_STRUCTURES_EDGEBYVERTEXSET_EDGEBYVERTEXMAP_HPP_ */
