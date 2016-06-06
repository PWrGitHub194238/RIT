/*
 * solveProgrammaticallyCreated.cpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/mstsolver/PrimeHeap.hpp>
#include <RIT/structures/EdgeInclude.hpp>
#include <RIT/structures/EdgeSetInclude.hpp>
#include <RIT/structures/GraphInclude.hpp>
#include <RIT/structures/VertexInclude.hpp>
#include <RIT/structures/VertexSetInclude.hpp>
#include <RIT/typedefs/struct.hpp>
#include <RIT/utils/enums/JSONFormat.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <iostream>
#include <string>
#include <utility>

TEST ( MST_2_TEST, SMALL ) {

	VertexIF* v1 = new VertexImpl { (VertexIdx) 0 };
	VertexIF* v2 = new VertexImpl { (VertexIdx) 1 };
	VertexIF* v3 = new VertexImpl { (VertexIdx) 2 };
	VertexSetIF* vSet = new VertexSetImpl { 3 };

	std::cout << v1->toString() << std::endl;
	std::cout << v2->toString() << std::endl;
	std::cout << v3->toString() << std::endl;

	EdgeIF* e1 = new EdgeImpl { 0, VertexPair(v1, v2), 3 };
	EdgeIF* e2 = new EdgeImpl { 1,  VertexPair(v2, v3), 4 };
	EdgeIF* e3 = new EdgeImpl { 2, VertexPair(v3, v1), 5 };
	EdgeSetIF* eSet = new EdgeSetImpl { 3 };

//	std::cout << e1->toString() << std::endl;
//	std::cout << e2->toString() << std::endl;
//	std::cout << e3->toString() << std::endl;
//
//	std::cout << v1->inputEdgesToString() << std::endl;
//	std::cout << v1->outputEdgesToString() << std::endl;
//
//	std::cout << v2->inputEdgesToString() << std::endl;
//	std::cout << v2->outputEdgesToString() << std::endl;
//
//	std::cout << v3->inputEdgesToString() << std::endl;
//	std::cout << v3->outputEdgesToString() << std::endl;

	GraphIF* g = new GraphImpl(vSet, eSet);

	MSTSolverIF* p = new PrimeHeap(g);

	vSet->push_back(v1);
	vSet->push_back(v2);
	vSet->push_back(v3);

	eSet->push_back(e1);
	eSet->push_back(e2);
	eSet->push_back(e3);

//	std::cout << v1->toJSONString(JSONFormat::NONE, 9) << std::endl;
//	std::cout << v2->toJSONString(JSONFormat::NONE, 9) << std::endl;
//	std::cout << v3->toJSONString(JSONFormat::NONE, 9) << std::endl;
//
//	std::cout << e1->toJSONString(JSONFormat::NONE, 9) << std::endl;
//	std::cout << e2->toJSONString(JSONFormat::NONE, 9) << std::endl;
//	std::cout << e3->toJSONString(JSONFormat::NONE, 9) << std::endl;
//
//	std::cout << vSet->toJSONString(JSONFormat::NONE, 9) << std::endl;
//
//	std::cout << eSet->toJSONString(JSONFormat::NONE, 9) << std::endl;
//
//	std::cout << g->toJSONString(JSONFormat::PRETTY, 3) << std::endl;

	EdgeSetIF* mst = p->getMST();

//	std::cout << mst->toString() << std::endl;
//
//	std::cout << eSet->toString() << std::endl;
//
//	std::cout << vSet->toString() << std::endl;

	MemoryUtils::removeCollection(mst, false);

	MemoryUtils::removeGraph(g);
	delete p;

}

