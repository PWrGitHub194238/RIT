/*
 * OutputFormat.hpp
 *
 *  Created on: 26 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_ENUMS_OUTPUTFORMAT_HPP_
#define SRC_INCLUDE_UTILS_ENUMS_OUTPUTFORMAT_HPP_

/**
 *
 */
enum class OutputFormat {
	GR,/*!<
	 http://www.dis.uniroma1.it/challenge9/format.shtml#graph
	 http://www.dis.uniroma1.it/challenge9/samples/sample.gr
	 */
	VA, /*!<
	 http://visualgo.net/mst.html
	 */
	DOT /*!<
	 http://www.graphviz.org/pdf/dotguide.pdf
	 */
};

#endif /* SRC_INCLUDE_UTILS_ENUMS_OUTPUTFORMAT_HPP_ */
