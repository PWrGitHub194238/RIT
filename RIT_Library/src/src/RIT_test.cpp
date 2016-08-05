#include <log4cxx/logger.h>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include "../include/enums/AIMSTSolverEnum.hpp"
#include "../include/enums/Connectivity.hpp"
#include "../include/enums/IMSTSolverEnum.hpp"
#include "../include/enums/MSTSolverEnum.hpp"
#include "../include/enums/Visibility.hpp"
#include "../include/log/utils/LocaleEnum.hpp"
#include "../include/log/utils/LogUtils.hpp"
#include "../include/rimstsolver/RIMSTSolverInclude.hpp"
#include "../include/structures/EdgeIF.hpp"
#include "../include/structures/EdgeSetIF.hpp"
#include "../include/structures/GraphEdgeCostsIF.hpp"
#include "../include/structures/GraphIF.hpp"
#include "../include/typedefs/primitive.hpp"
#include "../include/typedefs/struct.hpp"
#include "../include/utils/enums/InputFormat.hpp"
#include "../include/utils/enums/InputMode.hpp"
#include "../include/utils/IOUtils.hpp"
#include "../include/utils/MemoryUtils.hpp"

// TODO Jeśli koszty grafu zmieni się poza solverem (nie w getMST()) to solver tego nie wykrywa, trzeba za każsym razem sprawdzać, nie tylko gdy nie null.


void changeCostBack(GraphIF* g, GraphEdgeCostsIF* costs) {
	costs->begin();
	g->beginEdge();
				while (g->hasNextEdge()) {
					g->nextEdge()->setEdgeCost(costs->next());
				}
}

EdgeCost edgeCostPerturbFunction(EdgeCount const numberOfEdges,
		EdgeIdx const edgeIdx, EdgeCost const edgeCost) {
	return edgeCost
			+ (EdgeCost) (numberOfEdges * (edgeIdx * edgeIdx) + edgeIdx)
					/ ((numberOfEdges + 1) * (numberOfEdges + 1)
							* (numberOfEdges + 1));
}

void changeCost(GraphIF* g) {
	int edgeIdx;
		EdgeIF* edge;
		g->beginEdge();

			edgeIdx = 1;
			while (g->hasNextEdge(Connectivity::CONNECTED, Visibility::VISIBLE)) {
				edge = g->nextEdge();

				edge->setEdgeCost(
						edgeCostPerturbFunction(g->getNumberOfEdges(), edgeIdx,
								edge->getEdgeCost()));
				edgeIdx += 1;
			}
}




