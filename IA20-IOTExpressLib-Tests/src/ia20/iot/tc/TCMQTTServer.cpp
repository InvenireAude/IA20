/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCMQTTServer.cpp
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


#include "TCMQTTServer.h"



#include <memory>
using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCMQTTServer::TCMQTTServer(TestSuite* pTestSuite):
  TestUnit<TCMQTTServer>(this, "MQTTServer", pTestSuite){
	IA20_TRACER;

  addCase("caseBasic", &::IA20::TC::TCMQTTServer::caseBasic);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCMQTTServer::~TCMQTTServer() throw(){
	IA20_TRACER;
}
/*************************************************************************/
static String CMSgCONNECT_Req("100C00044D5154540402003C0000");
/*************************************************************************/
void TCMQTTServer::caseBasic(){

	IA20::TimeSample ts(true);

  env.reset();

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(0, &cpuset);
  int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);


 //nv.ptrListener->start();

  String strTest(CMSgCONNECT_Req);

  for(int i=0; i<1; i++){
    //cerr<<i<<"\t"<<ts.getSample()<<endl;
    env.ptrListener->sendMessage(strTest);
    env.ptrEngine->serve();
    env.ptrListener->serve();
  }

  
  cerr<<ts.getSample()<<endl;
  cerr.flush();

  env.ptrListener->stop();
  env.ptrListener->join();
  

}
/*************************************************************************/
void TCMQTTServer::TestEnv::reset(){

  //  ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SYS::TasksRing<ActivityStore::Task>::CreateInterface(50)));
  //  ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SYS::TasksRing<ActionsStore::Task>::CreateInterface(50)));
  //  ptrListener.reset(new Mocker::Listener(Tools::SYS::TasksRing<Listener::Task>::CreateInterface(50)));

  //  ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SPIN::TasksRing<ActivityStore::Task>::CreateInterface(50)));
  //  ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SPIN::TasksRing<ActionsStore::Task>::CreateInterface(50)));

  ptrListener.reset(new Mocker::Listener(Tools::SPIN::TasksRing<Listener::Task>::CreateInterface(50)));
  
  ptrEngine.reset(new Engine(ptrListener.get()));
  


  // ptrActivityStore->start();
  // ptrActionsStore->start();
}
/*************************************************************************/
}
}
