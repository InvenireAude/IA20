/*
 * File: IA20-CommonLib/src/commonlib/testcase/TestRunner.h
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
#ifndef _IA20_TestRunner_H_
#define _IA20_TestRunner_H_

#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/types.h>
#include <ia20/commonlib/exception/ItemNotFoundException.h>

#include <map>

namespace IA20 {

/*************************************************************************************/

class TestRunnerInterface{
	public:
	virtual ~TestRunnerInterface(){};
	virtual void run(const String& strCaseName) const =0;
	virtual const StringList& getCases() const = 0;

};

/*************************************************************************************/

template<class TT>
class TestRunner : public TestRunnerInterface {
public:

	virtual ~TestRunner() throw() {};

	virtual const StringList& getCases() const  { return lstCases; };

	virtual void run(const String& strCaseName) const {
		IA20_CHECK_IF_VALID(pTestUnit);
		if(hmCases.count(strCaseName) == 0)
			IA20_THROW(ItemNotFoundException(strCaseName+" : case not found."));

		(pTestUnit->*hmCases.find(strCaseName)->second)();

	}

	typedef void (TT::*CaseMethodType)();

	TestRunner(TT* pTestUnit)throw():pTestUnit(pTestUnit){};

	void addCase(const String& strCaseName, CaseMethodType pMethod){
		if(hmCases.count(strCaseName) == 0)
				lstCases.push_back(strCaseName);
			hmCases[strCaseName]=pMethod;
	}

private:

	typedef    ::std::map<String, CaseMethodType> CasesMap;

	String     strName;
	CasesMap   hmCases;
	StringList lstCases;

	TT* pTestUnit;

};

/*************************************************************************/
}

#endif /* _IA20_TestRunner_H_ */
