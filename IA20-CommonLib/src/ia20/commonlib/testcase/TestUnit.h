/*
 * File: IA20-CommonLib/src/commonlib/testcase/TestUnit.h
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
#ifndef _IA20_TestUnit_H_
#define _IA20_TestUnit_H_

#include <ia20/commonlib/types.h>

#include <map>

#include "TestRunner.h"

namespace IA20 {
class TestSuite;

/*************************************************************************/
/** The TestUnit class.
 *
 */

class TestUnit {
public:

	TestUnit();
	virtual ~TestUnit() throw();

	const String&     getName() const;
	const StringList& getCases() const;

	virtual void run(const String& strCaseName) const;

protected:

	void init(const String& strName, const TestRunnerInterface *pTestRunner, TestSuite* pTestSuite);

	private:

	String     strName;

	const TestRunnerInterface *pTestRunner;
};

/*************************************************************************/
}

#endif /* _IA20_TestUnit_H_ */
