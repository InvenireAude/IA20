/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCConnectionTCP.cpp
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


#include "TCConnectionTCP.h"

#include <ia20/commonlib/net/conn/tcp/DefaultConnectionFactory.h>
#include <unistd.h>
#include <string.h>

namespace IA20{
namespace TC{

using namespace Net;
using namespace Net::Conn;
using namespace Net::Conn::TCP;

static Barrier barrier(2,1000);

/*************************************************************************/
TCConnectionTCP::Env::Env(const String& strAddress,
          int iPort,
          Net::Conn::TCP::ConnectionFactory* pServerConnectionFactory,
          Net::Conn::TCP::ConnectionFactory* pClientConnectionFactory):
  peer(strAddress, iPort),
  ptrServer(new Server(peer, pServerConnectionFactory)),
  ptrClient(new Client(peer, pClientConnectionFactory)),
  Thread(this){

};
/*************************************************************************/
void TCConnectionTCP::Env::run(){
  IA20_TRACER;
  std::unique_ptr<FileHandle> ptrConnection(ptrServer->accept());

  Thread::Cancellation ca(false);

  ptrConnection->write("ABC",3,iBytes);

  if(iBytes != 3)
    ptrException.reset(new BadUsageException("'ABC' text test failure"));

  ptrConnection->read(buffer,CBufSize,iBytes);

  if(iBytes != 10 || memcmp(buffer,"1234567890",10) != 0)
    ptrException.reset(new BadUsageException("'1234567890' text test failure"));

  barrier.synchronize();
  IA20_LOG(true,"DDDone");
}
/*************************************************************************/
TCConnectionTCP::TCConnectionTCP(TestSuite* pTestSuite):
  TestUnit<TCConnectionTCP>(this, "ConnectionTCP", pTestSuite){
	IA20_TRACER;

  addCase("connect", &::IA20::TC::TCConnectionTCP::caseConnect);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCConnectionTCP::~TCConnectionTCP() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void TCConnectionTCP::caseConnect(){

	IA20::TimeSample ts(true);

  Env e("localhost",55555, DefaultConnectionFactory::GetInstance(), DefaultConnectionFactory::GetInstance());
  e.start();

  std::unique_ptr<FileHandle> ptrConnection(e.ptrClient->connect());

  uint8_t buffer[Env::CBufSize];
  size_t iBytes;

  ptrConnection->read(buffer,3,iBytes);

  if(iBytes != 3 || memcmp(buffer,"ABC",3) != 0)
    IA20_THROW(BadUsageException("'ABC' text test failure"));

  ptrConnection->write("1234567890",10,iBytes);
  barrier.synchronize();
  e.stop();
  e.join();

  if(e.ptrException)
    throw *(e.ptrException.release());
}
/*************************************************************************/
}
}
