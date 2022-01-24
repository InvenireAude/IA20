/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCBasic.cpp
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


#include "TCBasic.h"

#include <memory>
#include <string.h>

using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCBasic::TCBasic(TestSuite* pTestSuite):
  TestUnit<TCBasic>(this, "Basic", pTestSuite){
	IA20_TRACER;

  addCase("caseSMP",  &::IA20::TC::TCBasic::caseSharableMemoryPool);
  addCase("caseSBL",  &::IA20::TC::TCBasic::caseStreamBufferList);
  addCase("caseSBLC", &::IA20::TC::TCBasic::caseStreamBufferCopy);
  addCase("caseFOP",  &::IA20::TC::TCBasic::caseFixedObjectsPool);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCBasic::~TCBasic() throw(){
	IA20_TRACER;
}
/*************************************************************************/

class Test1K {
  uint8_t data[1000];
};
/*************************************************************************/

void TCBasic::caseSharableMemoryPool(){

	IA20::TimeSample ts(true);
  env.reset();

  Test1K* p1 = env.ptrMemoryPool->allocate<Test1K>(0);
  Test1K* p2 = env.ptrMemoryPool->allocate<Test1K>(p1);

  if(p2 - p1 != 1)
    IA20_THROW(BadUsageException("(p2 - p1 != 1"));

  Test1K* p3 = env.ptrMemoryPool->allocate<Test1K>(p1);
  Test1K* p4 = env.ptrMemoryPool->allocate<Test1K>(p1);
  Test1K* p5 = env.ptrMemoryPool->allocate<Test1K>(p1);

  if(p4 - p3 != 1)
    IA20_THROW(BadUsageException("(p4 - p3 != 1"));

  if(p5 - p4 == 1)
    IA20_THROW(BadUsageException("(p5 - p4 != 1"));

  env.ptrMemoryPool->free(p1);

  Test1K* p99 = env.ptrMemoryPool->allocate<Test1K>(p1);

  // if(p5 != p99)
  //   IA20_THROW(BadUsageException("p5 == p99"));

}

/*************************************************************************/
void TCBasic::checkSBL(const String& strValue, int n){
   
  Memory::SharableMemoryPool::unique_ptr<long> ptrOnwer(
    env.ptrMemoryPool->allocate<long>(0), env.ptrMemoryPool->getDeleter());

  String strTest(strValue);
  
  for(int i=0;i<n;i++)
    strTest+=strTest;
  
  Memory::StreamBufferList sbl(env.ptrMemoryPool.get(),ptrOnwer.get());
  Memory::StreamBufferList::Writer writer(sbl);
  
  writer.write((const uint8_t*)strTest.c_str(), strTest.length());

  Memory::StreamBufferList::Reader reader(sbl);
  
  int i=0;

  while(reader.hasData()){
    IA20_LOG(true, "Reading: "<<reader.getLength()<<" bytes: "<<String((char*)reader.getData(),reader.getLength()));
    IA20_LOG(false, strTest.substr(i,reader.getLength()));
    
    if(memcmp(reader.getData(), strTest.substr(i,reader.getLength()).c_str(), reader.getLength()) != 0)
      IA20_THROW(BadUsageException("Reading from SBL failed at :")<<i<<"["<<strValue<<"]:"<<n);

    i += reader.getLength();
    reader.getNext();
  }

  if(i != strTest.length())
    IA20_THROW(BadUsageException("Reading from SBL failed, missing bytes :")
    <<i<<" != "<<strTest.length()<<"["<<strValue<<"]:"<<n);
    

}
/*************************************************************************/
void TCBasic::checkSBLCopy(const String& strValue, int n){
   
  Memory::SharableMemoryPool::unique_ptr<long> ptrOnwer(
    env.ptrMemoryPool->allocate<long>(0), env.ptrMemoryPool->getDeleter());

  String strTest(strValue);
  
  for(int i=0;i<n;i++)
    strTest+=strTest;
  
  Memory::StreamBufferList sbl(env.ptrMemoryPool.get(),ptrOnwer.get());
  Memory::StreamBufferList::Writer writer(sbl);
  
  writer.write((const uint8_t*)strTest.c_str(), strTest.length());

  Memory::StreamBufferList::Reader reader(sbl);
  

  std::unique_ptr<uint8_t> ptrBuf(new uint8_t[128000]);

	int i = reader.copy(ptrBuf.get(), strTest.length());


  if(i != strTest.length())
    IA20_THROW(BadUsageException("Reading from SBL failed, missing bytes :")
    <<i<<" != "<<strTest.length()<<"["<<strValue<<"]:"<<n);
    
  if(strncmp(strTest.c_str(), (const char*)ptrBuf.get(), strTest.length()) == 0)
    return;

  i = 0;
  
  while(i < strTest.length() && strTest[i] == ptrBuf.get()[i]){
    i++;
  }

  std::cerr<<String((const char*)ptrBuf.get(), strTest.length())<<std::endl;

  IA20_THROW(BadUsageException("Reading from SBL failed, bad content :")
    <<i<<" != "<<strTest.length()<<"["<<strValue<<"]:"<<n);
}
/*************************************************************************/
void TCBasic::caseStreamBufferList(){

	IA20::TimeSample ts(true);
  env.reset();

  checkSBL("1234567890", 10);
  checkSBL("12345", 3);
  checkSBL("abcdef", 13);
  checkSBL("123", 13);
  checkSBL("1234567890123", 13);
  
}
/*************************************************************************/
void TCBasic::caseStreamBufferCopy(){

	IA20::TimeSample ts(true);
  env.reset();

  checkSBLCopy("1234567890", 10);
   checkSBLCopy("12345", 3);
  checkSBLCopy("abcdef", 13);
  checkSBLCopy("123", 13);
  checkSBLCopy("1234567890123", 13);
  
}
/*************************************************************************/
struct TestObject{
  static int TheNumCalls;

  TestObject(int iValue = 666):iValue(iValue){
    TheNumCalls++;
    IA20_LOG(true,"TestObject N: "<<iValue);
  }

  ~TestObject(){
    IA20_LOG(true,"TestObject D: "<<iValue);
  }
  int iValue;
};

int TestObject::TheNumCalls = 0;

typedef Memory::FixedObjectsPool<TestObject, 4> TestObjectsPool;

void TCBasic::caseFixedObjectsPool(){

	IA20::TimeSample ts(true);
  env.reset();
    
  TestObjectsPool::unique_ptr ptr(TestObjectsPool::New());
  TestObjectsPool::unique_ptr ptr2(TestObjectsPool::New(2));

}
/*************************************************************************/
void TCBasic::TestEnv::reset(){
  
  ptrMemoryPool.reset(new Memory::SharableMemoryPool());

}
/*************************************************************************/
}
}
