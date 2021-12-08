/*
 * File: IA20-CommonLib/src/commonlib/testcase/TestSuite.h
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
#ifndef _IA20_TestSuite_H_
#define _IA20_TestSuite_H_

#include <ia20/commonlib/types.h>
#include <ia20/commonlib/memory/memory.h>
#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/containers/HashMapStringToPointer.h>
#include <ia20/commonlib/tools/TypeTools.h>
#include <ia20/commonlib/exception/ItemNotFoundException.h>
#include <ia20/commonlib/exception/BadUsageException.h>

#include "TestUnit.h"

#include <list>

namespace IA20 {

/*************************************************************************/
/** The TestSuite class.
 *
 */
class TestSuite {
public:

	TestSuite();
	virtual ~TestSuite() throw();

	void addTestUnit(TestUnitInterface *pTestUnit);

	void run(const String& strCaseName="");
	void printResults(std::ostream& os);

  void setVerbose(bool bVerbose = true);

  inline bool isVerbose()const{
    return bVerbose;
  }

protected:

	enum Result{
		RESULT_NOT_EXECUTED,
		RESULT_OK,
		RESULT_FAILED
	};

	struct TestCase{
		TestUnitInterface *pTestUnit;
		String strName;
		String strInfo;
		Result iResult;
		long   iTimeMS;
		long   iNewMemoryEntries;
	};

	typedef std::list<TestCase> TestCasesList;
	TestCasesList               lstTestCases;

	typedef std::map<String, TestUnitInterface* > TestUnitsMap;

	TestUnitsMap                             hmTestUnits;

  bool bVerbose;
};

/*************************************************************************/
}

#endif /* _IA20_TestSuite_H_ */
