/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCSubscriptions.cpp
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


#include "TCSubscriptions.h"

#include <memory>
#include <string.h>

using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCSubscriptions::TCSubscriptions(TestSuite* pTestSuite):
  TestUnit<TCSubscriptions>(this, "Subscription", pTestSuite){
	IA20_TRACER;

  addCase("caseSimple", &::IA20::TC::TCSubscriptions::caseSimple);
  addCase("caseHash01", &::IA20::TC::TCSubscriptions::caseHash01);
  addCase("caseHash02", &::IA20::TC::TCSubscriptions::caseHash02);
  addCase("casePlus01", &::IA20::TC::TCSubscriptions::casePlus01);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCSubscriptions::~TCSubscriptions() throw(){
	IA20_TRACER;
}
/*************************************************************************/
std::initializer_list< std::pair<int, String> > Set01{
    { 0, "/abc" },
    { 0, "/abc/xyz" },
    { 1, "/abc/xyz" },
};

std::list< std::pair<int, String> > ResultSet01  {
    { 0, "/abc/xyz" },
    { 1, "/abc/xyz" },
};
/*************************************************************************/
std::initializer_list< std::pair<int, String> > SetHash01{
    { 0, "/abc" },
    { 0, "/abc/xyz" },
    { 1, "/abc/xyz" },
    { 1, "/abc/#" },
};
std::list< std::pair<int, String> > ResultSetHash01  {
    { 0, "/abc/xyz" },
    { 1, "/abc/#" },
    { 1, "/abc/xyz" }
};
/*************************************************************************/
std::initializer_list< std::pair<int, String> > SetHash02{
    { 0, "/abc" },
    { 0, "/abc/xyz" },
    { 1, "/abc/xyz" },
    { 1, "/#" },
    { 0, "/abc/#" }
};
std::list< std::pair<int, String> > ResultSetHash02{
    { 0, "/abc/#" },
    { 0, "/abc/xyz" },
    { 1, "/#" },
    { 1, "/abc/xyz" }
};
/*************************************************************************/
std::initializer_list< std::pair<int, String> > SetPlus01{
    { 0, "/abc" },
    { 0, "/+/xyz" },
    { 0, "/opr/xyz" },
    { 1, "/abc/+" },
    { 1, "/abc/def" },
    { 2, "/abc/+/ghj" },
};
std::list< std::pair<int, String> > ResultSetPlus01{
    { 0, "/+/xyz" },
    { 1, "/abc/+" },
};
/*************************************************************************/
class Callback : public Topic::Callback{
  public:
  virtual void onSubscription(const Subscription* pSubscription);

  std::list< std::pair<int, String> > lstResult;
};
/*************************************************************************/
void Callback::onSubscription(const Subscription* pSubscription){
  std::cerr<<"Callback: "<<pSubscription->getTopic()<<"\t"<<pSubscription->getConnectionHandle()<<std::endl;
  lstResult.push_back(std::pair<int, String>(
      pSubscription->getConnectionHandle(), 
      pSubscription->getTopic()) );
}
/*************************************************************************/
void TCSubscriptions::checkSubscriptions(
    const std::list< std::pair<int, String> >& lstSetup,
    const String& strPubTopic,
    const std::list< std::pair<int, String> >& lstResult){

	IA20::TimeSample ts(true);
  env.reset();

  for(const auto& e : lstSetup){
    Tools::StringRef strTopic(e.second);
    Topic* pTopic = env.ptrTopicsStore->getOrCreateTopic(strTopic);
      env.ptrSubscriptionsStore->addSubscription(e.first, pTopic, strTopic, 0);
  }

  Tools::StringRef strTopic(strPubTopic);
  std::unique_ptr<Callback> ptrCallback(new Callback());

  env.ptrTopicsStore->iterate(strTopic, ptrCallback.get());
  ptrCallback->lstResult.sort();

  if(ptrCallback->lstResult != lstResult){
    for(const auto& e: ptrCallback->lstResult){
       std::cerr<<"Got: "<<e.second<<"\t"<<e.first<<std::endl;
    }
    IA20_THROW(BadUsageException("checkSubscriptions:: result mismatch !!!"));
  }

}
/*************************************************************************/
void TCSubscriptions::caseSimple(){
  checkSubscriptions(Set01, "/abc/xyz", ResultSet01);
}
/*************************************************************************/
void TCSubscriptions::caseHash01(){
  checkSubscriptions(SetHash01, "/abc/xyz", ResultSetHash01);
}
/*************************************************************************/
void TCSubscriptions::caseHash02(){
  checkSubscriptions(SetHash02, "/abc/xyz", ResultSetHash02);
}
/*************************************************************************/
void TCSubscriptions::casePlus01(){
  checkSubscriptions(SetPlus01, "/abc/xyz", ResultSetPlus01);
}
/*************************************************************************/
void TCSubscriptions::TestEnv::reset(){
  
  ptrTopicsStore.reset(new IOT::TopicsStore());
  ptrSubscriptionsStore.reset(new IOT::SubscriptionsStore());

}
/*************************************************************************/
}
}
