/*
 * primitives.hpp
 *
 *  Created on: 6 sty 2016
 *      Author: tomasz
 */

#ifndef INCLUDE_TYPEDEFS_PRIMITIVE_HPP_
#define INCLUDE_TYPEDEFS_PRIMITIVE_HPP_

typedef unsigned int VertexCount;

typedef double VertexKey;

typedef unsigned int EdgeCount;

typedef unsigned int VertexIdx;
/**
 * Dla ułatwienia obsługi pętli, gdy schodzimy do zera i nie możemy na uintach zatrzymywać się na ui >= 0
 */
typedef long int SEdgeIdx;
typedef unsigned int EdgeIdx;
typedef double EdgeCost;

typedef double Density;

typedef double LambdaValue;
typedef unsigned int LambdaIdx;
typedef unsigned int LambdaCount;

typedef EdgeCount IncrementalParam;

typedef unsigned int IteratorId;

typedef unsigned int TabuIterationCount;

typedef unsigned int LPUIntValue;

typedef double LPFloatValue;

typedef unsigned int ThreadId;
typedef unsigned int ScenarioIdx;
typedef unsigned int ScenarioCount;

#endif /* INCLUDE_TYPEDEFS_PRIMITIVE_HPP_ */
