/*
 * VertexIF.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_VERTEXIF_HPP_
#define INCLUDE_STRUCTURES_VERTEXIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../enums/Visibility.hpp"
#include "../typedefs/primitive.hpp"
#include "JSONIF.hpp"
#include "VisibleElementIF.hpp"

class EdgeIF;

class EdgeByVertexSetIF;

class VertexIF: public VisibleElement, public JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	VertexIdx vertexIdx;
	EdgeByVertexSetIF * inputEdges;
	EdgeByVertexSetIF * outputEdges;

	//************************************** PROTECTED FUNCTIONS ***************************************//
public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	VertexIF(VertexIF * vertex);

	VertexIF(VertexIdx vertexIdx, Visibility const visibility);

	VertexIF(VertexIdx vertexIdx);

	virtual ~VertexIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	/** Łączy krawędź do wierzchołka, która do niego wchodzi
	 *
	 * @param inputEdge
	 */
	void addInputEdge(EdgeIF * inputEdge);

	/** Łączy krawędź do wierzchołka wychodzącą z tego wierzchołka
	 *
	 * @param outputEdge
	 */
	void addOutputEdge(EdgeIF * outputEdge);

	EdgeIF * findInputEdge(VertexIdx const vertexId,
			bool searchOutputIfNoResult);

	EdgeIF * findInputEdge(VertexIdx const vertexId);

	EdgeIF * findInputEdge(VertexIF * vertex, bool searchOutputIfNoResult);

	EdgeIF * findInputEdge(VertexIF * vertex);

	EdgeIF * findOutputEdge(VertexIdx const vertexId,
			bool searchInputIfNoResult);

	EdgeIF * findOutputEdge(VertexIdx const vertexId);

	EdgeIF * findOutputEdge(VertexIF * vertex, bool searchInputIfNoResult);

	EdgeIF * findOutputEdge(VertexIF * vertex);

	/** Rozłącza wierzchołek z wchodzącą do niego krawędzią
	 *
	 * @param inputEdge
	 */
	void removeInputEdge(EdgeIF * const inputEdge);

	void removeInputEdge(VertexIdx const vertexIdx);

	void removeInputEdge(VertexIF * const vertex);

	void removeOutputEdge(EdgeIF * outputEdge);

	void removeOutputEdge(VertexIdx const vertexIdx);

	void removeOutputEdge(VertexIF * const vertex);

	VertexIF* getRandomSuccessor();

	void beginInputEdges();

	bool hasNextInputEdge();

	bool hasNextInputEdge(Visibility const visibility);

	bool hasAnyInputEdge();

	bool hasAnyInputEdge(Visibility const visibility);

	EdgeIF * nextInputEdge();

	VertexIF * nextInputEdgeSource();

	void beginOutputEdges();

	bool hasNextOutputEdge();

	bool hasNextOutputEdge(Visibility const visibility);

	bool hasAnyOutputEdge();

	bool hasAnyOutputEdge(Visibility const visibility);

	EdgeIF * nextOutputEdge();

	EdgeIF * currentOutputEdge();

	VertexIF * nextOutputEdgeTarget();

	VertexIF * currentOutputEdgeTarget();

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	virtual std::string toString();

	virtual std::string inputEdgesToString();

	virtual std::string outputEdgesToString();

	//*************************************** GETTERS & SETTERS ****************************************//

	VertexIdx getVertexIdx() const;

	VertexCount getNumberOfInputEdges() const;

	VertexCount getNumberOfInputEdges(Visibility const visibility) const;

	VertexCount getNumberOfOutputEdges() const;

	VertexCount getNumberOfOutputEdges(Visibility const visibility) const;

};

#endif /* INCLUDE_STRUCTURES_VERTEXIF_HPP_ */
