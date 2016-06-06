/*
 * GRtoVA.cpp
 *
 *  Created on: 16 mar 2016
 *      Author: tomasz
 */

#include <gtest/gtest.h>
#include <RIT/exp/IOExceptions.hpp>
#include <RIT/utils/enums/GraphVizEngine.hpp>
#include <RIT/utils/enums/InputFormat.hpp>
#include <RIT/utils/enums/InputMode.hpp>
#include <RIT/utils/enums/OutputFormat.hpp>
#include <RIT/utils/IOUtils.hpp>
#include <RIT/utils/MemoryUtils.hpp>
#include <iostream>

TEST ( GRAPHVIZ_TEST, SMALL_GR_TO_VA ) {

	try {
		char const * filename = "test/minVA.gr";

		GraphIF * g = InputUtils::readGraph(filename, InputFormat::GR,
				InputMode::HDD);
		OutputUtils::exportGraph(g, "graph.va", OutputFormat::VA,
				GraphVizEngine::NEATO, 300, 300);
		MemoryUtils::removeGraph(g, true, true);
	} catch (const IOExceptions::FileNotFountException& e) {
		std::cout << e.what() << std::endl;
	}
}
