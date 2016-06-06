/*
 * GraphEdgeCostsIF.hpp
 *
 *  Created on: 16 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSIF_HPP_
#define SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../bundle/Bundle.hpp"
#include "../typedefs/primitive.hpp"
#include "../utils/BundleUtils.hpp"
#include "IterableIF.hpp"
#include "JSONIF.hpp"

class VertexIF;

class EdgeSetIF;

class EdgeIF;
class GraphIF;

struct compare_EdgeCost_inc {
	bool operator()(const EdgeCost& first, const EdgeCost& second) {
		return first < second;
	}
};

struct compare_EdgeCost_dec {
	bool operator()(const EdgeCost& first, const EdgeCost& second) {
		return first > second;
	}
};

class GraphEdgeCostsIF: public Iterable<EdgeCost>, public JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	EdgeCount numberOfEdges;
	std::string name = BundleUtils::getString(
			AppBundleKey::DEFAULT_SCENARIO_NAME);

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	GraphEdgeCostsIF();

	GraphEdgeCostsIF(std::string scenarioName);

	GraphEdgeCostsIF(GraphIF* const graph);

	GraphEdgeCostsIF(GraphIF* const graph, std::string scenarioName);

	GraphEdgeCostsIF(EdgeSetIF* const edgeSet);

	GraphEdgeCostsIF(EdgeSetIF* const edgeSet, std::string scenarioName);

	GraphEdgeCostsIF(EdgeCount numberOfEdges);

	GraphEdgeCostsIF(EdgeCount numberOfEdges, std::string scenarioName);

	virtual ~GraphEdgeCostsIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	virtual EdgeCost& operator[](EdgeIdx const edgeIdx) = 0;

	virtual void push_back(EdgeCost edgeCost) = 0;

	virtual EdgeCost at(EdgeIdx edgeIdx) = 0;

	virtual EdgeCount size() const = 0;

	virtual void sortInc() = 0;

	virtual void sortDec() = 0;

	virtual void begin() = 0;

	virtual void begin(IteratorId const iteratorId) = 0;

	virtual void end() = 0;

	virtual void end(IteratorId const iteratorId) = 0;

	virtual bool hasNext() = 0;

	virtual bool hasNext(IteratorId const iteratorId) = 0;

	virtual bool hasPrevious() = 0;

	virtual bool hasPrevious(IteratorId const iteratorId) = 0;

	virtual EdgeCost next() = 0;

	virtual EdgeCost next(IteratorId const iteratorId) = 0;

	virtual EdgeCost current() = 0;

	virtual EdgeCost current(IteratorId const iteratorId) = 0;

	virtual EdgeCost previous() = 0;

	virtual EdgeCost previous(IteratorId const iteratorId) = 0;

	virtual EdgeCost peek(int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	virtual EdgeCost peek(IteratorId const iteratorId, int moveIndex)
			throw (LogicExceptions::EmptyIteratorException) = 0;

	virtual IteratorId getIterator() = 0;

	virtual void removeIterator(IteratorId const iteratorId) = 0;

	EdgeCost getTotalEdgeCost();

	void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	std::string toString();

	//*************************************** GETTERS & SETTERS ****************************************//

	std::string getName() const;

};

#endif /* SRC_INCLUDE_STRUCTURES_GRAPHEDGECOSTSIF_HPP_ */
