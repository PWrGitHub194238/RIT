/*
 * IOUtils.hpp
 *
 *  Created on: 26 lut 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_UTILS_IOUTILS_HPP_
#define SRC_INCLUDE_UTILS_IOUTILS_HPP_

#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

#include "../typedefs/primitive.hpp"
#include "enums/GraphVizEngine.hpp"
#include "enums/InputFormat.hpp"
#include "enums/InputMode.hpp"
#include "enums/OutputFormat.hpp"
#include "GraphVizUtils.hpp"

#include "../exp/IOExceptions.hpp"

class GraphEdgeCostsIF;
class EdgeIF;
class VertexIF;
class GraphIF;

namespace IOUtils {

namespace impl {

extern const unsigned short MAX_CHARS_IN_LINE;

extern const long MAX_STREAM_SIZE;
extern const short BUFFER_SIZE;

namespace GR {

extern const char COMMENT_LINE;
extern const unsigned short COMMENT_LINE_NUMERIC;
extern const char* COMMENT_LINE_PATTERN;
extern const unsigned short COMMENT_LINE_PATTERN_ARGS;

extern const char PROBLEM_DEF_LINE;
extern const unsigned short PROBLEM_DEF_LINE_NUMERIC;
extern const std::unique_ptr<char[]> PROBLEM_DEF_LINE_PATTERN;
extern const unsigned short PROBLEM_DEF_LINE_PATTERN_ARGS;

extern const char ARC_DEF_LINE;
extern const unsigned short ARC_DEF_LINE_NUMERIC;
extern const std::unique_ptr<char[]> ARC_DEF_LINE_PATTERN;
extern const unsigned short ARC_DEF_LINE_PATTERN_ARGS;

}  // namespace GR

namespace VA {

extern const char* VERTEX_LIST_KEY;
extern const char* EDGE_LIST_KEY;
extern const char* EDGE_VERTEX_A_KEY;
extern const char* EDGE_VERTEX_B_KEY;
extern const char* EDGE_COST_KEY;

}  // namespace VA

namespace DOT {

extern const char* GRAPH_DEF;
extern const char* DIR;
extern const char* LABEL;
extern const char* WEIGHT;
extern const char* LEN;
extern const char* EDGE_UNDIR;
extern const char* ENDLN;

}  // namespace DOT

}  // namespace impl

}  // namespace IOUtils

namespace InputUtils {

GraphIF * readGraph(char const * filename, InputFormat inputFormat,
		InputMode inputMode) throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * filename, InputFormat inputFormat,
		InputMode inputMode, std::string scenarioName)
				throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * filename, InputFormat inputFormat,
		InputMode inputMode) throw (IOExceptions::FileNotFountException);

namespace impl {

namespace VA {

VertexIdx getVertexIdxFromValue(const rapidjson::Value& vertexIdx);

EdgeCost getEdgeCostFromValue(const rapidjson::Value& edgeCost);

}  // namespace VA

namespace RAM {

GraphIF * readGraph(char const * const filename, InputFormat inputFormat)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		InputFormat inputFormat, std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

namespace GR {

GraphIF * readGraph(char const * const filename)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

GraphIF * createGraph(char * const buffer)
		throw (IOExceptions::InvalidProblemRead);

EdgeCount getNumberOfEdgeCosts(char * const buffer)
		throw (IOExceptions::InvalidProblemRead);

void addEdge(EdgeIdx const edgeIdx, char * const buffer, GraphIF * const graph)
		throw (IOExceptions::InvalidArcRead);

EdgeCost getCost(char * const buffer) throw (IOExceptions::InvalidArcRead);

}  // namespace GR

namespace VA {

GraphIF * readGraph(char const * const filename)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

GraphIF * createGraph(rapidjson::Value & vertexList,
		rapidjson::SizeType const numberOfEdges)
				throw (IOExceptions::InvalidProblemRead);

void addEdge(EdgeIdx const edgeIdx, rapidjson::Value::ConstMemberIterator& edge,
		GraphIF * const graph) throw (IOExceptions::InvalidArcRead);

EdgeCost getCost(rapidjson::Value::ConstMemberIterator& edge)
		throw (IOExceptions::InvalidArcRead);

}  // namespace VA

}  // namespace RAM

namespace HDD {

GraphIF * readGraph(char const * const filename, InputFormat inputFormat)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		InputFormat inputFormat, std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

namespace GR {

GraphIF * readGraph(char const * const filename)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

GraphIF * createGraph(FILE * const dataFile)
		throw (IOExceptions::InvalidProblemRead);

EdgeCount getNumberOfEdgeCosts(FILE * const dataFile)
		throw (IOExceptions::InvalidProblemRead);

void addEdge(EdgeIdx const edgeIdx, FILE * const dataFile,
		GraphIF * const graph) throw (IOExceptions::InvalidArcRead);

EdgeCost getCost(FILE * const dataFile) throw (IOExceptions::InvalidArcRead);

}  // namespace GR

namespace VA {

GraphIF * readGraph(char const * const filename)
		throw (IOExceptions::FileNotFountException);

GraphEdgeCostsIF * readCosts(char const * const filename,
		std::string const & scenarioName)
				throw (IOExceptions::FileNotFountException);

GraphIF * createGraph(rapidjson::Value& vertexList,
		rapidjson::SizeType const numberOfEdges)
				throw (IOExceptions::InvalidProblemRead);

void addEdge(EdgeIdx const edgeIdx, rapidjson::Value::ConstMemberIterator& edge,
		GraphIF * const graph) throw (IOExceptions::InvalidArcRead);

EdgeCost getCost(rapidjson::Value::ConstMemberIterator& edge)
		throw (IOExceptions::InvalidArcRead);

}  // namespace VA

}  // namespace HDD

}  // namespace impl

}  // namespace InputUtils

namespace OutputUtils {

void exportGraph(GraphIF* const graph, char const * exportPath,
		OutputFormat const outputFormat, GraphVizEngine const layoutEngine,
		GraphVizUtils::GraphDimmention graphMaxWidth,
		GraphVizUtils::GraphDimmention graphMaxHeight)
				throw (IOExceptions::InvalidProblemWrite,
				IOExceptions::InvalidArcWrite);

void exportGraph(GraphIF* const graph, char const * exportPath,
		OutputFormat const outputFormat)
				throw (IOExceptions::InvalidProblemWrite,
				IOExceptions::InvalidArcWrite);

namespace impl {

namespace GR {

void exportGraph(GraphIF* const graph, char const * exportPath)
		throw (IOExceptions::InvalidProblemWrite, IOExceptions::InvalidArcWrite);

}  // namespace GR

namespace VA {

/** Jak na razie, tylko GraphVizEngine::NEATO jest dostępny.
 *
 * @param graph
 * @param exportPath
 * @param layoutEngine
 * @param graphMaxWidth
 * @param graphMaxHeight
 */
