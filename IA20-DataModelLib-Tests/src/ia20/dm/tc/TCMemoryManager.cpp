/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCMemoryManager.cpp
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


#include "TCMemoryManager.h"


namespace IA20{

using namespace DM;

namespace TC{
/*************************************************************************/
TCMemoryManager::TCMemoryManager(TestSuite* pTestSuite):
  TestUnit<TCMemoryManager>(this, "MemoryManager", pTestSuite){
	IA20_TRACER;

  addCase("caseAllocate", &::IA20::TC::TCMemoryManager::caseAllocate);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCMemoryManager::~TCMemoryManager() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void TCMemoryManager::caseAllocate(){

	IA20::TimeSample ts(true);

  std::unique_ptr<DM::Memory::MemoryManager> mm(new DM::Memory::MemoryManager());

}
/*************************************************************************/
}
}
