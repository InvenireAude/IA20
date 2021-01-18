/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCConnectionTCP.h
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
#ifndef _IA20_TC_TCConnectionTCP_H_
#define _IA20_TC_TCConnectionTCP_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/testcase/TestUnit.h>

#include <ia20/net/conn/tcp/Server.h>
#include <ia20/net/conn/tcp/Client.h>

#include <ia20/net/conn/tcp/Peer.h>

namespace IA20{
namespace TC{


class TCConnectionTCP:
  public TestUnit<TCConnectionTCP>{

public:

	TCConnectionTCP(TestSuite* pTestSuite);
	virtual ~TCConnectionTCP()  throw ();


	void caseConnect();

protected:

  struct Env : public Thread, public Runnable{

    Net::Conn::TCP::Peer peer;

    std::unique_ptr<Net::Conn::TCP::Server>   ptrServer;
    std::unique_ptr<Net::Conn::TCP::Client>   ptrClient;

    Env(const String& strAddress, int iPort,
        Net::Conn::TCP::ConnectionFactory* pServerConnectionFactory,
        Net::Conn::TCP::ConnectionFactory* pClientConnectionFactory);

    virtual void run();

    static const int CBufSize = 128;

    uint8_t buffer[CBufSize];
    size_t iBytes;

    std::unique_ptr<Exception> ptrException;
  };


};


}
}


#endif /*_IA20_TC_TCConnectionTCP_H_*/
