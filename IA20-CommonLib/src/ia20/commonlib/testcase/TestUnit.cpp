/*
 * File: IA20-CommonLib/src/commonlib/testcase/TestUnit.cpp
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
#include "TestUnit.h"

#include <ia20/commonlib/commonlib.h>

#include "TestSuite.h"

namespace IA20 {

/*************************************************************************/
TestUnit::TestUnit(){
	IA20_TRACER;
}
/*************************************************************************/
TestUnit::~TestUnit() throw(){
	IA20_TRACER;
}
/*************************************************************************/
const String& TestUnit::getName() const{
	IA20_TRACER;
	return strName;
}
/*************************************************************************/
const StringList& TestUnit::getCases() const{
	IA20_TRACER;
	IA20_CHECK_IF_VALID(pTestRunner);
	return pTestRunner->getCases();
}
/*************************************************************************/
void TestUnit::run(const String& strCaseName) const{
	IA20_TRACER;
	IA20_CHECK_IF_VALID(pTestRunner);
	pTestRunner->run(strCaseName);
}
/*************************************************************************/
void TestUnit::init(const String& strName, const TestRunnerInterface *pTestRunner, TestSuite* pTestSuite){
	IA20_TRACER;
	this->strName=strName;
	IA20_CHECK_IF_VALID(pTestSuite);
	IA20_CHECK_IF_VALID(pTestRunner);
	this->pTestRunner=pTestRunner;
	pTestSuite->addTestUnit(this);
}
/*************************************************************************/
}
