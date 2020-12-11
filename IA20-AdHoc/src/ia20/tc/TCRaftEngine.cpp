/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCRaftEngine.cpp
 *
 * Copyright (C) 2015, Albert Krzymowski
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


#include "TCRaftEngine.h"

#include <ia20/net/engine/raft/unix/PacketFactory.h>
using namespace Net::Engine::Raft;

namespace IA20{
namespace TC{

/*************************************************************************/
TCRaftEngine::Setup::Setup(int iNumEngines):
  ptrConnection(new Net::Engine::Raft::Mocker::Connection()),
  tabLoggers(iNumEngines),
  tabEngines(iNumEngines){
  for(int i=0; i<iNumEngines; i++){
      tabLoggers[i].reset(new Net::Engine::Raft::Mocker::Logger());
      tabEngines[i].reset(new Net::Engine::Raft::RaftEngine(i+1, iNumEngines, tabLoggers[i].get(), ptrConnection.get()));
      ptrConnection->add(tabEngines[i].get());
    };

};
/*************************************************************************/
void TCRaftEngine::Setup::assetLogger(int iLogger, const String& strResult){

  StringStream ssLogger;
  tabLoggers[iLogger]->simpleDump(ssLogger);

  if(ssLogger.str().compare(strResult) != 0){
    IA20_LOG(LogLevel::INSTANCE.isError(), "Got:      "<<ssLogger.str());
    IA20_LOG(LogLevel::INSTANCE.isError(), "Expected: "<<strResult);
    IA20_THROW(BadUsageException("Logger output does not match."));
  };

}
/*************************************************************************/
TCRaftEngine::TCRaftEngine(TestSuite* pTestSuite):
  TestUnit<TCRaftEngine>(this, "RaftEngine", pTestSuite){
	IA20_TRACER;

	addCase("simpleFailure", &::IA20::TC::TCRaftEngine::caseSimpleFailure);
	addCase("newElection", &::IA20::TC::TCRaftEngine::caseNewElection);

  pTestSuite->addTestUnit(this);

  PacketFactory::SetInstance(new  Unix::PacketFactory());
}
/*************************************************************************/
TCRaftEngine::~TCRaftEngine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void TCRaftEngine::caseSimpleFailure(){
	IA20_TRACER;

	IA20::TimeSample ts(true);

  Setup s(5);

  s.tabEngines[0]->startElection();

  RaftEngine *pLeaderEngine = s.tabEngines[0].get();

  s.ptrConnection->disable(s.tabEngines[3].get());

  pLeaderEngine->onData((void*)"ABC",3);

  s.ptrConnection->enable(s.tabEngines[3].get());

  pLeaderEngine->onData((void*)"XYZ",3);


  if(getTestSuite()->isVerbose())
    for(int i = 0; i < s.tabLoggers.size(); i++){
      Mocker::Logger *pLogger = s.tabLoggers[i].get();
      std::cout<<"Logger["<<i<<"]=";
      pLogger->simpleDump(std::cout);
      std::cout<<std::endl;
    }

  s.assetLogger(0,"[0,0][1,1][1,2]{ABC}[1,3]{XYZ}*");
  s.assetLogger(3,"[0,0][1,1][1,2]{ABC}[1,3]{XYZ}");
}
/*************************************************************************/
void TCRaftEngine::caseNewElection(){
	IA20_TRACER;

	IA20::TimeSample ts(true);

  Setup s(5);

  s.tabEngines[0].get()->startElection();

  RaftEngine *pLeaderEngine0 = s.tabEngines[0].get();

  pLeaderEngine0->onData((void*)"ABC",3);

  s.ptrConnection->disable(s.tabEngines[3].get());

  pLeaderEngine0->onData((void*)"XYZ",3);

  s.tabEngines[1].get()->startElection();

  RaftEngine *pLeaderEngine1 = s.tabEngines[1].get();

  pLeaderEngine1->onData((void*)"abc",3);

  s.ptrConnection->enable(s.tabEngines[3].get());

  pLeaderEngine1->onData((void*)"xyz",3);

  pLeaderEngine0->onData((void*)"666",3);


  if(getTestSuite()->isVerbose())
    for(int i = 0; i < s.tabLoggers.size(); i++){
      Mocker::Logger *pLogger = s.tabLoggers[i].get();
      std::cout<<"Logger["<<i<<"]=";
      pLogger->simpleDump(std::cout);
      std::cout<<std::endl;
    }

  s.assetLogger(0,"[0,0][1,1][1,2]{ABC}[1,3]{XYZ}[2,1][2,2]{abc}[2,3]{xyz}*");
  s.assetLogger(1,"[0,0][1,1][1,2]{ABC}[1,3]{XYZ}[2,1][2,2]{abc}[2,3]{xyz}*");
  s.assetLogger(3,"[0,0][1,1][1,2]{ABC}[1,3]{XYZ}[2,1][2,2]{abc}[2,3]{xyz}");
}
/*************************************************************************/
}
}
