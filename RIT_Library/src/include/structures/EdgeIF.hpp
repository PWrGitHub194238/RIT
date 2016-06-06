/*
 * EdgeIF.hpp
 *
 *  Created on: 2 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_STRUCTURES_EDGEIF_HPP_
#define INCLUDE_STRUCTURES_EDGEIF_HPP_

#include <rapidjson/document.h>
#include <string>

#include "../enums/Connectivity.hpp"
#include "../enums/EdgeConnectionType.hpp"
#include "../enums/Visibility.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "JSONIF.hpp"
#include "VisibleElementIF.hpp"

class VertexIF;

class EdgeIF: public VisibleElement, public JSONIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	void connectUndirected();

	void connectForward();

	void connectBackward();

	void disconnectUndirected();

	void disconnectForward();

	void disconnectBackward();

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	EdgeIdx edgeIdx;

	VertexPair edgeConnection;

	EdgeCost edgeCost;

	EdgeConnectionType connectionType;

	//************************************** PROTECTED FUNCTIONS ***************************************//

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	EdgeIF(EdgeIF * edge);

	EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
			EdgeCost const edgeCost, EdgeConnectionType isConnect,
			Visibility const visibility);

	EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
			EdgeCost const edgeCost, EdgeConnectionType isConnect);

	EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
			EdgeCost const edgeCost, Visibility visibility);

	EdgeIF(EdgeIdx const edgeIdx, VertexPair const & edgeConnections,
			EdgeCost const edgeCost);

	virtual ~EdgeIF();

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	void connect(EdgeConnectionType connectionType);

	void disconnect();

	bool isInState(Connectivity connectivity) const;

	virtual void fillJSON(rapidjson::Document& jsonDoc,
			rapidjson::Document::AllocatorType& allocator,
			unsigned short depth);

	virtual std::string toString() const;

	//*************************************** GETTERS & SETTERS ****************************************//

	EdgeIdx getEdgeIdx() const;

	EdgeCost getEdgeCost() const;

	void setEdgeCost(EdgeCost edgeCost);

	/** Jeśli krawędź nie jest BACKWARD wtedy zwróć jako źródło krawędzi (u,v) normalnie u.
	 * Jeśli krawędź jest BACKWARD wtedy z pary (u,v), v jest źródłem
	 *
	 * @return
	 */
	VertexIF * getSourceVertex() const;

	/** Jeśli krawędź nie jest BACKWARD wtedy zwróć jako target krawędzi (u,v) normalnie v.
	 * Jeśli krawędź jest BACKWARD wtedy z pary (u,v), v jest targetem
	 *
	 * @return
	 */
	VertexIF * getTargetVertex() const;

	/** W przypadku krawędzi nieskierowanych nie ma możliwości jednoznaczego wskazania, który wierzchołek jest teraz źródłem, a który celem.
	 * Funkcja zwraca drugi z pary wierzchołków krawędzi, inny niż podany vertex.
	 * Jeśli krawędź ma oba końce w tym samym wierzchołku to zwraca jeden z nich.
	 *
	 * @param vertex
	 * @return
	 */
	VertexIF * getOtherVertex(VertexIF* vertex) const;

	/** W przypadku krawędzi nieskierowanych nie ma możliwości jednoznaczego wskazania, który wierzchołek jest teraz źródłem, a który celem.
	 * Funkcja zwraca drugi z pary wierzchołków krawędzi 'v', którego idx jest różne od podanego.
	 * Jeśli krawędź ma oba końce w tym samym wierzchołku to zwraca jeden z nich.
	 *
	 *
	 * @param vertexIdx
	 * @return
	 */
	VertexIF* getOtherVertex(VertexIdx vertexIdx) const;

	EdgeConnectionType getConnectionType() const;

};

#endif /* INCLUDE_STRUCTURES_EDGEIF_HPP_ */
