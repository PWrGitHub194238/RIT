/*
 * GraphIF.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_GRAPHIF_HPP_
#define INCLUDE_STRUCTURES_GRAPHIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../enums/Connectivity.hpp"
#include "../enums/EdgeConnectionType.hpp"
#include "../enums/GraphConstructMode.hpp"
#include "../enums/Visibility.hpp"
#include "../typedefs/struct.hpp"
#include "../utils/MemoryUtils.hpp"
#include "JSONIF.hpp"

#include "../exp/LogicExceptions.hpp"

class EdgeIF;
class VertexIF;

class EdgeSetIF;
class VertexSetIF;

#include "../typedefs/primitive.hpp"

class GraphIF: public JSONIF {

	friend void MemoryUtils::removeGraph(GraphIF* const & graph,
			bool withVertices, bool withEdges);

private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	VertexSetIF * vertexSet;
	EdgeSetIF * edgeSet;

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	GraphIF(GraphIF * graph);

	/** Jeśli AUTO_CONSTRUCT_VERTEX to tworzy vertexCount węzłów od 0 do vertexCount-1 i rezerwuje miejsce na edgeCount krawędzi
	 * Jeśli RESERVE_SPACE_ONLY to tylko rezerwuje miejsce
	 *
	 * @param vertexCount
	 * @param edgeCount
	 * @param constructMode
	 */
	GraphIF(VertexCount const vertexCount, EdgeCount const edgeCount,
			GraphConstructMode constructMode);

	/** Domyślnie tworzy vertexCount węzłów od 0 do vertexCount-1 i rezerwuje miejsce na edgeCount krawędzi
	 *
	 * @param vertexCount
	 * @param edgeCount
	 */
	GraphIF(VertexCount const vertexCount, EdgeCount const edgeCount);

	/** Tworzy graf z podanych komponentów
	 *
	 * @param vertexSet
	 * @param edgeSet
	 */
	GraphIF(VertexSetIF * const & vertexSet, EdgeSetIF * const & edgeSet);

	virtual ~GraphIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	void addVertex(VertexIF* const vertex);

	void addEdge(EdgeIF* const edge);

	/** Tworzy nową krawędź i dodaje ją do grafu
	 *
	 * @param edgeIdx
	 * @param vertexIdxU
	 * @param vertexIdxV
	 * @param edgeCost
	 * @param connectionType typ krawędzi (skierowana, nieskierowana)
	 * @param visibility czy krawędź widoczna
	 */
	void addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
			VertexIdx const vertexIdxV, EdgeCost const edgeCost,
			EdgeConnectionType connectionType, Visibility visibility);

	void addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
			VertexIdx const vertexIdxV, EdgeCost const edgeCost,
			EdgeConnectionType connectionType);

	/** Czy dodajemy do grafu skierowanego czy nie, dlatego pure virtual
	 *
	 * @param vertexIdxU
	 * @param vertexIdxV
	 * @param edgeCost
	 */
	virtual void addEdge(EdgeIdx const edgeIdx, VertexIdx const vertexIdxU,
			VertexIdx const vertexIdxV, EdgeCost const edgeCost) = 0;

	VertexIF * getVertexByIdx(VertexIdx const vertexIdx);

	EdgeIF * findEdge(VertexIF * const sourceVertex,
			VertexIF * const targetVertex)
					throw (LogicExceptions::EdgeNotFoundException);

	EdgeIF * findEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx)
					throw (LogicExceptions::EdgeNotFoundException);

	bool hasEdge(VertexIF * const sourceVertex, VertexIF * const targetVertex);

	bool hasEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx);

	EdgeIF * findInputEdge(VertexIF * const sourceVertex,
			VertexIF * const targetVertex)
					throw (LogicExceptions::VertexNotFoundException,
					LogicExceptions::EdgeNotFoundException);

	EdgeIF * findInputEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx)
					throw (LogicExceptions::VertexNotFoundException,
					LogicExceptions::EdgeNotFoundException);

	bool hasInputEdge(VertexIF * const sourceVertex,
			VertexIF * const targetVertex);

	bool hasInputEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx);

	EdgeIF * findOutputEdge(VertexIF * const sourceVertex,
			VertexIF * const targetVertex)
					throw (LogicExceptions::VertexNotFoundException,
					LogicExceptions::EdgeNotFoundException);

	EdgeIF * findOutputEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx)
					throw (LogicExceptions::VertexNotFoundException,
					LogicExceptions::EdgeNotFoundException);

	bool hasOutputEdge(VertexIF * const sourceVertex,
			VertexIF * const targetVertex);

	bool hasOutputEdge(VertexIdx const sourceVertexIdx,
			VertexIdx const targetVertexIdx);

	EdgeCost getTotalEdgeCost();

	void beginVertex();

	void beginVertex(IteratorId const iteratorId);

	bool hasNextVertex();

	bool hasNextVertex(IteratorId const iteratorId);

	bool hasNextVertex(Visibility const visibility);

	bool hasNextVertex(IteratorId const iteratorId,
			Visibility const visibility);

	bool hasAnyVertex();

	bool hasAnyVertex(IteratorId const iteratorId);

	bool hasAnyVertex(Visibility const visibility);

	bool hasAnyVertex(IteratorId const iteratorId, Visibility const visibility);

	VertexIF * nextVertex();

	VertexIF * nextVertex(IteratorId const iteratorId);

	VertexIF * nextVertex(Visibility const visibility);

	VertexIF * nextVertex(IteratorId const iteratorId,
			Visibility const visibility);

	VertexIF * currentVertex();

	VertexIF * peekPreviousVertex()
			throw (LogicExceptions::EmptyIteratorException);

	IteratorId getVertexIteratorId();

	void removeVertexIterator(IteratorId const iteratorId);

	void beginEdge();

	void beginEdge(IteratorId const iteratorId);

	bool hasNextEdge();

	bool hasNextEdge(IteratorId const iteratorId);

	bool hasNextEdge(Visibility const visibility);

	bool hasNextEdge(IteratorId const iteratorId, Visibility const visibility);

	bool hasNextEdge(Connectivity const connectivity);

	bool hasNextEdge(IteratorId const iteratorId,
			Connectivity const connectivity);

	bool hasNextEdge(Connectivity const connectivity,
			Visibility const visibility);

	bool hasNextEdge(IteratorId const iteratorId,
			Connectivity const connectivity, Visibility const visibility);

	bool hasAnyEdge();

	bool hasAnyEdge(IteratorId const iteratorId);

	bool hasAnyEdge(Visibility const visibility);

	bool hasAnyEdge(IteratorId const iteratorId, Visibility const visibility);

	bool hasAnyEdge(Connectivity const connectivity);

	bool hasAnyEdge(IteratorId const iteratorId,
			Connectivity const connectivity);

	bool hasAnyEdge(Connectivity const connectivity,
			Visibility const visibility);

	bool hasAnyEdge(IteratorId const iteratorId,
			Connectivity const connectivity, Visibility const visibility);

	EdgeIF * nextEdge();

	EdgeIF * nextEdge(IteratorId const iteratorId);

	EdgeIF * currentEdge();

	IteratorId getEdgeIteratorId();

	void removeEdgeIterator(IteratorId const iteratorId);

	void showAllEdges();

	void showAllEdges(IteratorId const iteratorId);

	void hideAllEdges();

	void hideAllEdges(IteratorId const iteratorId);

	void disconnectAllEdges();

	void disconnectAllEdges(IteratorId const iteratorId);

	ConnectivityList storeEdgeConnectivity();

	ConnectivityList storeEdgeConnectivity(IteratorId const iteratorId);

	VisibilityList storeEdgeVisibility();

	VisibilityList storeEdgeVisibility(IteratorId const iteratorId);

	void restoreConnectivityAllEdges(ConnectivityList const & connectivityList);

	void restoreConnectivityAllEdges(ConnectivityList const & connectivityList,
			IteratorId const iteratorId);

	void restoreVisibilityAllEdges(VisibilityList const & visibilityList);

	void restoreVisibilityAllEdges(VisibilityList const & visibilityList,
			IteratorId const iteratorId);

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	virtual std::string toString();

	virtual std::string edgeSetToString();

	virtual std::string edgeSetToString(Visibility edgeVisibility);

//*************************************** GETTERS & SETTERS ****************************************//

	EdgeCount getNumberOfEdges() const;

	EdgeCount getNumberOfEdges(Connectivity const connectivity) const;

	EdgeCount getNumberOfEdges(Visibility const visibility) const;

	VertexCount getNumberOfVertices() const;

	VertexCount getNumberOfVertices(Visibility const visibility) const;
};

#endif /* INCLUDE_STRUCTURES_GRAPHIF_HPP_ */
