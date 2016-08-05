/*
 * TabuSearch.cpp
 *
 *  Created on: 30 lip 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/rimstsolver/RIMSTSolverInclude.hpp>
#include <RIT/enums/MSTSolverEnum.hpp>
#include <RIT/structures/EdgeSetIF.hpp>
#include <RIT/typedefs/primitive.hpp>
#include <RIT/typedefs/struct.hpp>
#include <RIT/utils/enums/InputFormat.hpp>
#include <RIT/utils/enums/InputMode.hpp>
#include <RIT/utils/IOUtils.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <iostream>
#include <set>
#include <string>
#include <utility>

TEST ( USE_CASE, TABU_SEARCH_EXAMPLE ) {

	std::ostringstream oss { };

	IncrementalParam k { 5 };
	TabuIterationCount tabuPeriod { 100 };
	TabuIterationCount numberOfPathIterations { 50 };
	TabuIterationCount numberOfIterations { 2000 };

	GraphIF* g = InputUtils::readGraph("test/rimst/TabuSearch/4/s0.json", InputFormat::VA, InputMode::HDD);
	GraphEdgeCostsSet adversarialScenarios { };

	for (int i = 1; i <= 2; i += 1) {
		oss << "test/rimst/TabuSearch/4/Adv/s" << i << ".json";
		adversarialScenarios.insert(
			InputUtils::readCosts(oss.str().c_str(), InputFormat::VA, InputMode::HDD)
		);
		oss.str("");
		oss.clear();
	}

	RIMSTSolverIF* rimstSolver = new RIMSTSolverImpl { AIMSTSolverEnum::DEFAULT,
		IMSTSolverEnum::DEFAULT, MSTSolverEnum::DEFAULT, g, adversarialScenarios, k,
		tabuPeriod, numberOfPathIterations, numberOfIterations
	};

	EdgeSetIF* solution = rimstSolver->getMST();
	delete rimstSolver;

	std::cout << solution->toString() << std::endl;
	std::cout << solution->getTotalEdgeCost() << std::endl;

	MemoryUtils::removeCollection(solution, false);
	MemoryUtils::removeScenarioSet(adversarialScenarios);
	MemoryUtils::removeGraph(g, true, true);

}
