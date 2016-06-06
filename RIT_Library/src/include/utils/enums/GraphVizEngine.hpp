/*
 * GraphVizEngine.hpp
 *
 *  Created on: 27 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_ENUMS_GRAPHVIZENGINE_HPP_
#define SRC_INCLUDE_UTILS_ENUMS_GRAPHVIZENGINE_HPP_

/**
 *
 */
enum class GraphVizEngine {
	CIRCO,//!< for circular graph layouts.
	DOT,  //!< default engine
	FDP,  //!<  layout engine for undirected graphs.
	NEATO,//!< spring model layout
	OSAGE,//!< clustered graphs layout
	SFDP, //!< a layout engine for undirected graphs that scales to very large graphs.
	TWOPI //!< radial graph layouts.
};

#endif /* SRC_INCLUDE_UTILS_ENUMS_GRAPHVIZENGINE_HPP_ */
