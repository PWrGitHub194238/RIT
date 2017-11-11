/*
 * SingleThread.cpp
 *
 *  Created on: 4 wrz 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/aimstsolver/AIMSTSolverIF.hpp>
#include <RIT/mstsolver/MSTSolverIF.hpp>
#include <RIT/structures/EdgeSetIF.hpp>
#include <RIT/typedefs/primitive.hpp>
#include <RIT/typedefs/struct.hpp>
#include <RIT/utils/AIMSTUtils.hpp>
#include <RIT/utils/enums/InputFormat.hpp>
#include <RIT/utils/enums/InputMode.hpp>
#include <RIT/utils/IOUtils.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <RIT/utils/SolverFactory.hpp>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

TEST ( USE_CASE, AIMST_ST_EXAMPLE ) {

	std::ostringstream oss { };

	IncrementalParam k = 5;
	GraphIF* g = InputUtils::readGraph("test/aimst/SingleThread/4/s0.json",
			InputFormat::VA, InputMode::HDD);
	GraphEdgeCostsSet adversarialScenarios { };
	MSTSolverIF* mstSolver = SolverFactory::getMSTSolver(g);
	AIMSTSolution aimstSolution { };
	EdgeSetIF* solution { };

	EdgeSetIF* baseSolution = mstSolver->getSolution();
	delete mstSolver;

	for (int i = 1; i <= 2; i += 1) {
		oss << "test/aimst/SingleThread/4/Adv/s" << i << ".json";
		adversarialScenarios.insert(
				InputUtils::readCosts(oss.str().c_str(), InputFormat::VA,
						InputMode::HDD));
		oss.str("");
		oss.clear();
	}

	AIMSTSolverIF* aimstSolver = SolverFactory::getAIMSTSolver(g,
			adversarialScenarios, k);

	aimstSolution = aimstSolver->getSolution(baseSolution);
	solution = AIMSTUtils::getEdgeSet(aimstSolution);

	MemoryUtils::removeCollection(baseSolution, false);
	delete aimstSolver;

	std::cout << solution->toString() << std::endl;
	std::cout << solution->getTotalEdgeCost() << std::endl;

	MemoryUtils::removeCollection(solution, false);
	MemoryUtils::removeScenarioSet(adversarialScenarios);
	MemoryUtils::removeGraph(g, true, true);

}
