/*
 * solveFromVA.cpp
 *
 *  Created on: 22 lut 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/enums/EdgeConnectionType.hpp>
#include <RIT/exp/IOExceptions.hpp>
#include <RIT/imstsolver/IMSTSolverInclude.hpp>
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

TEST ( IMST_TEST, SMALL_VA_NO_CHANGES ) {

	char const * filename = "test/va.json";
	try {
		GraphIF* g = InputUtils::readGraph(filename, InputFormat::VA,
				InputMode::HDD);

		std::cout << g->toString() << std::endl;

		IMSTSolverIF* bs = new IMSTSolverImpl { MSTSolverEnum::DEFAULT, g };

		EdgeSetIF* kmst = bs->getSolution(100);

		std::cout << kmst->toString() << std::endl;
		std::cout << g->edgeSetToString() << std::endl;

		if (GraphUtils::isGraphConnected(g, kmst)) {
			std::cout << "OK" << std::endl;
		}

		MemoryUtils::removeCollection(kmst, false);
		MemoryUtils::removeGraph(g);
		delete bs;
	} catch (const IOExceptions::FileNotFountException& e) {
		std::cout << e.what() << std::endl;
	}
}

TEST ( IMST_TEST, SMALL_VA_SMALL_MOD ) {
	char const * filename = "test/va.json";
	try {
		GraphIF* g = InputUtils::readGraph(filename, InputFormat::VA,
				InputMode::HDD);

		std::cout << g->toString() << std::endl;

		IMSTSolverIF* bs = new IMSTSolverImpl { MSTSolverEnum::DEFAULT, g };
		std::cout << "BEG: " << g->edgeSetToString() << std::endl;

		bs->changeEdgeCost(0, 7, EdgeConnectionType::FORWARD, 18);
		bs->changeEdgeCost(6, 5, 9);

		std::cout << "END: " << g->edgeSetToString() << std::endl;
		EdgeSetIF* kmst = bs->getSolution(3);

		std::cout << kmst->toString() << std::endl;
		std::cout << "KOSZT: " << kmst->getTotalEdgeCost() << std::endl;
		std::cout << g->edgeSetToString() << std::endl;

		if (GraphUtils::isGraphConnected(g, kmst)) {
			std::cout << "OK" << std::endl;
		}

		MemoryUtils::removeCollection(kmst, false);
		MemoryUtils::removeGraph(g);
		delete bs;
	} catch (const IOExceptions::FileNotFountException& e) {
		std::cout << e.what() << std::endl;
	}
}
