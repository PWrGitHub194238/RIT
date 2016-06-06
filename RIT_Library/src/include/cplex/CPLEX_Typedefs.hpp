/*
 * CPLEX_Typedefs.hpp
 *
 *  Created on: 24 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_CPLEX_CPLEX_TYPEDEFS_HPP_
#define SRC_INCLUDE_CPLEX_CPLEX_TYPEDEFS_HPP_

#include <ilconcert/iloexpression.h>
#include <map>
#include <utility>

#include "../../include/typedefs/primitive.hpp"

class EdgeIF;

/** Mapowanie zmiennej LP z właściwą jej krawędzią po danych indeksach wierzchołków, które łączy
 *
 */
typedef std::map<VertexIdx, std::pair<IloNumVar, EdgeIF*>> IloTargetVertexEdgeVarMap;
typedef std::map<VertexIdx, IloTargetVertexEdgeVarMap> IloEdgeVarMap;

typedef std::pair<const VertexIdx,
		std::map<VertexIdx, std::pair<IloNumVar, EdgeIF *>>>IloEdgeVarMapEntry;
typedef std::pair<const VertexIdx, std::pair<IloNumVar, EdgeIF *>> IloEdgeVarEntry;

typedef std::pair<const VertexIdx, IloNumVar> IloTargetVertexFlowVarEntry;
typedef std::map<VertexIdx, IloNumVar> IloTargetVertexFlowVarMap;

typedef std::pair<const VertexIdx, IloTargetVertexFlowVarMap> IloFlowVarMapEntry;
typedef std::map<VertexIdx, IloTargetVertexFlowVarMap> IloFlowVarMap;

typedef std::map<VertexIdx, IloFlowVarMap> IloCommodityFlowVarMap;

typedef std::map<VertexIdx, bool> IloTargetVertexEdgeValMap;
typedef std::map<VertexIdx, IloTargetVertexEdgeValMap> IloEdgeValMap;

#endif /* SRC_INCLUDE_CPLEX_CPLEX_TYPEDEFS_HPP_ */
