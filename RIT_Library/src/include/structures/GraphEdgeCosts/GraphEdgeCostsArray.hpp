/*
 * GraphEdgeCostsArray.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSARRAY_HPP_
#define SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSARRAY_HPP_

#include <map>
#include <string>
#include <vector>

#include "../../typedefs/primitive.hpp"
#include "../GraphEdgeCostsIF.hpp"

class EdgeIF;
class GraphIF;

class GraphEdgeCostsArray: public GraphEdgeCostsIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	std::vector<EdgeCost> edgeCosts;

	std::vector<EdgeCost>::const_iterator edgeCostsIteratorBegin;

	std::vector<EdgeCost>::const_iterator edgeCostsIterator;

	std::map<IteratorId, std::vector<EdgeCost>::const_iterator> iteratorMap;

	std::vector<EdgeCost>::const_iterator edgeCostsIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(std::vector<EdgeCost>::const_iterator & iterator);

	void end(std::vector<EdgeCost>::const_iterator & iterator);

	bool hasNext(std::vector<EdgeCost>::const_iterator & iterator);

	bool hasPrevious(std::vector<EdgeCost>::const_iterator & iterator);

	EdgeCost next(std::vector<EdgeCost>::const_iterator & iterator);

	EdgeCost current(std::vector<EdgeCost>::const_iterator & iterator);

	EdgeCost previous(std::vector<EdgeCost>::const_iterator & iterator);

	EdgeCost peek(std::vector<EdgeCost>::const_iterator & iterator,
			int moveIndex) throw (LogicExceptions::EmptyIteratorException);

	bool compare_inc(const EdgeCost& first, const EdgeCost& second);

	bool compare_dec(const EdgeCost& first, const EdgeCost& second);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	void createIteratorIfNotExists(IteratorId const iteratorId);

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	GraphEdgeCostsArray();

	GraphEdgeCostsArray(std::string scenarioName);

	GraphEdgeCostsArray(GraphEdgeCostsIF * graphEdgeCosts);

	GraphEdgeCostsArray(GraphEdgeCostsIF * graphEdgeCosts,
			std::string scenarioName);

	GraphEdgeCostsArray(GraphIF* const graph);

	GraphEdgeCostsArray(GraphIF* const graph, std::string scenarioName);

	GraphEdgeCostsArray(EdgeSetIF* const edgeSet);

	GraphEdgeCostsArray(EdgeSetIF* const edgeSet, std::string scenarioName);

	GraphEdgeCostsArray(EdgeCount numberOfEdges);

	GraphEdgeCostsArray(EdgeCount numberOfEdges, std::string scenarioName);

	GraphEdgeCostsArray(EdgeCount numberOfEdges, bool fillWithZeros);

	GraphEdgeCostsArray(EdgeCount numberOfEdges, std::string scenarioName,
			bool fillWithZeros);

	virtual ~GraphEdgeCostsArray();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual EdgeCost& operator[](EdgeIdx const edgeIdx);

	void push_back(EdgeCost edgecost);

	EdgeCost at(EdgeIdx edgeIdx);

	EdgeCount size() const;

	void sortInc();

	void sortDec();

	void begin();

	void begin(IteratorId const iteratorId);

	void end();

	void end(IteratorId const iteratorId);

	bool hasNext();

	bool hasNext(IteratorId const iteratorId);

	bool hasPrevious();

	bool hasPrevious(IteratorId const iteratorId);

	EdgeCost next();

	EdgeCost next(IteratorId const iteratorId);

	EdgeCost current();

	EdgeCost current(IteratorId const iteratorId);

	EdgeCost previous();

	EdgeCost previous(IteratorId const iteratorId);

	EdgeCost peek(int moveIndex) throw (LogicExceptions::EmptyIteratorException);

	EdgeCost peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

	IteratorId getIterator();

	void removeIterator(IteratorId const iteratorId);

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSARRAY_HPP_ */
