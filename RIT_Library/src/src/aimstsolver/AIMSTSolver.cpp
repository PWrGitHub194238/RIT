/*
 * TabuSearch.cpp
 *
 *  Created on: 14 kwi 2016
 *      Author: tomasz
 */

#include "../../include/aimstsolver/AIMSTSolver.hpp"

#include <log4cxx/helpers/messagebuffer.h>
#include <log4cxx/logger.h>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>         // std::thread

#include "../../include/enums/Visibility.hpp"
#include "../../include/imstsolver/IMSTSolverInclude.hpp"
#include "../../include/log/bundle/Bundle.hpp"
#include "../../include/log/utils/LogStringUtils.hpp"
#include "../../include/log/utils/LogUtils.hpp"
#include "../../include/mstsolver/MSTSolverIF.hpp"
#include "../../include/structures/EdgeSetIF.hpp"
#include "../../include/structures/GraphInclude.hpp"
#include "../../include/utils/AIMSTUtils.hpp"
#include "../../include/utils/MemoryUtils.hpp"
#include "../../include/utils/SolverFactory.hpp"

const static log4cxx::LoggerPtr logger(
		log4cxx::Logger::getLogger("aimstsolver.AIMSTSolver"));

//************************************ PRIVATE CONSTANT FIELDS *************************************//

//************************************** PRIVATE CLASS FIELDS **************************************//

//*************************************** PRIVATE FUNCTIONS ****************************************//

void AIMSTSolver::getIMST(ThreadId threadId, GraphIF* const graph,
		EdgeSetIF* const baseSolution, GraphEdgeCostsIF* scenario,
		EdgeSetIF*& outputSolution) {
	INFO(logger, LogBundleKey::AIMST_THREAD_RUN, threadId);
	IMSTSolverIF* imstSolver = new IMSTSolverImpl { mstSolverType, graph,
			baseSolution };
	outputSolution = imstSolver->getIMST(k, scenario);
	delete imstSolver;
	INFO(logger, LogBundleKey::AIMST_THREAD_END, threadId);
}

//*********************************** PROTECTED CONSTANT FIELDS ************************************//

//************************************ PROTECTED CLASS FIELDS **************************************//

