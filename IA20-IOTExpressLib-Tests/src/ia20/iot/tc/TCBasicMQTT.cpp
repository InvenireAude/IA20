/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCBasicMQTT.cpp
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


#include "TCBasicMQTT.h"
#include <ia20/iot/tools/MQTT.h>

#include <memory>
using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCBasicMQTT::TCBasicMQTT(TestSuite* pTestSuite):
  TestUnit<TCBasicMQTT>(this, "BasicMQTT", pTestSuite){
	IA20_TRACER;

  addCase("caseLengthConverter", &::IA20::TC::TCBasicMQTT::caseLengthConverter);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCBasicMQTT::~TCBasicMQTT() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void testLVEncodeDecode(Tools::MQTT::VLIntType a){

  uint8_t buf[16];
  *(uint64_t*)buf = 0L;
  
  Tools::MQTT::VLIntType b;

  uint8_t* pEnd = Tools::MQTT::encodeVL(buf, a);
  b = Tools::MQTT::decodeVL(buf);

  IA20_LOG(true, a<<"="<<MiscTools::BinarytoHex(buf,4));

  if(a != b)
    IA20_THROW(BadUsageException("Error in testLVEncodeDecode: [")<<(int)a<<","<<(int)b<<"]");

  int iLen = pEnd - buf;

  if( (a < 128 && iLen != 1) ||
      (128 <= a && a < 16384 && iLen != 2) ||
      (16384 <= a && a < 2097152 && iLen != 3) ||
      (2097152 <= a && a < 268435456 && iLen != 4)){
            IA20_THROW(BadUsageException("Error in testLVEncodeDecode (length): [")<<(int)a<<","<<iLen<<"]");
      }

}
/*************************************************************************/
void TCBasicMQTT::caseLengthConverter(){

	IA20::TimeSample ts(true);

  testLVEncodeDecode(0);
  testLVEncodeDecode(1);
  testLVEncodeDecode(127);
  testLVEncodeDecode(128);
  testLVEncodeDecode(16383);
  testLVEncodeDecode(16384);
  testLVEncodeDecode(2097151);
  testLVEncodeDecode(2097152);
  testLVEncodeDecode(268435454);
  testLVEncodeDecode(268435455);
  
}
/*************************************************************************/
}
}
