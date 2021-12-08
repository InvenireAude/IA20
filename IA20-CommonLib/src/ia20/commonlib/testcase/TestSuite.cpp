/*
 * File: IA20-CommonLib/src/commonlib/testcase/TestSuite.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
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
#include "TestSuite.h"

#include <ia20/commonlib/commonlib.h>
#include "TestUnit.h"
#include <ia20/commonlib/time/Timestamp.h>

namespace IA20 {

/*************************************************************************/
TestSuite::TestSuite():
  bVerbose(false),
  hmTestUnits(){
	IA20_TRACER;
}
/*************************************************************************/
TestSuite::~TestSuite() throw () {
	IA20_TRACER;
}
/*************************************************************************/
void TestSuite::addTestUnit(TestUnitInterface *pTestUnit) {
	IA20_TRACER;
	IA20_CHECK_IF_VALID(pTestUnit);

	if(hmTestUnits.count(pTestUnit->getName()) != 0){
		IA20_THROW(BadUsageException("TestUnit alread exists: ")<<pTestUnit->getName());
	}

	hmTestUnits[pTestUnit->getName()] = pTestUnit;

	const StringList& lstCases = pTestUnit->getCases();

	IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(), "Adding: "<<pTestUnit->getName()<<", no cases="<<lstCases.size());

	for (StringList::const_iterator it = lstCases.begin(); it != lstCases.end(); it++) {

		TestCase aNewTestCase;

		aNewTestCase.pTestUnit = pTestUnit;
		aNewTestCase.strName = *it;
		aNewTestCase.iResult = RESULT_NOT_EXECUTED;
		aNewTestCase.iNewMemoryEntries=0;
		aNewTestCase.iTimeMS=0;
		lstTestCases.push_back(aNewTestCase);
	}

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "TS:"<<pTestUnit->getName()<<", sz: "<<hmTestUnits.size());
}
/*************************************************************************/
void TestSuite::setVerbose(bool bVerbose) {
  this->bVerbose = bVerbose;
}
/*************************************************************************/
void TestSuite::run(const String& strCaseName) {
	IA20_TRACER;

  IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(), "Testing : "<<strCaseName);

	for (TestCasesList::iterator it = lstTestCases.begin(); it != lstTestCases.end(); it++) {

		if(!strCaseName.empty() && strCaseName.compare((*it).pTestUnit->getName())!=0)
			continue;

		IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(), "TEST Started: "<<(*it).pTestUnit->getName()<<":"<<(*it).strName);

    if(bVerbose){
      std::cout<<"*******************************************************"<<std::endl;
      std::cout<<"**  TEST CASE :"<<(*it).pTestUnit->getName()<<":"<<(*it).strName<<std::endl;
      std::cout<<"*******************************************************"<<std::endl;
    }

		try {

			MemoryManager::GetInstance()->clearNewFlag();
			TimeSample ts(true);
			(*it).pTestUnit->run((*it).strName);
			(*it).iTimeMS=ts.getSample()/1000;
			(*it).iResult = RESULT_OK;
			(*it).iNewMemoryEntries=MemoryManager::GetInstance()->getNumerOfNew();

		} catch (Exception& e) {

			StringStream ssInfo;
			e.printToStream(ssInfo);
			(*it).strInfo = ssInfo.str();
			(*it).iResult = RESULT_FAILED;

		} catch (std::exception& e) {

			(*it).strInfo = e.what();
			(*it).iResult = RESULT_FAILED;

		} catch (...) {

			(*it).strInfo = "unhandled exception";
			(*it).iResult = RESULT_FAILED;
		}

		IA20_LOG(IA20::LogLevel::INSTANCE.isInfo(), "TEST Ended: "<<(*it).strName);
	}
}
/*************************************************************************/
void TestSuite::printResults(std::ostream& os) {
	IA20_TRACER;

	for (TestCasesList::iterator it = lstTestCases.begin(); it != lstTestCases.end(); it++)
  if((*it).iResult != Result::RESULT_NOT_EXECUTED){

		char sBuffer[2024];
		snprintf(sBuffer, 2024, "TestCase: %48s : %30s | %d : %ld : %6ld : %s \n", (*it).pTestUnit->getName().c_str(),
				(*it).strName.c_str(), (*it).iResult, (*it).iNewMemoryEntries, (*it).iTimeMS,(*it).strInfo.c_str());

		os << sBuffer;

	}
}
/*************************************************************************/
}
