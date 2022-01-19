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
#include <fcntl.h>
#include <unistd.h>


#include <memory>
using namespace std;


namespace IA20{
using namespace IOT;

namespace TC{
/*************************************************************************/
TCMQTTServer::TCMQTTServer(TestSuite* pTestSuite):
  TestUnit<TCMQTTServer>(this, "MQTTServer", pTestSuite){
	IA20_TRACER;

  addCase("caseBasicv31",           &::IA20::TC::TCMQTTServer::caseBasicv31);
  addCase("caseBasicv5",            &::IA20::TC::TCMQTTServer::caseBasicv5);
  addCase("caseBasicv31QoS1",       &::IA20::TC::TCMQTTServer::caseBasicv31QoS1);
  addCase("caseBasicv5QoS1",        &::IA20::TC::TCMQTTServer::caseBasicv5QoS1);
  addCase("caseBasicv5QoS1Retain",  &::IA20::TC::TCMQTTServer::caseBasicv5QoS1Retain);
  
  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCMQTTServer::~TCMQTTServer() throw(){
	IA20_TRACER;
}
/*************************************************************************/

std::initializer_list< std::pair<int, String> > CaseMQv31{
    { 0, "100C00044D5154540402003C0000" },
    { 0, "821B000100042F6162630000042F78797A0000082F6162632F78797A00" },
    { 0, "301100082F6162632F78797A00014142434446" },
    { 1, "100C00044D5154540402003C0000" },
    { 1, "821B000100042F6162630000042F78797A0000082F6162632F78797A00" },    
    { 0, "301100082F6162632F78797A00024142434447" }
};

std::initializer_list< std::pair<int, String> > CaseMQv5{
    { 0, "101000044D5154540502003C032100140000" },
    { 0, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },
    { 0, "301200082F6162632F78797A0001004142434446" },
    { 1, "101000044D5154540502003C032100140000" },
    { 1, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },    
    { 0, "301200082F6162632F78797A0002004142434447" }
};

std::initializer_list< std::pair<int, String> > CaseMQv31QoS1{
    { 0, "100C00044D5154540402003C0000" },
    { 0, "821B000100042F6162630000042F78797A0000082F6162632F78797A00" },
    { 0, "321100082F6162632F78797A00014142434446" },
    { 0, "4003000100" },
    { 1, "100C00044D5154540402003C0000" },
    { 1, "821B000100042F6162630000042F78797A0000082F6162632F78797A00" },    
    { 0, "321100082F6162632F78797A00024142434447" },
    { 0, "4003000200" },
    { 1, "4003000200" },
};

std::initializer_list< std::pair<int, String> > CaseMQv5QoS1{
    { 0, "101000044D5154540502003C032100140000" },
    { 0, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },
    { 0, "321200082F6162632F78797A0001004142434446" },
    { 0, "4003000100" },
    { 1, "101000044D5154540502003C032100140000" },
    { 1, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },    
    { 0, "321200082F6162632F78797A0002004142434447" },
    { 0, "4003000200" },
    { 1, "4003000200" }
};

std::initializer_list< std::pair<int, String> > CaseMQv5QoS1Retain{
    { 0, "101000044D5154540502003C032100140000" },
    { 0, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },
    { 0, "331200082F6162632F78797A0001004142434446" },
    { 0, "4003000100" },
    { 1, "101000044D5154540502003C032100140000" },
    { 1, "821C00010000042F6162630000042F78797A0000082F6162632F78797A00" },    
    { 0, "321200082F6162632F78797A0002004142434447" },
    { 0, "4003000200" },
    { 1, "4003000200" }
};

// static String CMSgCONNECT_Req ("101000044D5154540502003C032100140000");
// static String CMSgSUBSCRIBE_Req("821C00010000042F6162630000042F78797A0000082F6162632F78797A00");
// static String CMSgPUBLISH1_Req("321200082F6162632F78797A0001004142434446");
// static String CMSgPUBLISH2_Req("321200082F6162632F78797A0002004142434447");                                
                              //321100082F6162632F78797A0001  4142434446
/*************************************************************************/
 void TCMQTTServer::caseBasicImpl(const std::initializer_list<std::pair<int, String> >& lstArgs){

  env.reset(); // TODO compare results

	IA20::TimeSample ts(true);

   for(const auto& s: lstArgs){

    IA20_LOG(true, "*** Iterate 1 ***");

    env.ptrListener->sendMessage(s.second, s.first);
    
    env.ptrListener->getRingHandler()->handle();
    //env.ptrEngine->getPort()->schedule();
    env.ptrEngine->getRingHandler()->handle();

    IA20_LOG(true, "*** Engine ***");

    env.ptrEngine->serve();

    IA20_LOG(true, "*** Responses ***");

    //env.ptrEngine->getPort()->flush();
    env.ptrEngine->getRingHandler()->handle();

    env.ptrListener->getPort()->schedule();
    env.ptrListener->getRingHandler()->handle();

    env.ptrListener->serveUntilEmptyQueue();
   }

   env.ptrListener->dump(std::cerr); // TODO compare results
   env.ptrListener->getConnectionStates(); // TODO compare results
 }
/*************************************************************************/
void TCMQTTServer::caseBasicv31(){
  IA20_LOG(true,"caseBasicv31");
  caseBasicImpl(CaseMQv31);
}
/*************************************************************************/
void TCMQTTServer::caseBasicv5(){
  IA20_LOG(true,"caseBasicv5");
  caseBasicImpl(CaseMQv5);
}
/*************************************************************************/
void TCMQTTServer::caseBasicv31QoS1(){
  IA20_LOG(true,"caseBasicv31QoS1");
  caseBasicImpl(CaseMQv31QoS1);
}
/*************************************************************************/
void TCMQTTServer::caseBasicv5QoS1(){
  IA20_LOG(true,"caseBasicv5QoS1");
  caseBasicImpl(CaseMQv5QoS1);
}
/*************************************************************************/
void TCMQTTServer::caseBasicv5QoS1Retain(){
  IA20_LOG(true,"caseBasicv5QoS1Retain");
  caseBasicImpl(CaseMQv5QoS1Retain);
}
/*************************************************************************/
// void TCMQTTServer::caseBasic(){

// 	IA20::TimeSample ts(true);

//   env.reset();

//     cpu_set_t cpuset;
//     CPU_ZERO(&cpuset);
//     CPU_SET(0, &cpuset);
//     int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);

//     //nv.ptrListener->start();
//     //String strTest(CMSgCONNECT_Req);

//     //for(int i=0; i<1; i++){
//     //cerr<<i<<"\t"<<ts.getSample()<<endl;

//     env.ptrListener->sendMessage(CMSgCONNECT_Req);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();

//     env.ptrListener->sendMessage(CMSgSUBSCRIBE_Req);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();

//     env.ptrListener->sendMessage(CMSgPUBLISH1_Req);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();

//     env.ptrListener->sendMessage(CMSgCONNECT_Req,1);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();

//     env.ptrListener->sendMessage(CMSgSUBSCRIBE_Req,1);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();

//     env.ptrListener->sendMessage(CMSgPUBLISH2_Req);
//     env.ptrEngine->serve();
//     env.ptrListener->serveUntilEmptyQueue();
 

//   //}


//   cerr<<ts.getSample()<<endl;
//   cerr.flush();

//   env.ptrListener->stop();
//   env.ptrListener->join();


// }
/*************************************************************************/
void TCMQTTServer::TestEnv::reset(){

  //  ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SYS::TasksRing<ActivityStore::Task>::CreateInterface(50)));
  //  ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SYS::TasksRing<ActionsStore::Task>::CreateInterface(50)));
  //  ptrListener.reset(new Mocker::Listener(Tools::SYS::TasksRing<Listener::Task>::CreateInterface(50)));

  //  ptrActivityStore.reset(new Mocker::ActivityStore(Tools::SPIN::TasksRing<ActivityStore::Task>::CreateInterface(50)));
  //  ptrActionsStore.reset(new Mocker::ActionsStore(Tools::SPIN::TasksRing<ActionsStore::Task>::CreateInterface(50)));

   int fdRequests[2];
   int fdResponses[2];

   int flags = 0;// | O_NONBLOCK;

   pipe2(fdRequests,  flags);
   pipe2(fdResponses,  flags);

   
   ptrListener.reset(new Mocker::Listener(fdResponses[0], fdRequests[1], 2));

   ptrEngine.reset(new Engine());

   ptrEngine->addListener(ptrListener.get(), fdRequests[0], fdResponses[1]);

}
/*************************************************************************/
}
}
