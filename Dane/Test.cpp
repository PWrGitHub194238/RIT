/*
 * Main.cpp
 *
 *  Created on: 23 maj 2016
 *      Author: tomasz
 */

#include <IMST/rimstsolver/RIMSTSolverInclude.hpp>
#include <IMST/structures/EdgeSetIF.hpp>
#include <IMST/structures/GraphIF.hpp>
#include <IMST/typedefs/primitive.hpp>
#include <IMST/typedefs/struct.hpp>
#include <IMST/utils/enums/InputFormat.hpp>
#include <IMST/utils/enums/InputMode.hpp>
#include <IMST/utils/IOUtils.hpp>
#include <IMST/utils/MemoryUtils.hpp>
#include <iostream>
#include <set>
#include <string>
#include <utility>

class MSTSolverIF;

class CPLEX_LP_IMSTSolverIF;

class GraphIF;


//GlpkUtils.exportIncrementalData(GraphIF * const graph, char const * exportPath, GraphEdgeCostsIF * const adversarialScenario)

void exportIncrementalData(GraphIF * const graph, char const * exportPath, GraphEdgeCostsIF * const adversarialScenario) {
	std::ofstream myfile;
	  myfile.open (exportPath);
	  graph->
	  myfile << "Writing this to a file.\n";
	  myfile.close();
}

