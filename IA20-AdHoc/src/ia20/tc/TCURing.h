/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCURing.h
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
#ifndef _IA20_TC_TCURing_H_
#define _IA20_TC_TCURing_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>


#include <ia20/uring/RingHandler.h>
#include <ia20/uring/IO/TCP/AsyncServer.h>
#include <ia20/uring/IO/ReadHandler.h>

#include <ia20/commonlib/net/conn/tcp/Peer.h>

namespace IA20{
namespace TC{


class TCURing:
  public TestUnit<TCURing>{

public:

	TCURing(TestSuite* pTestSuite);
	virtual ~TCURing()  throw ();


	void caseConnect();

protected:

  struct Env : public Thread, public Runnable{

    Net::Conn::TCP::Peer peer;

    std::unique_ptr<URing::RingHandler>            ptrRingHandler;
    std::unique_ptr<Net::Conn::TCP::FileHandle>    ptrFileHandle;


    std::unique_ptr<URing::IO::TCP::AsyncServer::Acceptor> ptrAcceptor;

    Env(const String& strAddress, int iPort);

    virtual void run();


    bool bResult;
  };



};


}
}


#endif /*_IA20_TC_TCURing_H_*/