AIMSTSolution AIMSTSolver::resolve(EdgeSetIF* const baseSolution) {
	ThreadId i { };
	MSTSolverIF* mstSolver { };
	EdgeSetIF* solution { };
	EdgeSetIF* returnedSolution { };
	EdgeCost solutionCost { };
	GraphEdgeCostsIF* solutionScenario { };
	GraphEdgeCostsSet::const_iterator scenarioIt { };
	EdgeCount numberOfEdgesInSolution { };
	EdgeSetIF** solutionSet { };
	GraphIF** threadGraphSet { };
	EdgeSetIF** baseSolutionSet { };

	std::thread* scenarioThreads { };
	ThreadId numberOfScenarios { adversarialScenarioSet.size() };
	if (numberOfScenarios > 0) {
		INFO(logger, LogBundleKey::AIMST_RESOLVE_INIT, numberOfScenarios);
		numberOfEdgesInSolution = graph->getNumberOfVertices(
				Visibility::VISIBLE) - 1;
		solutionSet = new EdgeSetIF*[numberOfScenarios] { };
		scenarioThreads = new std::thread[numberOfScenarios] { };
		threadGraphSet = new GraphIF*[numberOfScenarios] { };
		baseSolutionSet = new EdgeSetIF*[numberOfScenarios] { };

		scenarioIt = adversarialScenarioSet.begin();
		for (i = 0; i < numberOfScenarios; i += 1) {
			INFO(logger, LogBundleKey::AIMST_RESOLVE_RUN_THREAD, i,
					LogStringUtils::edgeCostSetDescription(*scenarioIt, "\t").c_str());
			threadGraphSet[i] = new GraphImpl { graph };
			baseSolutionSet[i] = AIMSTUtils::getEdgeSetCopy(threadGraphSet[i],
					baseSolution);
			if (AIMSTUtils::MULTITHREAD) {
				scenarioThreads[i] = std::thread(&AIMSTSolver::getIMST, this, i,
						threadGraphSet[i], baseSolutionSet[i], *scenarioIt,
						std::ref(solutionSet[i]));
			} else {
				getIMST(i, threadGraphSet[i], baseSolutionSet[i], *scenarioIt,
						std::ref(solutionSet[i]));
			}
			++scenarioIt;
		}

		INFO_NOARG(logger, LogBundleKey::AIMST_RESOLVE_JOIN);
		if (AIMSTUtils::MULTITHREAD) {
			for (i = 0; i < numberOfScenarios; i += 1) {
				scenarioThreads[i].join();
			}
		}
		INFO(logger, LogBundleKey::AIMST_RESOLVE_JOINED, numberOfScenarios);

		scenarioIt = adversarialScenarioSet.begin();
		solution = solutionSet[0];
		solutionScenario = *scenarioIt;
		solutionCost = solution->getTotalEdgeCost();
		INFO(logger, LogBundleKey::AIMST_RESOLVE_THREAD_SOL, 0,
				LogStringUtils::edgeSetVisualization(solution, "\t").c_str(),
				solutionCost);
		for (i = 1; i < numberOfScenarios; i += 1) {
			++scenarioIt;
			INFO(logger, LogBundleKey::AIMST_RESOLVE_THREAD_SOL, i,
					LogStringUtils::edgeSetVisualization(solutionSet[i], "\t").c_str(),
					solutionSet[i]->getTotalEdgeCost());
			if (solutionCost < solutionSet[i]->getTotalEdgeCost()) {
				INFO(logger, LogBundleKey::AIMST_RESOLVE_THREAD_BETTER_SOL, i,
						solutionCost, solutionSet[i]->getTotalEdgeCost());
				solution = solutionSet[i];
				solutionScenario = *scenarioIt;
				solutionCost = solution->getTotalEdgeCost();
			}
		}
		returnedSolution = AIMSTUtils::getEdgeSetCopy(graph, solution);

		INFO(logger, LogBundleKey::AIMST_RESOLVE_THREAD_SOLUTION,
				numberOfScenarios,
				LogStringUtils::edgeCostSetDescription(solutionScenario, "\t").c_str(),
				LogStringUtils::edgeSetVisualization(returnedSolution, "\t").c_str(),
				returnedSolution->getTotalEdgeCost());

		for (i = 0; i < numberOfScenarios; i += 1) {
			MemoryUtils::removeCollection(solutionSet[i], false);
			MemoryUtils::removeGraph(threadGraphSet[i]);
			MemoryUtils::removeCollection(baseSolutionSet[i], false);
		}
		delete[] solutionSet;
		delete[] scenarioThreads;
		delete[] threadGraphSet;
		delete[] baseSolutionSet;

		return AIMSTUtils::createAIMSTSolution(returnedSolution,
				solutionScenario, solutionCost);
	} else {
		WARN_NOARG(logger, LogBundleKey::AIMST_RESOLVE_NO_SCENARIO);
		mstSolver = SolverFactory::getMSTSolver(mstSolverType, graph);
		returnedSolution = mstSolver->getMST();
		delete mstSolver;
		return AIMSTUtils::createAIMSTSolution(returnedSolution, nullptr,
				returnedSolution->getTotalEdgeCost());
	}
}

//************************************** PROTECTED FUNCTIONS ***************************************//

//************************************* PUBLIC CONSTANT FIELDS *************************************//

//************************************** PUBLIC CLASS FIELDS ***************************************//

//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

AIMSTSolver::AIMSTSolver(IMSTSolverEnum imstSolverType,
		MSTSolverEnum mstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		AIMSTSolverIF(imstSolverType, mstSolverType, graph,
				adversarialScenarioSet, k) {
}

AIMSTSolver::AIMSTSolver(IMSTSolverEnum imstSolverType, GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		AIMSTSolverIF(imstSolverType, graph, adversarialScenarioSet, k) {

}

AIMSTSolver::AIMSTSolver(GraphIF * const graph,
		GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k) :
		AIMSTSolverIF(graph, adversarialScenarioSet, k) {

}

//*************************************** PUBLIC FUNCTIONS *****************************************//

//*************************************** GETTERS & SETTERS ****************************************//

