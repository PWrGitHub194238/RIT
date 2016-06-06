/*
 * VertexSetIT.cpp
 *
 *  Created on: 12 mar 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/structures/VertexInclude.hpp>
#include <RIT/structures/VertexSetInclude.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <cstdlib>

TEST ( ITERATORS, VERTEX_SET_IT ) {

	const unsigned int numberOfElemenrs = 1000;
	const unsigned int numberOfIterators = 10;
	const unsigned int maxItId = 1000;

	bool itIdArray[maxItId];

	unsigned int itIds[numberOfIterators];
	unsigned int nextBy[numberOfIterators];

	VertexSetIF * vSet = new VertexSetImpl { numberOfElemenrs };

	for (int i = 0; i < maxItId; i += 1) {
		itIdArray[i] = false;
	}

	for (int i = 0; i < numberOfElemenrs; i += 1) {
		vSet->push_back(new VertexImpl { i });
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		nextBy[i] = rand() % numberOfElemenrs;
	}

	for (int i = 0; i < numberOfIterators / 2; i += 1) {
		do {
			itIds[i] = rand() % maxItId;
		} while (itIdArray[itIds[i]] == true);
		itIdArray[itIds[i]] = true;

		vSet->begin(itIds[i]);
		for (int j = 0; j < nextBy[i]; j += 1) {
			vSet->next(itIds[i]);
		}
	}

	for (int i = 0; i < numberOfIterators / 2; i += 1) {
		ASSERT_FALSE(vSet->current(itIds[i])->getVertexIdx() != nextBy[i]);
	}

	for (int i = 0; i < numberOfIterators / 2; i += 1) {
		for (int j = 0; j < nextBy[i]; j += 1) {
			vSet->previous(itIds[i]);
		}
	}

	for (int i = 0; i < numberOfIterators / 2; i += 1) {
		ASSERT_FALSE(vSet->current(itIds[i])->getVertexIdx() != 0);
	}

	for (int i = numberOfIterators / 2; i < numberOfIterators; i += 1) {
		itIds[i] = vSet->getIterator();
		vSet->begin(itIds[i]);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		for (int j = 0; j < nextBy[i]; j += 1) {
			vSet->next(itIds[i]);
		}
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		ASSERT_FALSE(vSet->current(itIds[i])->getVertexIdx() != nextBy[i]);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		vSet->end(itIds[i]);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		ASSERT_FALSE(
				vSet->previous(itIds[i])->getVertexIdx()
						!= numberOfElemenrs - 1);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		if (nextBy[i] > 0) {
			for (int j = 0; j < nextBy[i]; j += 1) {
				vSet->previous(itIds[i]);
			}
		}
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		if (nextBy[i] > 0) {
			ASSERT_FALSE(
					vSet->current(itIds[i])->getVertexIdx()
							!= numberOfElemenrs - 1 - nextBy[i]);
		}
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		vSet->begin(itIds[i]);
		for (int j = 0; j < numberOfElemenrs; j += 1) {
			ASSERT_FALSE(vSet->peek(itIds[i], j)->getVertexIdx() != j);
		}
		ASSERT_FALSE(
				vSet->peek(itIds[i], numberOfElemenrs + 1)->getVertexIdx()
						!= numberOfElemenrs - 1);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		for (int j = 0; j < nextBy[i]; j += 1) {
			ASSERT_FALSE(vSet->peek(itIds[i], j)->getVertexIdx() != j);
		}
		ASSERT_FALSE(vSet->peek(itIds[i], -1)->getVertexIdx() != 0);
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		vSet->end(itIds[i]);
		for (int j = 0; j < numberOfElemenrs; j += 1) {
			ASSERT_FALSE(
					vSet->peek(itIds[i], -j)->getVertexIdx()
							!= numberOfElemenrs - 1 - j);
		}
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		for (int j = 0; j < nextBy[i]; j += 1) {
			ASSERT_FALSE(
					vSet->peek(itIds[i], -j)->getVertexIdx()
							!= numberOfElemenrs - 1 - j);
		}
	}

	for (int i = 0; i < numberOfIterators; i += 1) {
		vSet->removeIterator(itIds[i]);
	}

	MemoryUtils::removeCollection(vSet, true);
}
