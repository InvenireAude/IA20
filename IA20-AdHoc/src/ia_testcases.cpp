/*
 * File: IA20-CommonLib-TestCases/src/commonlib-testcases.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <ia20/commonlib/commonlib.h>

#include <ia20/commonlib/testcase/TestSuite.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include "ia20/tc/TCRaftEngine.h"
#include "ia20/tc/TCConnectionTCP.h"
#include "ia20/tc/TCURing.h"

using namespace IA20;

int main(int argc, char* argv[]) {
	IA20_TRACER;
	IA20::Logger::GetInstance();

	std::cout << "Starting\n";

	try {

		std::unique_ptr<TestSuite> ptrSuite(new TestSuite());
    ptrSuite->setVerbose();

    TC::TCRaftEngine    tcRaftEngine(ptrSuite.get());
    TC::TCConnectionTCP tcConnectionTCP(ptrSuite.get());
    TC::TCURing         tcUring(ptrSuite.get());

		ptrSuite->run(argc == 1 ? "" : argv[1]);
		ptrSuite->printResults(std::cout);

	} catch (IA20::SystemException& e) {
		std::cerr << "System Exception:\n";
		e.printToStream(std::cerr);
		std::cerr << "\n";
	} catch (IA20::Exception& e) {
		std::cerr << "Exception:\n";
		e.printToStream(std::cerr);
		std::cerr << "\n";
	} catch (...) {
		std::cerr << "Unknown exception !\n";
	}

	// std::cout << "ALL:\n";
	// IA20::MemoryManager::GetInstance()->printToStream(std::cout);
	// std::cout << "Tracer:\n";
	// //IA20::TracerStats::GetInstance()->printToStream(std::cout);
	// std::cout << "Ending\n";
}
