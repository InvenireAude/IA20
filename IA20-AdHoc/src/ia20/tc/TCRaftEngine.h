/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCRaftEngine.h
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
#ifndef _IA20_TC_TCRaftEngine_H_
#define _IA20_TC_TCRaftEngine_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include <ia20/net/engine/raft/mocker/Connection.h>
#include <ia20/net/engine/raft/mocker/Logger.h>
#include <ia20/net/engine/raft/RaftEngine.h>


namespace IA20{
namespace TC{


class TCRaftEngine: public TestUnit{

public:
	TCRaftEngine(TestSuite* pTestSuite);
	virtual ~TCRaftEngine()  throw ();

	virtual void init(TestSuite* pTestSuite);

	void case01();

protected:

  struct Setup {

    typedef std::unique_ptr<Net::Engine::Raft::RaftEngine>     EnginePtr;
    typedef std::unique_ptr<Net::Engine::Raft::Mocker::Logger> LoggerPtr;

    typedef std::vector<EnginePtr> EngineTab;
    typedef std::vector<LoggerPtr> LoggerTab;

    EngineTab tabEngines;
    LoggerTab tabLoggers;

    std::unique_ptr<Net::Engine::Raft::Mocker::Connection> ptrConnection;

    Setup(int iNumEngines);

    void assetLogger(int iLogger, const String& strResult);

  };

	::IA20::TestRunner<TCRaftEngine> theTestRunner;
};


}
}


#endif /*_IA20_TC_TCRaftEngine_H_*/
