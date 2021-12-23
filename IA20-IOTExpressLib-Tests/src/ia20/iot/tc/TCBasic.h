/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCBasic.h
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
#ifndef _IA20_TC_TCBasic_H_
#define _IA20_TC_TCBasic_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include <ia20/iot/iot.h>
#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/memory/FixedObjectsPool.h>

namespace IA20{
namespace TC{


class TCBasic:
  public TestUnit<TCBasic>{

public:

	TCBasic(TestSuite* pTestSuite);
	virtual ~TCBasic()  throw ();


	void caseSharableMemoryPool();
	void caseStreamBufferList();
	void caseFixedObjectsPool();

protected:

  struct TestEnv {

    std::unique_ptr<IOT::Memory::SharableMemoryPool> ptrMemoryPool;
    void reset();
  };

  struct TestEnv env;

  void checkSBL(const String& strValue, int n);

};


}
}


#endif /*_IA20_TC_TCBasic_H_*/
