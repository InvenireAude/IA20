/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCURing.cpp
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


#include "TCURing.h"

#include <ia20/commonlib/net/conn/tcp/DefaultConnectionFactory.h>
#include <ia20/commonlib/net/conn/tcp/Client.h>
#include <ia20/commonlib/net/conn/tcp/Server.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <ia20/commonlib/uring/uring.h>

#include <unistd.h>
#include <string.h>

namespace IA20{
namespace TC{

using namespace URing;
using namespace URing::IO;

using namespace Net;
using namespace Net::Conn;
using namespace Net::Conn::TCP;

static int CNumClients = 2;
static Barrier barrierStart(2,1000);
static Barrier barrierWork(2 + CNumClients, 1000);
/*************************************************************************/
TCURing::Env::Env(const String& strAddress,
          int iPort):
  peer(strAddress, iPort),
  Thread(this){

  ptrRingHandler.reset(new RingHandler);

  bResult = true;
};

/*************************************************************************/
class Server : public ReadHandler, public WriteHandler {
  public:
    Server(RingHandler *pRingHandler, FileHandle* pFileHandle):
      ReadHandler(pRingHandler, pFileHandle),
      WriteHandler(pRingHandler, pFileHandle),
      ptrFileHandle(pFileHandle){
        WriteHandler::iovec.iov_len  = CStrData.length();
        WriteHandler::iovec.iov_base = malloc(4096);

        memcpy(WriteHandler::iovec.iov_base, CStrData.c_str(), WriteHandler::iovec.iov_len);

          ReadHandler::iovec.iov_base = malloc(4096);
          ReadHandler::iovec.iov_len  = 4096;

      };

    ~Server(){
      free(WriteHandler::iovec.iov_base);
      free(ReadHandler::iovec.iov_base);
    }
    
  static const String CStrData;

  protected:

  virtual void handleRead(off_t iDataLen){
      std::cout<<"Got data: ["<<iDataLen<<"]"<<std::endl;
      barrierWork.synchronize();
  }

  virtual void handleWrite(off_t iDataLen){
      std::cout<<"Written: ["<<iDataLen<<"]"<<std::endl;
      barrierWork.synchronize();
  }

  std::unique_ptr<FileHandle> ptrFileHandle;
};

const String Server::CStrData("TEST_TEST_TEST_TEST");
/*************************************************************************/
class Acceptor : public IO::TCP::AsyncServer::Acceptor {
  public:
    Acceptor(IO::TCP::AsyncServer* pAsyncServer, RingHandler *pRingHandler):
      IO::TCP::AsyncServer::Acceptor(pRingHandler, pAsyncServer){};

  protected:
    virtual void handleImpl(Net::Conn::TCP::FileHandle* pFileHandle){
      prepare();

      std::unique_ptr<Server> ptrServer(new Server(pRingHandler, pFileHandle));
      ptrServer->WriteHandler::prepare();
      ptrServer->ReadHandler::prepare();
      lstServers.push_back(std::move(ptrServer));
  }

  std::list< std::unique_ptr<Server> > lstServers;
};

/*************************************************************************/
class TestClient : public ReadHandler,
                   public WriteHandler,
                   public Thread, public Runnable{
  public:
    TestClient(FileHandle* pFileHandle):
      ReadHandler(0, pFileHandle),
      WriteHandler(0, pFileHandle),
      ptrFileHandle(pFileHandle),
      bResult(false),
      Thread(this){

        WriteHandler::iovec.iov_len  = CStrData.length();
        WriteHandler::iovec.iov_base = malloc(4096);
        memcpy(WriteHandler::iovec.iov_base, CStrData.c_str(), WriteHandler::iovec.iov_len);

      }

  bool bResult;

  protected:

    virtual void handleRead(off_t iDataLen){
      std::cout<<"Client got data: ["<<iDataLen<<"]"<<std::endl;
      String strData((char*)ReadHandler::iovec.iov_base, iDataLen);

      bResult = strData.compare(Server::CStrData) == 0;
      barrierWork.synchronize();
  }

  virtual void handleWrite(off_t iDataLen){
      std::cout<<"Client written: ["<<iDataLen<<"]"<<std::endl;
      barrierWork.synchronize();
  }

  static const String CStrData;

  virtual void run(){

    std::unique_ptr<URing::RingHandler>  ptrRingHandler(new RingHandler);

    ReadHandler::pRingHandler = ptrRingHandler.get();
    WriteHandler::pRingHandler = ptrRingHandler.get();

    ReadHandler::prepare();
    WriteHandler::prepare();
    ptrRingHandler->handle();

  }


 std::unique_ptr<FileHandle> ptrFileHandle;
};
const String TestClient::CStrData("ClientData");
/*************************************************************************/


/*************************************************************************/
void TCURing::Env::run(){

  SYS::Signal::ThreadRegistration tr;

  std::unique_ptr<IO::TCP::AsyncServer> ptrAsyncServer(new IO::TCP::AsyncServer(peer, DefaultConnectionFactory::GetInstance()));
  std::unique_ptr<IO::TCP::AsyncServer::Acceptor> ptrAcceptor(new Acceptor(ptrAsyncServer.get(), ptrRingHandler.get()));

  ptrAcceptor->prepare();

  barrierStart.synchronize();

  ptrRingHandler->handle();

};
/*************************************************************************/
TCURing::TCURing(TestSuite* pTestSuite):
  TestUnit<TCURing>(this, "URing", pTestSuite){
	IA20_TRACER;

  addCase("tcp/read/write1", &::IA20::TC::TCURing::caseConnect);

  pTestSuite->addTestUnit(this);
}
/*************************************************************************/
TCURing::~TCURing() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void TCURing::caseConnect(){

	IA20::TimeSample ts(true);

  SYS::Signal::GetInstance();
  Env e("0.0.0.0",55556);

  e.start();
  barrierStart.synchronize();

  Net::Conn::TCP::Peer peerLocal("localhost", 55559);
  Net::Conn::TCP::Peer peer1("127.0.0.1", 55556);
  Net::Conn::TCP::Peer peer2("192.168.0.94", 55556);

  IA20_LOG(true,"TEST1");

  std::unique_ptr<Client> ptrClient1(new Client(peer1, peerLocal, DefaultConnectionFactory::GetInstance()));
  std::unique_ptr<Client> ptrClient2(new Client(peer2, peerLocal, DefaultConnectionFactory::GetInstance()));

  TestClient c1(ptrClient1->connect());
  TestClient c2(ptrClient2->connect());

  c1.start();
  c2.start();

  barrierWork.synchronize();
  barrierWork.synchronize();

  c1.stop();
  c2.stop();
  e.stop();

  c1.join();
  c2.join();
  e.join();

  sleep(10);

  if(!c1.bResult || !c2.bResult)
     IA20_THROW(BadUsageException("Read failed! "));

}
/*************************************************************************/
}
}