int main(int argc, char **argv) {

	log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("log"));

	LogUtils::configureLog("Log4cxxConfig.xml");

	LogUtils::setLocale(LocaleEnum::EN_US_UTF8);


	// Generowanie grafów od 100 do 10000 wierzchołków, z gęstością = 1.0

	/*GraphIF* g { };
	std::ostringstream oss { };

	for ( int i = 100; i <= 10000; i += 100) {
		std::cout << i << std::endl;
		g = GraphUtils::getRandomGraph(i,1.0,1,1000);	// koszty tutaj  itak nic nie znaczą, bo są modyfikowane dla imst
		oss << "/home/tomasz/Pulpit/Dane/rnd/N_" << i << "_D_1.gr";
		OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
		oss.str("");
		oss.clear();
		MemoryUtils::removeGraph(g);
	}
*/




	/*char * a = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_IV/INC-MST-example/a.json";

	char * b = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_IV/INC-MST-example/b.json";

	GraphEdgeCostsIF* cost = InputUtils::readCosts(b,InputFormat::VA, InputMode::RAM);

	GraphIF * g = InputUtils::readGraph(a, InputFormat::VA, InputMode::RAM);

	MSTSolverIF* mstSolver = new MSTSolverImpl { g };

	EdgeSetIF* e1 = mstSolver->getMST();

	std::cout << "E1: " << e1->toString() << std::endl << std::endl;

	std::cout << "G: " << g->toString() << std::endl;


	GraphUtils::changeGraphCosts(g, cost);



	std::cout << "G: " << g->toString() << std::endl;

	EdgeSetIF* e2 = mstSolver->getMST();

	std::cout << "E2: " << e2->toString() << std::endl << std::endl;

	std::cout << LogStringUtils::mstEdgeDifference(e1,e2) << std::endl;

	EdgeIF* e { };

	e1->begin();
	while(e1->hasNext()) {
		e = e1->next();
		e->setEdgeCost(e->getEdgeCost() - 4);
	}

	std::cout << "G: " << g->toString() << std::endl;


	IteratorId i = g->getVertexIteratorId();
	g->beginVertex(i);
	while(g->hasNextVertex(i)) {
		VertexIF* v = g->nextVertex(i);
	e2 = mstSolver->getMST(v);

	std::cout << "E2: (" << v->getVertexIdx() << ") " << e2->toString() << std::endl << e2->getTotalEdgeCost() << std::endl;

	std::cout << LogStringUtils::mstEdgeDifference(e1,e2) << std::endl;
	}*/



	/*// RIMST, 30 nodów

	char * a0 = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
	char * a1 = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s1.json";
	char * a2 = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s2.json";


	GraphIF* g = InputUtils::readGraph(a0, InputFormat::VA, InputMode::HDD);
	GraphEdgeCostsSet advScenarios { };
	advScenarios.insert(InputUtils::readCosts(a1, InputFormat::VA, InputMode::HDD, "s1"));
	advScenarios.insert(InputUtils::readCosts(a2, InputFormat::VA, InputMode::HDD, "s2"));

	RIMSTSolverIF* rimstSolver = new RIMSTSolverImpl { g, advScenarios, 1, 60, 10, 5 };

	EdgeSetIF* solution = rimstSolver->getMST();

	std::cout << "COST: " << solution->getTotalEdgeCost() << std::endl;

	MemoryUtils::removeCollection(solution, false);
	delete rimstSolver;
	MemoryUtils::removeScenarioSet(advScenarios);
	MemoryUtils::removeGraph(g);*/









	/*// Kopia grafu, MST
	char * a0 = "/home/tomasz/Pulpit/TB_MST_Examples/1/s0.json";
	char * a1 = "/home/tomasz/Pulpit/TB_MST_Examples/1/Adv/s1.json";

	GraphIF* g = InputUtils::readGraph(a0, InputFormat::VA, InputMode::HDD);
	GraphEdgeCostsIF* costs = InputUtils::readCosts(a1, InputFormat::VA, InputMode::HDD);
	GraphIF* g1 = new GraphImpl { g };
	MemoryUtils::removeGraph(g);

	MSTSolverIF* mstSolver = new MSTSolverImpl { g1 };
	IMSTSolverIF* imstSolver = new IMSTSolverImpl { mstSolver, g1 };
	EdgeSetIF* solution = imstSolver->getMST(1,costs);

	std::cout << "COST: " << solution->getTotalEdgeCost() << std::endl;
	MemoryUtils::removeCollection(solution, false);
	delete imstSolver;
	delete mstSolver;
	delete costs;
	MemoryUtils::removeGraph(g1);*/




	/*// AIMST, 30 nodów
	char * a0 = "/home/tomasz/Pulpit/TB_MST_Examples/1/s0.json";
	char * a1 = "/home/tomasz/Pulpit/TB_MST_Examples/1/Adv/s1.json";
	char * a2 = "/home/tomasz/Pulpit/TB_MST_Examples/1/Adv/s2.json";


	GraphIF* g = InputUtils::readGraph(a0, InputFormat::VA, InputMode::HDD);

	GraphEdgeCostsIF* s1 = InputUtils::readCosts(a1, InputFormat::VA, InputMode::HDD);

	GraphEdgeCostsIF* s2 = InputUtils::readCosts(a2, InputFormat::VA, InputMode::HDD);


	GraphEdgeCostsSet adv { };
	adv.insert(s1);
	adv.insert(s2);

	AIMSTSolverIF* aimstSolver = new AIMSTSolverImpl { g, adv, 1 };

	MSTSolverIF* mstSolver = new MSTSolverImpl { g };
	EdgeSetIF* sol = mstSolver->getMST();
	AIMSTSolution solution = aimstSolver->getMST(sol);

	MemoryUtils::removeCollection(sol, false);
	delete mstSolver;

	EdgeSetIF* solutionSet = AIMSTUtils::getEdgeSet(solution);
	GraphEdgeCostsIF* solutionScenario = AIMSTUtils::getScenario(solution);
	EdgeCost solutionCost = AIMSTUtils::getSolutionCost(solution);

	std::cout << "COST: " << solutionCost << std::endl;

	MemoryUtils::removeCollection(solutionSet, false);
	delete aimstSolver;
	MemoryUtils::removeScenarioSet(adv);
	MemoryUtils::removeGraph(g);*/












	/*//Zaburzenie kosztów, MST, 30 nodów
	char * a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";

	 GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	 GraphEdgeCostsIF* costs = new GraphEdgeCostsImpl { g };

	 MSTSolverIF* mstSolver = new MSTSolverImpl { g };

	 EdgeSetIF* sol = mstSolver->getMST();

	 std::cout << "COST: " << sol->getTotalEdgeCost() << std::endl;

	changeCost(g);

	sol = mstSolver->getMST();

	changeCostBack(g,costs);

		 std::cout << "COST: " << sol->getTotalEdgeCost() << std::endl;*/






	/*// LP MST v2, 30 nodów
	char * a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";

	 GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	 //TODO zabronienie odpalenia 2 razy getMST() na tym samym obiecie
	 CPLEX_LP_MSTSolverIF* mstSolver = new CPLEX_LP_MSTSolver_v2 { g };

	 EdgeSetIF* solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	mstSolver = new CPLEX_LP_MSTSolver_v2 { g };

	EdgeSetIF* smallSet = new EdgeSetImpl { };

	g->beginEdge();
	while(g->hasNextEdge()) {
		smallSet->push_back(g->nextEdge());
	}

	solution = mstSolver->getMST(smallSet);
	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	mstSolver = new CPLEX_LP_MSTSolver_v2 { g };

	MemoryUtils::removeCollection(smallSet, false);

	solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	MemoryUtils::removeGraph(g);*/









	/*// LP MST v3, 30 nodów
	char * a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";

	 GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	 //TODO zabronienie odpalenia 2 razy getMST() na tym samym obiecie
	 CPLEX_LP_MSTSolverIF* mstSolver = new CPLEX_LP_MSTSolver_v3 { g };

	 EdgeSetIF* solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	mstSolver = new CPLEX_LP_MSTSolver_v3 { g };

	EdgeSetIF* smallSet = new EdgeSetImpl { };

	g->beginEdge();
	while(g->hasNextEdge()) {
		smallSet->push_back(g->nextEdge());
	}

	solution = mstSolver->getMST(smallSet);
	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	mstSolver = new CPLEX_LP_MSTSolver_v3 { g };

	MemoryUtils::removeCollection(smallSet, false);

	solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	MemoryUtils::removeGraph(g);*/





	/*//MST 30 nodów, normalnie, z selekcją, normalnie

	char* a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
	GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);
	MSTSolverIF* mstSolver = new MSTSolverImpl { g };
	EdgeSetIF* solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);

	EdgeSetIF* smallSet = new EdgeSetImpl { };

	g->beginEdge();
	while(g->hasNextEdge()) {
		smallSet->push_back(g->nextEdge());
	}

	solution = mstSolver->getMST(smallSet);
	MemoryUtils::removeCollection(solution, false);

	MemoryUtils::removeCollection(smallSet, false);

	solution = mstSolver->getMST();

	MemoryUtils::removeCollection(solution, false);
	delete mstSolver;
	MemoryUtils::removeGraph(g);*/





	/*// IMST, 30 nodów, random graph, worst case scenario

	GraphIF* g = GraphUtils::getRandomGraph(50,0.4,1,9);

	//GraphIF* g = InputUtils::readGraph("/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json", InputFormat::VA, InputMode::HDD);

	IMSTSolverIF* imstSolver = new BinarySearch_v2 { MSTSolverEnum::DEFAULT, g };

	EdgeSetIF* mstSol = imstSolver->getMST();

	GraphEdgeCostsIF* costs = GraphEdgeCostUtils::getInverseCaseScenario(g, mstSol, 18);

	//GraphEdgeCostsIF* costs = InputUtils::readCosts("/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s1.json", InputFormat::VA, InputMode::HDD);

	MemoryUtils::removeCollection(mstSol, false);

	EdgeSetIF* solution = imstSolver->getIMST(1, costs);

	std::cout << "COST: " << solution->getTotalEdgeCost() << std::endl;

	MemoryUtils::removeCollection(solution, false);
	delete imstSolver;
	delete costs;
	MemoryUtils::removeGraph(g);*/







	/*// IMST, 30 nodów
	  char* a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
	//char * a = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/a.json";

	char* b = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s1.json";
	//char * b = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/b.json";

	GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	GraphEdgeCostsIF* costs = InputUtils::readCosts(b, InputFormat::VA, InputMode::HDD);

	MSTSolverIF* mstSolver = new MSTSolverImpl { g };

	IMSTSolverIF* imstSolver = new IMSTSolverImpl { mstSolver, g };

	EdgeSetIF* solution = imstSolver->getMST(1, costs);

	std::cout << "COST: " << solution->getTotalEdgeCost() << std::endl;

	MemoryUtils::removeCollection(solution, false);
	delete imstSolver;
	delete mstSolver;
	delete costs;
	MemoryUtils::removeGraph(g);*/





	/*//LP IMST, 30 nodów, bez wymuszenia mst (różne wyniki LP i zwykłego imst)
		char* a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
		//char * a = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/a.json";

		char* b = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s2.json";
		//char * b = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/b.json";

		GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

		GraphEdgeCostsIF* costs = InputUtils::readCosts(b, InputFormat::VA, InputMode::HDD);

		CPLEX_LP_MSTSolverIF* lpMstSolver = new CPLEX_LP_MSTSolverImpl { g };
		//MSTSolverIF* mstSolver = new MSTSolverImpl { g };

		//EdgeSetIF* ee = mstSolver->getMST();

		CPLEX_LP_IMSTSolverIF* imst = new CPLEX_LP_IMSTSolverImpl { lpMstSolver, g };//, ee };

		//MemoryUtils::removeCollection(ee, false);
		//delete mstSolver;

		EdgeSetIF* e = imst->getMST(1, costs);

		std::cout << "COST: " << e->getTotalEdgeCost() << std::endl;

		delete costs;
		delete lpMstSolver;
		delete imst;

		MemoryUtils::removeCollection(e, false);
		MemoryUtils::removeGraph(g);*/






	/*//LP IMST, 30 nodów z wymuszonym MST
	char* a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
	//char * a = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/a.json";

	char* b = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s1.json";
	//char * b = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/b.json";

	GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	GraphEdgeCostsIF* costs = InputUtils::readCosts(b, InputFormat::VA, InputMode::HDD);

	MSTSolverIF* mstSolver = new MSTSolverImpl { g };

	EdgeSetIF* ee = mstSolver->getMST();

	CPLEX_LP_MSTSolverIF* mst = new CPLEX_LP_MSTSolverImpl { g };

	CPLEX_LP_IMSTSolverIF* imst = new CPLEX_LP_IMSTSolverImpl { mst, g, ee };

	MemoryUtils::removeCollection(ee, false);
	delete mstSolver;

	EdgeSetIF* e = imst->getMST(1, costs);

	std::cout << "COST: " << e->getTotalEdgeCost() << std::endl;

	delete costs;
	delete mst;
	delete imst;

	MemoryUtils::removeCollection(e, false);
	MemoryUtils::removeGraph(g);*/




	/*char * a = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/a.json";

	char * b = "/home/tomasz/Pulpit/[WPPT W-11 194238] Praca magisterska/Thesis/Chapter_II/INC-MST-example/b.json";

	GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	GraphEdgeCostsIF* costs = InputUtils::readCosts(b, InputFormat::VA, InputMode::HDD);

	MSTSolverIF* mst = new MSTSolverImpl { g };

	IMSTSolverIF* imst = new IMSTSolverImpl { mst, g };

	EdgeSetIF* e = imst->getMST(1, costs);

	std::cout << e->getTotalEdgeCost() << std::endl;

	delete costs;
	delete mst;
	delete imst;

	MemoryUtils::removeCollection(e, false);
	MemoryUtils::removeGraph(g);*/





	/*
	char * a = "/home/tomasz/Pulpit/TB_MST_Examples/1/s0.json";

	GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	CPLEX_LP_IMSTSolverIF* solver = new CPLEX_LP_IMSTSolverImpl { g };

	EdgeSetIF* e = solver->getMST(0);

	delete solver;

	MemoryUtils::removeCollection(e, false);

	MemoryUtils::removeGraph(g);
*/











	/*char * a = "/home/tomasz/Pulpit/TB_MST_Examples/1/s0.json";

	 GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	 CPLEX_LP_MSTSolverIF* solver = new CPLEX_LP_MSTSolverImpl { g };

	 EdgeSetIF* e = solver->getMST();

	 delete solver;

	 MemoryUtils::removeCollection(e, false);

	 MemoryUtils::removeGraph(g);*/




	/*char * a = "/home/tomasz/Pulpit/TB_MST_Examples/4/s0.json";
	 char * a1 = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s1.json";
	 char * a2 = "/home/tomasz/Pulpit/TB_MST_Examples/4/Adv/s2.json";

	 GraphIF* g = InputUtils::readGraph(a, InputFormat::VA, InputMode::HDD);

	 GraphEdgeCostsSet adversarialScenarioSet { };

	 adversarialScenarioSet.insert(
	 InputUtils::readCosts(a1, InputFormat::VA, InputMode::HDD, "s1"));

	 adversarialScenarioSet.insert(
	 InputUtils::readCosts(a2, InputFormat::VA, InputMode::HDD, "s2"));

	 RIMSTSolverIF* solver = new RIMSTSolverImpl { g, adversarialScenarioSet, 1 };

	 EdgeSetIF* s = solver->getMST();

	 std::cout << "SOLUTION: " << s->toString() << "\nwith cost: " << s->getTotalEdgeCost() << std::endl;

	 MemoryUtils::removeCollection(s, false);

	 MemoryUtils::removeScenarioSet(adversarialScenarioSet);

	 MemoryUtils::removeGraph(g);

	 delete solver;*/






	/*	EdgeSetIF* s = GraphUtils::getRandomSpanningTree(g);

	 EdgeSetIF* ss = GraphUtils::getMSTEdgesBeetwenNodes(g, s, 0, 2);

	 SpanningTreeNeighborhood neighborhood = GraphUtils::getEdgeSetNeighbourhood(
	 g, s);

	 for (auto& neighbor : neighborhood) {
	 MemoryUtils::removeCollection(TabuSearchUtils::getEdgeSet(neighbor),
	 false);
	 }

	 MemoryUtils::removeCollection(s, false);

	 MemoryUtils::removeCollection(ss, false);

	 MemoryUtils::removeGraph(g);*/

	/*GraphEdgeCostsIF * s1 = InputUtils::readCosts(a1, InputFormat::VA,
	 InputMode::HDD);

	 GraphEdgeCostsIF * s2 = InputUtils::readCosts(a2, InputFormat::VA,
	 InputMode::HDD);
	 //IMSTSolverIF* solver = new BinarySearch_v1 { g };

	 bool vectorsX[][6] = {
	 { 1, 0, 1, 1, 0, 1 },
	 { 1, 1, 0, 1, 0, 1 },
	 { 1, 1,	1, 0, 0, 1 },
	 { 1, 1, 1, 0, 1, 0 },
	 { 0, 1, 0, 1, 1, 1 },
	 { 0, 1, 1, 0, 1, 1 },
	 { 0, 1, 1, 1, 1, 0 },
	 { 1, 1, 0, 1, 1, 0 },
	 { 1, 0, 0, 1, 1, 1 },
	 { 1, 0, 1, 0, 1, 1 },
	 { 1, 0, 1, 1, 1, 0 },
	 { 0, 1, 1, 1, 0, 1 }
	 };

	 EdgeSetIF ** edgeSetArray = new EdgeSetIF*[12] { };

	 //e_{23}	e_{34}	e_{41}	e_{12}	e_{25}	e_{54}


	 for (int i = 0; i < 12; i += 1) {
	 edgeSetArray[i] = new EdgeSetImpl { };
	 if (vectorsX[i][0]) {
	 edgeSetArray[i]->push_back(g->findEdge(2-1, 3-1));
	 }
	 if (vectorsX[i][1]) {
	 edgeSetArray[i]->push_back(g->findEdge(3-1, 4-1));
	 }
	 if (vectorsX[i][2]) {
	 edgeSetArray[i]->push_back(g->findEdge(4-1, 1-1));
	 }
	 if (vectorsX[i][3]) {
	 edgeSetArray[i]->push_back(g->findEdge(1-1, 2-1));
	 }
	 if (vectorsX[i][4]) {
	 edgeSetArray[i]->push_back(g->findEdge(2-1, 5-1));
	 }
	 if (vectorsX[i][5]) {
	 edgeSetArray[i]->push_back(g->findEdge(5-1, 4-1));
	 }
	 }

	 IMSTSolverIF* solver { };
	 EdgeSetIF * s { };
	 EdgeCost s1c { };
	 EdgeCost s2c { };
	 for (int i = 7; i < 8; i += 1) {

	 std::cout << vectorsX[i][0] << "\t" << vectorsX[i][1] << "\t" << vectorsX[i][2] << "\t"
	 << vectorsX[i][3] << "\t" << vectorsX[i][4] << "\t" << vectorsX[i][5] << "\t" << std::flush;
	 solver = new BinarySearch_v1 { g, edgeSetArray[i] };
	 s = solver->getMST(1,s1);
	 std::cout << s->toString() << std::endl;
	 s1c = s->getTotalEdgeCost();
	 std::cout << s1c << "\t" << std::flush;

	 delete solver;
	 MemoryUtils::removeCollection(s,false);

	 solver = new BinarySearch_v1 { g, edgeSetArray[i] };
	 s = solver->getMST(1,s2);
	 std::cout << s->toString() << std::endl;
	 s2c = s->getTotalEdgeCost();
	 std::cout << s2c << "\t" << std::flush;

	 delete solver;
	 MemoryUtils::removeCollection(s,false);

	 std::cout << (s1c < s2c ? s2c : s1c) << std::endl;
	 }

	 for (int i = 0; i < 12; i += 1) {
	 MemoryUtils::removeCollection(edgeSetArray[i], false);
	 }
	 delete[] edgeSetArray;

	 delete s1;
	 delete s2;


	 MemoryUtils::removeGraph(g);*/

	/*
	 GraphEdgeCostsIF * s2 = InputUtils::readCosts(a2, InputFormat::VA,
	 InputMode::HDD);

	 std::cout << s2->toString() << std::endl;
	 //solver->changeEdgeCost(1, 4, 7);
	 //solver->changeEdgeCost(2, 4, 4);

	 EdgeSetIF * s = solver->getMST(1,s2);

	 //std::cout << s->toString() << std::endl;
	 //std::cout << s->getTotalEdgeCost() << std::endl;
	 //MSTSolverIF* solver = new LP_MSTSolver_v1 { g };

	 //EdgeSetIF * s = solver->getMST();

	 delete s;
	 delete solver;

	 MemoryUtils::removeGraph(g);

	 delete s2;*/
	return 0;
}

