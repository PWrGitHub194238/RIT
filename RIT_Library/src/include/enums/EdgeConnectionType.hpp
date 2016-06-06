/*
 * MSTSolverMode.hpp
 *
 *  Created on: 13 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_ENUMS_EDGECONNECTIONTYPE_HPP_
#define SRC_INCLUDE_ENUMS_EDGECONNECTIONTYPE_HPP_

/**
 *
 */
enum class EdgeConnectionType {
	UNDIRECTED, //!< łączy krawędź w obie strony z wierzchołkami
	FORWARD,   //!< łączy krawędź (u,v) jako skierowaną do v
	BACKWARD,  //!< łączy krawędź (u,v) jako skierowaną do u
	UNCONNECTED//!< dodaj krawędź, ale jej nie łącz
};

#endif /* SRC_INCLUDE_ENUMS_EDGECONNECTIONTYPE_HPP_ */
