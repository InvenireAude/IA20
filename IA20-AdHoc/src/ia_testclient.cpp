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


#include <ia20/uring/RingHandler.h>
#include <ia20/uring/IO/TCP/AsyncServer.h>
#include <ia20/uring/IO/ReadHandler.h>

#include <ia20/commonlib/net/conn/tcp/Peer.h>


#include <ia20/commonlib/net/conn/tcp/DefaultConnectionFactory.h>
#include <ia20/commonlib/net/conn/tcp/Client.h>
#include <ia20/commonlib/net/conn/tcp/Server.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <ia20/uring/IO/ReadHandler.h>
#include <ia20/uring/IO/WriteHandler.h>
#include <ia20/uring/IO/ShutdownHandler.h>

#include <unistd.h>
#include <string.h>

using namespace IA20;

using namespace URing;
using namespace URing::IO;

using namespace Net;
using namespace Net::Conn;
using namespace Net::Conn::TCP;

namespace MY{

static String CMSgCONNECT_Req("100C00044D5154540402003C0000");
static String CMSgCONNECT_Res("20020000");
static String CMSgPUB_Req("30100009686F6D652F74656D703132333435");
static String CMSgDISC_Req("E000");

/*************************************************************************/
class TestClient : public ReadHandler,
                   public WriteHandler,
                   public Thread, public Runnable{
  public:
    TestClient(FileHandle* pFileHandle):
      ReadHandler(0, pFileHandle),
      WriteHandler(0, pFileHandle),
      ptrFileHandle(pFileHandle),
      bConnected(false),
      Thread(this){

        WriteHandler::iovec.iov_len  = 4096;
        WriteHandler::iovec.iov_base = malloc(4096);
      }

  volatile bool bConnected;
  int  iLeft = 1;

  protected:

    void sendAndRead(const String& msg){
      MiscTools::HexToBinary(msg, WriteHandler::iovec.iov_base, 4000);
      WriteHandler::iovec.iov_len = msg.length()/2;
      WriteHandler::prepare();
      ReadHandler::prepare();
  }

    void send(const String& msg){
      MiscTools::HexToBinary(msg, WriteHandler::iovec.iov_base, 4000);
      int iMsgLen = WriteHandler::iovec.iov_len = msg.length()/2;

      uint8_t *s = (uint8_t*)(WriteHandler::iovec.iov_base);
      uint8_t *c = s + iMsgLen;

      while(--iLeft > 0 && WriteHandler::iovec.iov_len < 1000){
        memcpy(c, s, iMsgLen);
        c += iMsgLen;
        WriteHandler::iovec.iov_len += iMsgLen;
      }


      WriteHandler::prepare();
    }

    virtual void handleRead(off_t iDataLen){
        //  std::cout<<"Client read: ["<<iDataLen<<"]:"<<bConnected<<", iLeft:"<<iLeft<<std::endl;
          bConnected = true;
          send(CMSgPUB_Req);
    }


  virtual void handleWrite(off_t iDataLen){
      //std::cout<<"Client written: ["<<iDataLen<<"]:"<<bConnected<<", iLeft:"<<iLeft<<std::endl;

      if(!bConnected){
        return;
      }

      if(iLeft == -1){
          Thread::stop();
      }

      if(iLeft == 0){
        send(CMSgDISC_Req);
      }else{
        send(CMSgPUB_Req);
      }

  }

  virtual void run(){

    std::unique_ptr<URing::RingHandler>  ptrRingHandler(new RingHandler);

    ReadHandler::pRingHandler = ptrRingHandler.get();
    WriteHandler::pRingHandler = ptrRingHandler.get();

    sendAndRead(CMSgCONNECT_Req);

    ptrRingHandler->handle();

  }

 std::unique_ptr<FileHandle> ptrFileHandle;
};

}

using namespace MY;

/*************************************************************************/
int main(int argc, char* argv[]){

    SYS::Signal::ThreadRegistration tr;

    try{

      Net::Conn::TCP::Peer peer(argc <= 1 ? "127.0.0.1" : argv[1], argc <= 2 ? 55556 : atoi(argv[2]));
      Net::Conn::TCP::Peer peerLocal("127.0.0.1");

      std::unique_ptr<Client> ptrClient1(new Client(peer, peerLocal, DefaultConnectionFactory::GetInstance()));
      TestClient c1(ptrClient1->connect());
      c1.iLeft = argc <= 3 ? 1 : atoi(argv[3]);
      c1.start();
      c1.join();

      }catch(Exception& e){
        e.printToStream(std::cerr);
    }

  return 0;
};
/*************************************************************************/
