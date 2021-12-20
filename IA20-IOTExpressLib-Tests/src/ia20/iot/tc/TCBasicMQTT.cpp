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



#include <memory>
using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCBasicMQTT::TCBasicMQTT(TestSuite* pTestSuite):
  TestUnit<TCBasicMQTT>(this, "BasicMQTT", pTestSuite){
	IA20_TRACER;

  addCase("caseBasic", &::IA20::TC::TCBasicMQTT::caseBasic);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCBasicMQTT::~TCBasicMQTT() throw(){
	IA20_TRACER;
}
/*************************************************************************/
static String CMSgCONNECT_Req("100C00044D5154540402003C0000");
/*************************************************************************/
void TCBasicMQTT::caseBasic(){

	IA20::TimeSample ts(true);

  env.reset();

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);


  for(int i=0; i<1000000; i++){
    //cerr<<i<<"\t"<<ts.getSample()<<endl;
    env.ptrListener->sendMessage(CMSgCONNECT_Req);
    env.ptrEngine->serveListener();
    env.ptrEngine->serveActivityStore();
    env.ptrEngine->serveActionsStore();
  }

  cerr<<ts.getSample()<<endl;
  cerr.flush();

  env.ptrListener->stop();
  env.ptrActivityStore->stop();
  env.ptrActionsStore->stop();

  env.ptrListener->join();
  env.ptrActivityStore->join();
  env.ptrActionsStore->join();


}
/*************************************************************************/
void TCBasicMQTT::TestEnv::reset(){

  //  ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SYS::TasksRing<ActivityStore::Task>::CreateInterface(50)));
  //  ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SYS::TasksRing<ActionsStore::Task>::CreateInterface(50)));
  //  ptrListener.reset(new Mocker::Listener(Tools::SYS::TasksRing<Listener::Task>::CreateInterface(50)));

   ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SPIN::TasksRing<ActivityStore::Task>::CreateInterface(50)));
   ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SPIN::TasksRing<ActionsStore::Task>::CreateInterface(50)));
   ptrListener.reset(new Mocker::Listener(Tools::SPIN::TasksRing<Listener::Task>::CreateInterface(50)));

  ptrEngine.reset(new Engine(ptrListener->getInterface(),
              ptrActivityStore->getInterface(),
              ptrActionsStore->getInterface()));

  ptrListener->start();
  ptrActivityStore->start();
  ptrActionsStore->start();
}
/*************************************************************************/
}
}