void exportGraph(GraphIF* const graph, char const * exportPath,
		GraphVizEngine const layoutEngine,
		GraphVizUtils::GraphDimmention graphMaxWidth,
		GraphVizUtils::GraphDimmention graphMaxHeight);

void exportGraph(GraphIF* const graph, char const * exportPath);

rapidjson::Document getVertexSetJson(
		rapidjson::Document::AllocatorType& allocator, GraphIF * const graph,
		GraphVizUtils::VerticesCoordinates verticesCoordinates);

rapidjson::Document getVertexSetJson(
		rapidjson::Document::AllocatorType& allocator, GraphIF * const graph);

rapidjson::Document getEdgeSetJson(
		rapidjson::Document::AllocatorType& allocator, GraphIF * const graph);

rapidjson::Document getVertexJson(rapidjson::Document& vertexJsonDoc,
		rapidjson::Document::AllocatorType& allocator, VertexIF * const vertex,
		GraphVizUtils::VertexCoordinates vertexCoordinates);

rapidjson::Document getVertexJson(rapidjson::Document& vertexJsonDoc,
		rapidjson::Document::AllocatorType& allocator, VertexIF * const vertex);

rapidjson::Document getEdgeJson(rapidjson::Document& vertexJsonDoc,
		rapidjson::Document::AllocatorType& allocator, EdgeIF * const edge);

}  // namespace VA

namespace DOT {

void exportGraph(GraphIF* const graph, char const * exportPath);

std::string exportGraph(GraphIF* const graph);

void exportGraphToStream(GraphIF* const graph, std::ostream& stream);

}  // namespace DOT

}  // namespace impl

}  // namespace OutputUtils

#endif /* SRC_INCLUDE_UTILS_IOUTILS_HPP_ */
