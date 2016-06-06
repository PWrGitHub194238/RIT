/*
 * GraphEdgeCostsList.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSLIST_HPP_
#define SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSLIST_HPP_

#include <list>
#include <map>
#include <string>

#include "../../typedefs/primitive.hpp"
#include "../GraphEdgeCostsIF.hpp"

class EdgeIF;
class GraphIF;

class GraphEdgeCostsList: public GraphEdgeCostsIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	std::list<EdgeCost> edgeCosts;

	std::list<EdgeCost>::const_iterator edgeCostsIteratorBegin;

	std::list<EdgeCost>::const_iterator edgeCostsIterator;

	std::map<IteratorId, std::list<EdgeCost>::const_iterator> iteratorMap;

	std::list<EdgeCost>::const_iterator edgeCostsIteratorEnd;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void begin(std::list<EdgeCost>::const_iterator & iterator);

	void end(std::list<EdgeCost>::const_iterator & iterator);

	bool hasNext(std::list<EdgeCost>::const_iterator & iterator);

	bool hasPrevious(std::list<EdgeCost>::const_iterator & iterator);

	EdgeCost next(std::list<EdgeCost>::const_iterator & iterator);

	EdgeCost current(std::list<EdgeCost>::const_iterator & iterator);

	EdgeCost previous(std::list<EdgeCost>::const_iterator & iterator);

	EdgeCost peek(std::list<EdgeCost>::const_iterator & iterator, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException);

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

	GraphEdgeCostsList();

	GraphEdgeCostsList(std::string scenarioName);

	GraphEdgeCostsList(GraphEdgeCostsIF * graphEdgeCosts);

	GraphEdgeCostsList(GraphEdgeCostsIF * graphEdgeCosts,
			std::string scenarioName);

	GraphEdgeCostsList(GraphIF* const graph);

	GraphEdgeCostsList(GraphIF* const graph, std::string scenarioName);

	GraphEdgeCostsList(EdgeSetIF* const edgeSet);

	GraphEdgeCostsList(EdgeSetIF* const edgeSet, std::string scenarioName);

	GraphEdgeCostsList(EdgeCount numberOfEdges);

	GraphEdgeCostsList(EdgeCount numberOfEdges, std::string scenarioName);

	GraphEdgeCostsList(EdgeCount numberOfEdges, bool fillWithZeros);

	GraphEdgeCostsList(EdgeCount numberOfEdges, std::string scenarioName,
			bool fillWithZeros);

	virtual ~GraphEdgeCostsList();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	EdgeCost& operator[](EdgeIdx const edgeIdx);

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

#endif /* SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTS_GRAPHEDGECOSTSLIST_HPP_ */
