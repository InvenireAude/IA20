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
#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/mqtt/FixedHeaderBuilder.h>
#include <ia20/iot/mqtt/HeaderReader.h>

#include <memory>
using namespace std;


namespace IA20{
using namespace IOT;
using namespace IOT::MQTT;
namespace TC{
/*************************************************************************/
TCBasicMQTT::TCBasicMQTT(TestSuite* pTestSuite):
  TestUnit<TCBasicMQTT>(this, "BasicMQTT", pTestSuite){
	IA20_TRACER;

  addCase("caseLengthConverter",   &::IA20::TC::TCBasicMQTT::caseLengthConverter);
  addCase("caseFixedHeaderBuild",  &::IA20::TC::TCBasicMQTT::caseFixedHeaderBuild);
  addCase("caseFixedHeaderReader", &::IA20::TC::TCBasicMQTT::caseFixedHeaderReader);

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
void testFixedHeaderBuilder(
  Message::Type iType,
  Message::Flag iFlags,
  uint8_t       iQoS,
  uint32_t iLength,
  uint16_t iID,
  const String& strExpected){

  MQTT::FixedHeaderBuilder builder;

  builder.setType(iType);
  builder.setFlags(iFlags,iQoS);
  builder.setID(iID);
  builder.setLength(iLength);

  uint8_t buf[MQTT::FixedHeaderBuilder::CMaxLen];
  uint8_t *pEnd = builder.build(buf);

  String strResult = MiscTools::BinarytoHex(buf, pEnd - buf);

  if(strResult.compare(strExpected) != 0)
    IA20_THROW(BadUsageException("Bad FixedHeader bulid, expected: ")
        <<strExpected<<", got:"<<strResult);

}
/*************************************************************************/
void TCBasicMQTT::caseFixedHeaderBuild(){

	IA20::TimeSample ts(true);
  
  testFixedHeaderBuilder(Message::Type::MT_CONNECT, 
                         Message::Flag::MF_NONE, 0,
                         4, 10, "1004");

  testFixedHeaderBuilder(Message::Type::MT_PUBLISH, 
                         Message::Flag::MF_NONE, 0,
                         99999, 99, "309F8D06");
  
  testFixedHeaderBuilder(Message::Type::MT_PUBLISH, 
                         Message::Flag::MF_NONE, 2,
                         99999, 99, "349F8D066300");

  testFixedHeaderBuilder(Message::Type::MT_SUBSCRIBE, 
                         Message::Flag::MF_NONE, 0,
                         666, 99, "809A056300");

  testFixedHeaderBuilder(Message::Type::MT_PINGREQ, 
                         Message::Flag::MF_NONE, 0,
                         666, 99, "C09A05");
}
/*************************************************************************/
void testFixedHeaderReader(
  Message::Type iType,
  Message::Flag iFlags,
  uint8_t       iQoS,
  uint32_t iLength,
  uint16_t iID,
  const String& strWireData){
  
  uint8_t buf[MQTT::FixedHeaderBuilder::CMaxLen];

  MiscTools::HexToBinary(strWireData, buf, MQTT::FixedHeaderBuilder::CMaxLen);

  HeaderReader hr(buf);

  if(hr.getType() != iType)
    IA20_THROW(BadUsageException("Bad FixedHeader bulid, expected type: ")
        <<iType<<", got:"<<hr.getType()<<" in: "<<strWireData);

  if(hr.getLength() != iLength)
    IA20_THROW(BadUsageException("Bad FixedHeader bulid, expected length: ")
        <<(int)iLength<<", got:"<<(int)hr.getLength()<<" in: "<<strWireData);

  if(hr.getQoS() != iQoS)
    IA20_THROW(BadUsageException("Bad FixedHeader bulid, expected QOS: ")
        <<iQoS<<", got:"<<hr.getQoS()<<" in: "<<strWireData);


	if( iType >= Message::MT_PUBACK && iType <= Message::MT_UNSUBACK ||
			iType == Message::MT_PUBLISH && iQoS >= 1){
	
  if(hr.getID() != iID)
    IA20_THROW(BadUsageException("Bad FixedHeader bulid, expected QOS: ")
        <<iID<<", got:"<<hr.getID()<<" in: "<<strWireData);
      }

}
/*************************************************************************/
void TCBasicMQTT::caseFixedHeaderReader(){
  
  testFixedHeaderReader(Message::Type::MT_CONNECT, 
                         Message::Flag::MF_NONE, 0,
                         4, 10, "1004");

  testFixedHeaderReader(Message::Type::MT_PUBLISH, 
                         Message::Flag::MF_NONE, 0,
                         99999, 99, "309F8D06");
  
  testFixedHeaderReader(Message::Type::MT_PUBLISH, 
                         Message::Flag::MF_NONE, 2,
                         99999, 99, "349F8D066300");

  testFixedHeaderReader(Message::Type::MT_SUBSCRIBE, 
                         Message::Flag::MF_NONE, 0,
                         666, 99, "809A056300");

  testFixedHeaderReader(Message::Type::MT_PINGREQ, 
                         Message::Flag::MF_NONE, 0,
                         666, 99, "C09A05");
}
/*************************************************************************/
}
}
