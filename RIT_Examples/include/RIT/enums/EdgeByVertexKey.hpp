/*
 * EdgeByVertexKey.hpp
 *
 *  Created on: 9 sty 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_ENUMS_EDGEBYVERTEXKEY_HPP_
#define SRC_INCLUDE_UTILS_ENUMS_EDGEBYVERTEXKEY_HPP_

/**
 *
 */
enum class EdgeByVertexKey {
	INCOMING_EDGES, //!< W EdgeByVertexSetIF klucze będą wtedy idx wierzchołków źródeł: \f$ \left\{ u \; : \; \left( u, v \right) \right\} \f$
	OUTGOING_EDGES //!< OUTGOING_EDGES

};

#endif /* SRC_INCLUDE_UTILS_ENUMS_EDGEBYVERTEXKEY_HPP_ */
