/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCDataObject.cpp
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


#include "TCDataObject.h"


#include <ia20/dm/dm.h>

#include <memory>
using namespace std;


namespace IA20{
using namespace DM;

namespace TC{
/*************************************************************************/
TCDataObject::TCDataObject(TestSuite* pTestSuite):
  TestUnit<TCDataObject>(this, "DataObject", pTestSuite){
	IA20_TRACER;

  addCase("caseBasic", &::IA20::TC::TCDataObject::caseBasic);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCDataObject::~TCDataObject() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void TCDataObject::caseBasic(){

	IA20::TimeSample ts(true);

  DM::DataFactory::GetDefault();

  unique_ptr<DM::DataFactory> ptrDataFactory(new DataFactory());

  const String& CNamespace("http://www.invenieaude.org");

  ptrDataFactory->createNamespace(CNamespace);

  DM::ComplexType* pPartyType = ptrDataFactory->createType(CNamespace, Type::CDataObjectType, "Party")->asComplexType();
  DM::ComplexType* pPersonType = ptrDataFactory->createType(CNamespace, Type::CDataObjectType, "Person", pPartyType)->asComplexType();
  DM::ComplexType* pOrganizationType = ptrDataFactory->createType(CNamespace, Type::CDataObjectType, "Organization", pPartyType)->asComplexType();

  const DM::Type* pIntegerType = ptrDataFactory->getType(DM::Namespace::CDefaultName, DM::IntegerType::CDefaultName);
  const DM::Type* pStringType  = ptrDataFactory->getType(DM::Namespace::CDefaultName, DM::StringType::CDefaultName);

  pPartyType->defineProperty(pIntegerType,"id");
  pOrganizationType->defineProperty(pStringType,"name");
  pOrganizationType->defineProperty(pIntegerType,"numberOfEmployees");
  pPersonType->defineProperty(pStringType,"firstName");
  pPersonType->defineProperty(pStringType,"lastName");
  pPersonType->defineProperty(pIntegerType,"age");

  Memory::MemoryManager::AssureCreated(true);
  const ComplexType::PropertiesArray* tabPersonProperties(pPersonType->getProperties());

  cerr<<endl;
   for(auto p : *tabPersonProperties){
      cerr<<"Property ["<<p->getIdx()<<"]: "<<p->getName()<<", :"<<p->getType()->getName()<<endl;
  }
  cerr<<endl;


  for(int j=0; j<10000000; j++){
      DataObject *pPerson1 = pPersonType->create();
      pPerson1->createProperty(0);
      pPerson1->createProperty(1);
      pPerson1->createProperty(2);
      pPerson1->getProperty(0)->setInteger(123456789);
      pPerson1->getProperty(1)->setString("John");
      pPerson1->getProperty(2)->setString("Doe");
      Memory::MemoryManager::Free(pPerson1);
  }


  DataObject *pPerson = pPersonType->create();

  pPerson->createProperty(0);
  pPerson->createProperty(1);
  pPerson->createProperty(2);

  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;
  pPerson->getProperty("id")->setInteger(7777);
  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;
  cerr<<"stream: ";
  pPerson->getProperty("id")->saveToStream(cerr);
  cerr<<endl;
  pPerson->getProperty("id")->setString("9999");
  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;


  for(int j=0; j<20; j++){
    pPerson->getProperty(0)->setInteger(j);
    pPerson->getProperty(1)->setString("John");
    pPerson->getProperty(2)->setString("Doe");
  }


  pPerson->getProperty("firstName")->setString("John");
  cerr<<"firstName: "<<pPerson->getProperty("firstName")->getCString()<<endl;

  Memory::MemoryManager::Free(pPerson);
  Memory::MemoryManager::AssureCreated(true);

  cerr<<ts.getSample()<<endl;

}

/*************************************************************************/
}
}