int main() {

	//GraphIF* g { };
	//std::ostringstream oss { };

	//std::string path = "/home/s194238/outGraph";

	//std::string path = "/home/tomasz/workspace";

	/*for (int i = 50; i <= 1000; i += 10) {
	 std::cout << i << std::endl;
	 g = GraphUtils::getRandomGraph(i, 1.0, 1, 1000);// koszty tutaj  itak nic nie znaczą, bo są modyfikowane dla imst
	 oss << path << "/N_" << i << "_D_1.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }
	 }*/

	/*MSTSolverIF* mstSolver { };
	 EdgeSetIF* mstSolution { };
	 GraphEdgeCostsIF* worstScenario { };

	 for (int i = 50; i <= 1000; i += 10) {
	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_1.gr";

	 g = InputUtils::readGraph(oss.str().c_str(), InputFormat::GR,
	 InputMode::RAM);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 worstScenario = GraphEdgeCostUtils::getInverseCaseScenario(g,
	 mstSolution, 1000);
	 GraphUtils::changeGraphCosts(g, worstScenario);

	 oss.str("");
	 oss.clear();
	 delete mstSolver;
	 MemoryUtils::removeCollection(mstSolution, false);

	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }*/

	/*MSTSolverIF* mstSolver { };
	 IMSTSolverIF* imstSolver { };
	 GraphEdgeCostsIF* advCost { };
	 EdgeSetIF* mstSolution { };
	 EdgeSetIF* imstSolution { };
	 clock_t begin { };
	 clock_t end { };
	 double elapsed_secs { };
	 double maxTime { };
	 std::string graphPath { };
	 std::string outPath { };

	 int iMax { 1000 };
	 int jMax { 100 };

	 std::ofstream myfile { };

	 oss << path << "/BS2_50_1000_10_D_1_opt.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();
	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);

	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();
	 // BS_1

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v2 { g };
	 begin = clock();
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }

	 oss << path << "/BS1_50_1000_10_D_1_opt.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();

	 // BS_1

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v1 { g };
	 begin = clock();
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }

	 maxTime = elapsed_secs;

	 CPLEX_LP_IMSTSolverIF* mipSolver { };

	 oss << path << "/MIP_50_1000_10_D_1_opt.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();

	 // BS_1

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 mipSolver = new CPLEX_LP_IMSTSolverImpl { g };
	 begin = clock();
	 imstSolution = mipSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete mipSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();
	 if (elapsed_secs > maxTime) {
	 break;
	 }
	 }*/

	/*MSTSolverIF* mstSolver { };
	 IMSTSolverIF* imstSolver { };
	 GraphEdgeCostsIF* advCost { };
	 EdgeSetIF* mstSolution { };
	 EdgeSetIF* imstSolution { };
	 std::string graphPath { };
	 std::string outPath { };

	 int iMax { 1000 };

	 std::ofstream myfile { };

	 oss << path << "/BS2_50_1000_10_D_1_sol.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();
	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);

	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();
	 // BS_1

	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v2 { g };
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution, advCost);

	 myfile << i << " " << imstSolution->getTotalEdgeCost() << std::endl
	 << std::flush;
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);

	 myfile.close();

	 }

	 oss << path << "/BS1_50_1000_10_D_1_sol.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 std::cout << i << std::endl;

	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();

	 // BS_1

	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v1 { g };
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 myfile << i << " " << imstSolution->getTotalEdgeCost() << std::endl << std::flush;

	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);

	 myfile.close();

	 }

	 CPLEX_LP_IMSTSolverIF* lpSolver { };

	 oss << path << "/MIP_50_1000_10_D_1_sol.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 oss << path << "/N_" << i << "_D_1.gr";
	 graphPath = oss.str();

	 // BS_1

	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_1_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 lpSolver = new CPLEX_LP_IMSTSolver_v3 { g, mstSolution };
	 imstSolution = lpSolver->getIMST((i - 1) / 20,
	 advCost);
	 myfile << i << " " << imstSolution->getTotalEdgeCost() << std::endl << std::flush;

	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete lpSolver;
	 MemoryUtils::removeGraph(g);

	 myfile.close();
	 }*/

	/*	for (int i = 50; i <= 1410; i += 10) {
	 std::cout << i << std::endl;
	 g = GraphUtils::getRandomGraph(i, 1.0, 0.5, 1000); // koszty tutaj  itak nic nie znaczą, bo są modyfikowane dla imst
	 oss << path << "/N_" << i << "_D_05.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }

	 MSTSolverIF* mstSolver { };
	 EdgeSetIF* mstSolution { };
	 GraphEdgeCostsIF* worstScenario { };

	 for (int i = 50; i <= 1410; i += 10) {
	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_05.gr";

	 g = InputUtils::readGraph(oss.str().c_str(), InputFormat::GR,
	 InputMode::RAM);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 worstScenario = GraphEdgeCostUtils::getInverseCaseScenario(g,
	 mstSolution, 1000);
	 GraphUtils::changeGraphCosts(g, worstScenario);

	 oss.str("");
	 oss.clear();
	 delete mstSolver;
	 MemoryUtils::removeCollection(mstSolution, false);

	 oss << path << "/N_" << i << "_D_05_WC_1000.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }*/

	/*MSTSolverIF* mstSolver { };
	 IMSTSolverIF* imstSolver { };
	 GraphEdgeCostsIF* advCost { };
	 EdgeSetIF* mstSolution { };
	 EdgeSetIF* imstSolution { };
	 clock_t begin { };
	 clock_t end { };
	 double elapsed_secs { };
	 std::string graphPath { };
	 std::string outPath { };

	 int iMax { 1410 };
	 int jMax { 50 };

	 std::ofstream myfile { };

	 oss << path << "/BS2_50_1410_10_D_05.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();
	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);

	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_05.gr";
	 graphPath = oss.str();
	 // BS_2

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_05_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v2 { g };
	 begin = clock();
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }

	 oss << path << "/BS1_50_1410_10_D_05_opt.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 890; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 oss << path << "/N_" << i << "_D_05.gr";
	 graphPath = oss.str();

	 // BS_1

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_05_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v1 { g };
	 begin = clock();
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }

	 for (int i = 50; i <= 3160; i += 10) {
	 std::cout << i << std::endl;
	 g = GraphUtils::getRandomGraph(i, 0.1, 1, 1000); // koszty tutaj  itak nic nie znaczą, bo są modyfikowane dla imst
	 oss << path << "/N_" << i << "_D_01.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }

	 //MSTSolverIF* mstSolver { };
	 //EdgeSetIF* mstSolution { };
	 GraphEdgeCostsIF* worstScenario { };

	 for (int i = 50; i <= 3160; i += 10) {
	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_01.gr";

	 g = InputUtils::readGraph(oss.str().c_str(), InputFormat::GR,
	 InputMode::RAM);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 worstScenario = GraphEdgeCostUtils::getInverseCaseScenario(g,
	 mstSolution, 1000);
	 GraphUtils::changeGraphCosts(g, worstScenario);

	 oss.str("");
	 oss.clear();
	 delete mstSolver;
	 MemoryUtils::removeCollection(mstSolution, false);

	 oss << path << "/N_" << i << "_D_01_WC_1000.gr";
	 OutputUtils::exportGraph(g, oss.str().c_str(), OutputFormat::GR);
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeGraph(g);
	 }*/

	/*MSTSolverIF* mstSolver { };
	 IMSTSolverIF* imstSolver { };
	 GraphEdgeCostsIF* advCost { };
	 EdgeSetIF* mstSolution { };
	 EdgeSetIF* imstSolution { };
	 clock_t begin { };
	 clock_t end { };
	 double elapsed_secs { };
	 std::string graphPath { };
	 std::string outPath { };

	 int iMax { 3160 };
	 int jMax { 50 };

	 std::ofstream myfile { };

	 oss << path << "/BS2_50_3160_10_D_01.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();
	 // 860 ????
	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);

	 std::cout << i << std::endl;
	 oss << path << "/N_" << i << "_D_01.gr";
	 graphPath = oss.str();
	 // BS_2

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_01_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v2 { g };
	 begin = clock();

	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }

	 oss << path << "/BS1_50_3160_10_D_01_opt.data";
	 outPath = oss.str();
	 oss.str("");
	 oss.clear();

	 for (int i = 50; i <= iMax; i += 10) {
	 myfile.open(outPath.c_str(), std::ios::out | std::ios::app);
	 oss << path << "/N_" << i << "_D_01.gr";
	 graphPath = oss.str();

	 // BS_1

	 elapsed_secs = 0;
	 for (int j = 0; j < jMax; j += 1) {
	 std::cout << "(" << j << ")" << std::endl;
	 g = InputUtils::readGraph(graphPath.c_str(), InputFormat::GR,
	 InputMode::HDD);
	 oss.str("");
	 oss.clear();
	 oss << path << "/N_" << i << "_D_01_WC_1000.gr";
	 advCost = InputUtils::readCosts(oss.str().c_str(), InputFormat::GR,
	 InputMode::HDD);
	 mstSolver = new MSTSolverImpl { g };
	 mstSolution = mstSolver->getMST();
	 imstSolver = new BinarySearch_v1 { g };
	 begin = clock();
	 imstSolution = imstSolver->getIMST((i - 1) / 20, mstSolution,
	 advCost);
	 std::cout << "\t" << imstSolution->getTotalEdgeCost() << std::endl;
	 end = clock();
	 oss.str("");
	 oss.clear();
	 MemoryUtils::removeCollection(mstSolution, false);
	 delete mstSolver;
	 delete advCost;
	 MemoryUtils::removeCollection(imstSolution, false);
	 delete imstSolver;
	 MemoryUtils::removeGraph(g);
	 elapsed_secs = elapsed_secs + double(end - begin) / CLOCKS_PER_SEC;
	 }
	 elapsed_secs /= jMax;

	 myfile << i << " " << elapsed_secs << std::endl << std::flush;
	 myfile.close();

	 }*/

	return 0;
}

