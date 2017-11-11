/*
 * solveFromVA.cpp
 *
 *  Created on: 21 lut 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/enums/Visibility.hpp>
#include <RIT/mstsolver/MSTSolverInclude.hpp>
#include <RIT/structures/EdgeSetIF.hpp>
#include <RIT/structures/GraphIF.hpp>
#include <RIT/utils/enums/InputFormat.hpp>
#include <RIT/utils/enums/InputMode.hpp>
#include <RIT/utils/GraphUtils.hpp>
#include <RIT/utils/IOUtils.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <iostream>
#include <string>

TEST ( MST_TEST, SMALL_VA ) {

	char const * filename = "test/va.json";
	GraphIF* g = InputUtils::readGraph(filename, InputFormat::VA,
			InputMode::HDD);

	std::cout << g->toString() << std::endl;

	MSTSolverIF* p = new MSTSolverImpl { g };

	EdgeSetIF* eSetMST = p->getSolution();
	std::cout << "MST: " << eSetMST->toString() << std::endl;

	std::cout << "total cost: " << eSetMST->getTotalEdgeCost() << std::endl;
	std::cout << "END" << std::endl;

	GraphUtils::isGraphConnected(g, eSetMST);

	std::cout << "EDGEs ALL: " << g->edgeSetToString() << std::endl;

	std::cout << "EDGEs BOTH: " << g->edgeSetToString(Visibility::BOTH)
			<< std::endl;
	std::cout << "EDGEs HIDE: " << g->edgeSetToString(Visibility::HIDDEN)
			<< std::endl;
	std::cout << "EDGEs VIS: " << g->edgeSetToString(Visibility::VISIBLE)
			<< std::endl;

	MemoryUtils::removeCollection(eSetMST, false);
	MemoryUtils::removeGraph(g);
	delete p;

}

