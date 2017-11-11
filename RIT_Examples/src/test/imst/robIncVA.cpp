/*
 * robIncVA.cpp
 *
 *  Created on: 10 kwi 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/exp/IOExceptions.hpp>
#include <RIT/imstsolver/IMSTSolverInclude.hpp>
#include <RIT/structures/EdgeSetInclude.hpp>
#include <RIT/structures/GraphIF.hpp>
#include <RIT/typedefs/primitive.hpp>
#include <RIT/utils/enums/InputFormat.hpp>
#include <RIT/utils/enums/InputMode.hpp>
#include <RIT/utils/IOUtils.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <iostream>

TEST ( RIMST_TEST, SMALL_VA_NO_CHANGES ) {

	char const * filename = "test/va.json";
	try {

		char * a = "test/a.json";
		char * a1 = "test/a1.json";
		char * a2 = "test/a2.json";

		GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

		GraphEdgeCostsIF * s1 = InputUtils::readCosts(a1, InputFormat::VA,
				InputMode::HDD);

		GraphEdgeCostsIF * s2 = InputUtils::readCosts(a2, InputFormat::VA,
				InputMode::HDD);

		//e_{23}	e_{34}	e_{41}	e_{12}	e_{25}	e_{54}

		bool vectorsX[][6] = { { 1, 0, 1, 1, 0, 1 }, { 1, 1, 0, 1, 0, 1 }, { 1,
				1, 1, 0, 0, 1 }, { 1, 1, 1, 0, 1, 0 }, { 0, 1, 0, 1, 1, 1 }, {
				0, 1, 1, 0, 1, 1 }, { 0, 1, 1, 1, 1, 0 }, { 1, 1, 0, 1, 1, 0 },
				{ 1, 0, 0, 1, 1, 1 }, { 1, 0, 1, 0, 1, 1 },
				{ 1, 0, 1, 1, 1, 0 }, { 0, 1, 1, 1, 0, 1 } };

		EdgeSetIF ** edgeSetArray = new EdgeSetIF*[12] { };

		for (int i = 0; i < 12; i += 1) {
			edgeSetArray[i] = new EdgeSetImpl { };
			if (vectorsX[i][0]) {
				edgeSetArray[i]->push_back(g->findEdge(2 - 1, 3 - 1));
			}
			if (vectorsX[i][1]) {
				edgeSetArray[i]->push_back(g->findEdge(3 - 1, 4 - 1));
			}
			if (vectorsX[i][2]) {
				edgeSetArray[i]->push_back(g->findEdge(4 - 1, 1 - 1));
			}
			if (vectorsX[i][3]) {
				edgeSetArray[i]->push_back(g->findEdge(1 - 1, 2 - 1));
			}
			if (vectorsX[i][4]) {
				edgeSetArray[i]->push_back(g->findEdge(2 - 1, 5 - 1));
			}
			if (vectorsX[i][5]) {
				edgeSetArray[i]->push_back(g->findEdge(5 - 1, 4 - 1));
			}
		}

		IMSTSolverIF* solver { };
		EdgeSetIF * s { };
		EdgeCost s1c { };
		EdgeCost s2c { };
		for (int i = 0; i < 12; i += 1) {

			std::cout << vectorsX[i][0] << "\t" << vectorsX[i][1] << "\t"
					<< vectorsX[i][2] << "\t" << vectorsX[i][3] << "\t"
					<< vectorsX[i][4] << "\t" << vectorsX[i][5] << "\t"
					<< std::flush;
			solver = new IMSTSolverImpl { g, edgeSetArray[i] };
			s = solver->getSolution(1,s1);
			//std::cout << s->toString() << std::endl;
			s1c = s->getTotalEdgeCost();
			std::cout << s1c << "\t" << std::flush;

			delete solver;
			MemoryUtils::removeCollection(s, false);

			solver = new IMSTSolverImpl { g, edgeSetArray[i] };
			s = solver->getSolution(1,s2);
			//std::cout << s->toString() << std::endl;
			s2c = s->getTotalEdgeCost();
			std::cout << s2c << "\t" << std::flush;

			delete solver;
			MemoryUtils::removeCollection(s, false);

			std::cout << (s1c < s2c ? s2c : s1c) << std::endl;
		}

		for (int i = 0; i < 12; i += 1) {
			MemoryUtils::removeCollection(edgeSetArray[i], false);
		}
		delete[] edgeSetArray;

		delete s1;
		delete s2;

		MemoryUtils::removeGraph(g);

	} catch (const IOExceptions::FileNotFountException& e) {
		std::cout << e.what() << std::endl;
	}
}
