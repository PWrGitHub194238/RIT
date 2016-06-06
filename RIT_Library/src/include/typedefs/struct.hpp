/*
 * struct.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_TYPEDEFS_STRUCT_HPP_
#define INCLUDE_TYPEDEFS_STRUCT_HPP_

#include <list>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "../enums/EdgeConnectionType.hpp"
#include "primitive.hpp"

class VertexIF;
class EdgeIF;
class EdgeSetIF;
class GraphEdgeCostsIF;

typedef std::pair<VertexIF*, VertexIF*> VertexPair;

typedef std::pair<VertexIdx, EdgeIF*> EdgeByVertexIdxPair;

typedef std::map<VertexIdx, EdgeIF*> EdgeByVertexIdxMap;

typedef std::list<bool> VisibilityList;

typedef std::list<EdgeConnectionType> ConnectivityList;

typedef std::vector<LambdaValue> LambdaParamArray;

typedef std::map<EdgeIdx, LambdaValue> LambdaParamMap;

typedef std::map<EdgeIdx, LambdaParamMap> LambdaParamPairMap;

typedef std::vector<EdgeIdx> EdgeIdxArray;

typedef std::tuple<EdgeIdx, EdgeIdx, EdgeSetIF*> SpanningTreeNeighbor;

typedef std::set<SpanningTreeNeighbor> SpanningTreeNeighborhood;

typedef std::set<GraphEdgeCostsIF*> GraphEdgeCostsSet;

typedef std::map<EdgeIdx, TabuIterationCount> TabuSubMap;

typedef std::tuple<SpanningTreeNeighbor, EdgeCost> NeighborSolution;

typedef std::tuple<TabuSubMap, TabuSubMap> TabuMap;

typedef std::tuple<EdgeSetIF*, GraphEdgeCostsIF*, EdgeCost> AIMSTSolution;

#endif /* INCLUDE_TYPEDEFS_STRUCT_HPP_ */
