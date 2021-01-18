/*
 * File: IA20-CommonLib-TestCases/src/testcases/TCURing.cpp
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


#include "TCURing.h"

#include <ia20/net/conn/tcp/DefaultConnectionFactory.h>
#include <ia20/net/conn/tcp/Client.h>
#include <ia20/net/conn/tcp/Server.h>
#include <ia20/net/conn/tcp/FileHandle.h>
#include <ia20/uring/IO/ReadHandler.h>
#include <ia20/uring/IO/WriteHandler.h>

#include <unistd.h>
#include <string.h>

namespace IA20{
namespace TC{

using namespace URing;
using namespace URing::IO;
using namespace URing::IO::TCP;
using namespace Net;
using namespace Net::Conn;
using namespace Net::Conn::TCP;

static Barrier barrier(2,1000);
/*************************************************************************/
TCURing::Env::Env(const String& strAddress,
          int iPort):
  peer(strAddress, iPort),
  Thread(this){

  ptrRingHandler.reset(new RingHandler);

};
/*************************************************************************/
class Reader : public ReadHandler {
  public:
    Reader(FileHandle* pFileHandle):
      ReadHandler(pFileHandle){};

  protected:
    virtual void handleImpl(){
      std::cout<<"Got data: ["<<iDataLen<<"]"<<std::endl;
      barrier.synchronize();
  }

};
/*************************************************************************/
class Writer : public WriteHandler {
  public:
    Writer(FileHandle* pFileHandle):
      WriteHandler(pFileHandle){
        //iovec.iov_base = (void*)CStrData.c_str();
        iovec.iov_len  = CStrData.length();
        iovec.iov_base = malloc(4096);
        memcpy(iovec.iov_base, CStrData.c_str(), iovec.iov_len);
      };

  static const String CStrData;

  protected:
    virtual void handleImpl(){
      std::cout<<"Written: ["<<iDataLen<<"]"<<std::endl;
      barrier.synchronize();
  }

};
const String Writer::CStrData("TEST_TEST_TEST_TEST");
/*************************************************************************/
class Acceptor : public AsyncServer::Acceptor {
  public:
    Acceptor(AsyncServer* pAsyncServer, RingHandler *pRingHandler):
      AsyncServer::Acceptor(pAsyncServer),
      pRingHandler(pRingHandler){};

  protected:
    virtual void handleImpl(Net::Conn::TCP::FileHandle* pFileHandle){
      ptrFileHandle.reset(pFileHandle);
      ptrReader.reset(new Reader(pFileHandle));
      ptrReader->prepare(pRingHandler);
      ptrWriter.reset(new Writer(pFileHandle));
      ptrWriter->prepare(pRingHandler);
  }

  std::unique_ptr<Net::Conn::TCP::FileHandle> ptrFileHandle;
  std::unique_ptr<Reader> ptrReader;
  std::unique_ptr<Writer> ptrWriter;
  RingHandler *pRingHandler;
};
/*************************************************************************/
void TCURing::Env::run(){

  SYS::Signal::ThreadRegistration tr;

  std::unique_ptr<AsyncServer> ptrAsyncServer(new AsyncServer(peer, DefaultConnectionFactory::GetInstance()));
  std::unique_ptr<AsyncServer::Acceptor> ptrAcceptor(new Acceptor(ptrAsyncServer.get(), ptrRingHandler.get()));

  ptrAcceptor->prepare(ptrRingHandler.get());

  barrier.synchronize();

  ptrRingHandler->handle();

};
/*************************************************************************/
TCURing::TCURing(TestSuite* pTestSuite):
  TestUnit<TCURing>(this, "URing", pTestSuite){
	IA20_TRACER;

  addCase("tcp/read/write", &::IA20::TC::TCURing::caseConnect);

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
  Env e("localhost",55556);

  e.start();

  barrier.synchronize();

  std::unique_ptr<Client> ptrClient(new Client(e.peer, DefaultConnectionFactory::GetInstance()));
  std::unique_ptr<FileHandle> ptrClientFile(ptrClient->connect());
  size_t iWritten;
  ptrClientFile->write("1234567890",10,iWritten);

  barrier.synchronize();

  char sBuffer[256];
  bzero(sBuffer, 256);
  size_t iDataLen;
  ptrClientFile->read(sBuffer,256,iDataLen);

  barrier.synchronize();

  e.stop();
  e.join();

  if(iDataLen != Writer::CStrData.length() ||
    Writer::CStrData.compare(sBuffer) != 0 )
      IA20_THROW(BadUsageException("Read failed: ")<<iDataLen<<"["<<sBuffer<<"]");

}
/*************************************************************************/
}
}
