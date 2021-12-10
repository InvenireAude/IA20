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

  Memory::MemoryManager::AssureCreated();

  unique_ptr<DM::Type> ptrIntegerType(new IntegerType("Integer"));
  unique_ptr<DM::Type> ptrStringType(new StringType("String"));

  unique_ptr<DM::ComplexType> ptrPartyType(new ComplexType("Party"));
  unique_ptr<DM::ComplexType> ptrPersonType(new ComplexType("Person",ptrPartyType.get()));
  unique_ptr<DM::ComplexType> ptrOrganizationType(new ComplexType("Organization",ptrPartyType.get()));

  ptrPartyType->defineProperty(ptrIntegerType.get(),"id");
  ptrOrganizationType->defineProperty(ptrStringType.get(),"name");
  ptrOrganizationType->defineProperty(ptrIntegerType.get(),"numberOfEmployees");
  ptrPersonType->defineProperty(ptrStringType.get(),"firstName");
  ptrPersonType->defineProperty(ptrStringType.get(),"lastName");
  ptrPersonType->defineProperty(ptrIntegerType.get(),"age");

  const ComplexType::PropertiesArray* tabPersonProperties(ptrPersonType->getProperties());

  cerr<<endl;
  for(auto p : *tabPersonProperties){
    cerr<<"Property ["<<p->getIdx()<<"]: "<<p->getName()<<", :"<<p->getType()->getName()<<endl;
  }
  cerr<<endl;


  DataObject *pPerson = ptrPersonType->create();
  cerr<<"***"<<endl;
  pPerson->createProperty(0);
  cerr<<"***"<<endl;
  pPerson->createProperty(1);
  cerr<<"***"<<endl;

  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;
  pPerson->getProperty("id")->setInteger(7777);
  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;
  cerr<<"stream: ";
  pPerson->getProperty("id")->saveToStream(cerr);
  cerr<<endl;
  pPerson->getProperty("id")->setString("9999");
  cerr<<"value: "<<pPerson->getProperty("id")->getInteger()<<endl;
  //pPerson->getProperty(1)->setString("John");
  pPerson->getProperty("firstName")->setString("John");
  cerr<<"firstName: "<<pPerson->getProperty("firstName")->getCString()<<endl;
}

/*************************************************************************/
}
}
