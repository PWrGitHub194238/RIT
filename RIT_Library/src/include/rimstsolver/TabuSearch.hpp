/*
 * TabuSearch.hpp
 *
 *  Created on: 14 kwi 2016
 *      Author: tomasz
 */

#ifndef SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_
#define SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_

#include "../enums/AIMSTSolverEnum.hpp"
#include "../enums/IMSTSolverEnum.hpp"
#include "../enums/MSTSolverEnum.hpp"
#include "../typedefs/primitive.hpp"
#include "../typedefs/struct.hpp"
#include "../utils/enums/TabuMapEnum.hpp"
#include "RIMSTSolverIF.hpp"

class MSTSolverIF;

class GraphIF;
class IMSTSolverIF;

class TabuSearch: public RIMSTSolverIF {
private:

	//************************************ PRIVATE CONSTANT FIELDS *************************************//

	//************************************** PRIVATE CLASS FIELDS **************************************//

	TabuMap tabuMap;
	TabuIterationCount tabuPeriod;
	TabuIterationCount numberOfIterations;
	TabuIterationCount numberOfPathIterations;

	//*************************************** PRIVATE FUNCTIONS ****************************************//

	/** Zwraca najgorszy scenariusz dla podanego drzewa spośród wszystkich scenariuszy adwersarza
	 *
	 * @param spanningTree
	 * @return
	 */
	GraphEdgeCostsIF * getWorstCaseScenario(EdgeSetIF * spanningTree);

	/** Zwraca alternatywę dla najgorszego przypadku dla drzewa T:
	 * T^{*} = getWorstCaseAlternative(getWorstCaseScenario(T));
	 *
	 * @param worstCaseScenario
	 * @return
	 */
	EdgeSetIF * getWorstCaseAlternative(GraphEdgeCostsIF * worstCaseScenario);

	bool stopCriterionMet(TabuIterationCount iterationCount);

	bool isAspirationCriteriaMet(EdgeCost const neighborCost,
			EdgeCost const tmpSolutionCost);

	TabuSubMap getTabuList(TabuMapEnum tabuListType);

	bool isMoveAllowed(EdgeIdx const dropEdgeIdx, EdgeIdx const addEdgeIdx);

	/**
	 *
	 * @param initialSolution sąsiednie drzewo pierwotnego drzewa
	 * @return
	 */
	AIMSTSolution getSolutionForTree(EdgeSetIF* initialSolution);

	/** Już w czasie przeliczania kolejnych sąsiadów, wybiera tego najlepszego,
	 * zamiast policzyć wszystkich sąsiadów i wybrać minimum
	 *
	 * @param spanningTree
	 * @return
	 */
	NeighborSolution findMinimumInNeighborhood(EdgeSetIF* const spanningTree,
			EdgeCost const tmpSolutionCost);

	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx);

	void insertIntoTabu(TabuMapEnum const tabuListType, EdgeIdx const edgeIdx,
			TabuIterationCount tabuPeriod);

	void eraseTabuList();

	void updateTabuList(SpanningTreeNeighbor && tabuMove);

protected:

	//*********************************** PROTECTED CONSTANT FIELDS ************************************//

	//************************************ PROTECTED CLASS FIELDS **************************************//

	//************************************** PROTECTED FUNCTIONS ***************************************//

	virtual EdgeSetIF * resolve();

public:

	//************************************* PUBLIC CONSTANT FIELDS *************************************//

	//************************************** PUBLIC CLASS FIELDS ***************************************//

	//************************************ CONSTRUCTOR & DESTRUCTOR ************************************//

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(AIMSTSolverEnum aimstSolverType, IMSTSolverEnum imstSolverType,
			MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			MSTSolverEnum innerMstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			MSTSolverEnum innerMstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(IMSTSolverEnum imstSolverType, MSTSolverEnum mstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(MSTSolverEnum mstSolverType, MSTSolverEnum innerMstSolverType,
			GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k,
			TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(MSTSolverEnum mstSolverType, GraphIF * const graph,
			GraphEdgeCostsSet adversarialScenarioSet, IncrementalParam k);

	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k, TabuIterationCount tabuPeriod,
			TabuIterationCount numberOfPathIterations,
			TabuIterationCount numberOfIterations);

	TabuSearch(GraphIF * const graph, GraphEdgeCostsSet adversarialScenarioSet,
			IncrementalParam k);

	virtual ~TabuSearch() {
	}

	//*************************************** PUBLIC FUNCTIONS *****************************************//

	//*************************************** GETTERS & SETTERS ****************************************//

};

#endif /* SRC_INCLUDE_RIMSTSOLVER_TABUSEARCH_HPP_ */
