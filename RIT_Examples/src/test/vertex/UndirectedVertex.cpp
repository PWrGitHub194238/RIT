/*
 * Vertex.cpp
 *
 *  Created on: 12 sty 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/enums/EdgeByVertexKey.hpp>
#include <RIT/structures/EdgeByVertexSetInclude.hpp>
#include <RIT/structures/EdgeInclude.hpp>
#include <RIT/structures/EdgeSetInclude.hpp>
#include <RIT/structures/VertexInclude.hpp>
#include <RIT/structures/VertexSetInclude.hpp>
#include <RIT/typedefs/primitive.hpp>
#include <RIT/typedefs/struct.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <map>
#include <utility>

TEST ( UNDIRECTED_VERTEX_TEST, OUT_IN_EDGES_100_10000 ) {

	srand(time(nullptr));

	VertexCount i;
	VertexCount numberOfVertices = 100;
	VertexSetIF* vertexSet = new VertexSetImpl { numberOfVertices };

	EdgeCount j;
	EdgeIdx edgeIdx;
	EdgeCount numberOfEdges = 10000;
	EdgeIF* edge;
	EdgeSetIF* edgeSet = new EdgeSetImpl { numberOfEdges };

	VertexIdx vIdx, uIdx;
	EdgeCost eCost;
	std::map<VertexIdx, std::map<VertexIdx, EdgeCost>> sourceTestData;

	numberOfEdges = std::min(numberOfEdges,
			numberOfVertices * (numberOfVertices - 1) / 2);
	edgeIdx = 0;

	for (i = 0; i < numberOfVertices; i += 1) {
		vertexSet->push_back(new VertexImpl { i });
	}

	for (j = 0; j < numberOfEdges; j += 1) {
		uIdx = rand() % numberOfVertices;
		vIdx = rand() % numberOfVertices;
		if (uIdx == vIdx) {
			j -= 1;
			continue;
		}
		if (sourceTestData.count(uIdx) == 1
				&& sourceTestData.at(uIdx).count(vIdx) != 0) {
			j -= 1;
			continue;
		}
		if (sourceTestData.count(vIdx) == 1
				&& sourceTestData.at(vIdx).count(uIdx) != 0) {
			j -= 1;
			continue;
		}

		eCost = rand();

		if (!sourceTestData.count(uIdx)) {
			sourceTestData.insert(
					std::make_pair(uIdx, std::map<VertexIdx, EdgeCost> { }));
		}

		if (!sourceTestData.at(uIdx).count(vIdx)) {
			sourceTestData.at(uIdx).insert(std::make_pair(vIdx, eCost));
			edgeSet->push_back(
					new EdgeImpl { edgeIdx++, VertexPair(vertexSet->getElementAt(uIdx),
							vertexSet->getElementAt(vIdx)), eCost });
			/*std::cout << "ADD: (" << uIdx << " : " << vIdx << ") : " << eCost
			 << std::endl;*/
		}
	}

	EdgeByVertexSetIF** inputEdges = new EdgeByVertexSetIF*[numberOfVertices];
	EdgeByVertexSetIF** outputEdges = new EdgeByVertexSetIF*[numberOfVertices];
	EdgeByVertexSetIF* edgeByVertexSet;

	for (i = 0; i < numberOfVertices; i += 1) {
		inputEdges[i] = new EdgeByVertexSetImpl { vertexSet->getElementAt(i),
				EdgeByVertexKey::INCOMING_EDGES };
		outputEdges[i] = new EdgeByVertexSetImpl { vertexSet->getElementAt(i),
				EdgeByVertexKey::OUTGOING_EDGES };
	}

	for (i = 0; i < numberOfVertices; i += 1) {
		edgeSet->begin();
		while (edgeSet->hasNext()) {
			edge = edgeSet->next();
			inputEdges[i]->addEdge(edge);
			outputEdges[i]->addEdge(edge);
		}
	}

	/*	std::cout << "InputSets:" << std::endl;*/
	EdgeByVertexIdxPair evPair;
	for (i = 0; i < numberOfVertices; i += 1) {
		/*		std::cout << i << " -->" << std::endl;*/
		edgeByVertexSet = inputEdges[i];
		edgeByVertexSet->begin();
		while (edgeByVertexSet->hasNext()) {
			evPair = edgeByVertexSet->next();
			/*std::cout << "-->" << evPair.first << "\t:\t"
			 << evPair.second->toString() << std::endl;*/
		}
	}

	/*	std::cout << "OutputSets:" << std::endl;*/
	for (i = 0; i < numberOfVertices; i += 1) {
		/*		std::cout << i << " <--" << std::endl;*/
		edgeByVertexSet = outputEdges[i];
		edgeByVertexSet->begin();
		while (edgeByVertexSet->hasNext()) {
			evPair = edgeByVertexSet->next();
			/*	std::cout << "<--" << evPair.first << "\t:\t"
			 << evPair.second->toString() << std::endl;*/
		}
	}

	std::map<VertexIdx, std::map<VertexIdx, EdgeCost>>::const_iterator sourceItBegin;
	std::map<VertexIdx, std::map<VertexIdx, EdgeCost>>::const_iterator sourceItEnd;
	std::map<VertexIdx, EdgeCost>::const_iterator targetItBegin;
	std::map<VertexIdx, EdgeCost>::const_iterator targetItEnd;

	sourceItEnd = sourceTestData.end();
	for (sourceItBegin = sourceTestData.begin(); sourceItBegin != sourceItEnd;
			++sourceItBegin) {
		targetItEnd = (*sourceItBegin).second.end();
		for (targetItBegin = (*sourceItBegin).second.begin();
				targetItBegin != targetItEnd; ++targetItBegin) {
			/*std::cout << "Looking for edge (" << (*sourceItBegin).first
			 << " -> " << (*targetItBegin).first << ") with cost "
			 << (*targetItBegin).second << "..." << std::endl;*/
			edge = inputEdges[(*targetItBegin).first]->findEdge(
					(*sourceItBegin).first);
			/*std::cout << "Found edge ("
			 << edge->getSourceVertex()->getVertexIdx() << ","
			 << edge->getTargetVertex()->getVertexIdx()
			 << ") with cost: " << edge->getEdgeCost() << std::endl;*/
			ASSERT_TRUE(
					edge != nullptr
							&& edge->getEdgeCost() == (*targetItBegin).second);
			/*std::cout << "InputEdges[" << (*targetItBegin).first << "].size = "
			 << inputEdges[(*targetItBegin).first]->size() << std::endl;
			 std::cout << "OutputEdges[" << (*sourceItBegin).first << "].size = "
			 << outputEdges[(*sourceItBegin).first]->size() << std::endl;*/
			inputEdges[(*targetItBegin).first]->removeEdge(
					(*sourceItBegin).first);
			outputEdges[(*sourceItBegin).first]->removeEdge(
					(*targetItBegin).first);
			/*std::cout << "InputEdges[" << (*targetItBegin).first << "].size = "
			 << inputEdges[(*targetItBegin).first]->size() << std::endl;
			 std::cout << "OutputEdges[" << (*sourceItBegin).first << "].size = "
			 << outputEdges[(*sourceItBegin).first]->size() << std::endl;
			 std::cout << "Looking for edge (" << (*sourceItBegin).first
			 << " <- " << (*targetItBegin).first << ") with cost "
			 << (*targetItBegin).second << "..." << std::endl;*/
			edge = outputEdges[(*targetItBegin).first]->findEdge(
					(*sourceItBegin).first);
			/*std::cout << "Found edge ("
			 << edge->getSourceVertex()->getVertexIdx() << ","
			 << edge->getTargetVertex()->getVertexIdx()
			 << ") with cost: " << edge->getEdgeCost() << std::endl;*/
			ASSERT_TRUE(
					edge != nullptr
							&& edge->getEdgeCost() == (*targetItBegin).second);
			/*std::cout << "OutputEdges[" << (*targetItBegin).first << "].size = "
			 << outputEdges[(*targetItBegin).first]->size() << std::endl;
			 std::cout << "InputEdges[" << (*sourceItBegin).first << "].size = "
			 << inputEdges[(*sourceItBegin).first]->size() << std::endl;*/
			outputEdges[(*targetItBegin).first]->removeEdge(
					(*sourceItBegin).first);
			inputEdges[(*sourceItBegin).first]->removeEdge(
					(*targetItBegin).first);
			/*std::cout << "OutputEdges[" << (*targetItBegin).first << "].size = "
			 << outputEdges[(*targetItBegin).first]->size() << std::endl;
			 std::cout << "InputEdges[" << (*sourceItBegin).first << "].size = "
			 << inputEdges[(*sourceItBegin).first]->size() << std::endl;*/
		}
	}

	for (i = 0; i < numberOfVertices; i += 1) {
		/*std::cout << "InputEdges[" << i << "].size = " << inputEdges[i]->size()
		 << std::endl;*/
		ASSERT_TRUE(inputEdges[i]->size() == 0);
		/*std::cout << "OutputEdges[" << i << "].size = "
		 << outputEdges[i]->size() << std::endl;*/
		ASSERT_TRUE(outputEdges[i]->size() == 0);
	}

	for (i = 0; i < numberOfVertices; i += 1) {
		delete inputEdges[i];
		delete outputEdges[i];
	}
	delete[] inputEdges;
	delete[] outputEdges;

	MemoryUtils::removeCollection(vertexSet, true);
	MemoryUtils::removeCollection(edgeSet, true);
}
