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


#include <ia20/commonlib/net/conn/tcp/Peer.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <ia20/commonlib/net/conn/tcp/DefaultConnectionFactory.h>
#include <ia20/commonlib/net/conn/tcp/Client.h>
#include <ia20/commonlib/net/conn/tcp/Server.h>
#include <ia20/commonlib/net/conn/tcp/FileHandle.h>
#include <ia20/commonlib/uring/uring.h>
#include <ia20/commonlib/uring/IO/FileHandler.h>

#include <unistd.h>
#include <string.h>

using namespace IA20;

using namespace URing;
using namespace URing::IO;

using namespace Net;
using namespace Net::Conn;
using namespace Net::Conn::TCP;

namespace MY{

Net::Conn::TCP::Peer peer("127.0.0.1", 55556);

long CStart;

/*************************************************************************/
class Worker : public Thread, public Runnable{
  public:
   
    class Handler : public ReadHandler, public WriteHandler {
      public:

      Handler(RingHandler *pRingHandler, FileHandle* pFileHandleIn, FileHandle* pFileHandleOut):
        ReadHandler(pRingHandler, pFileHandleOut),
        WriteHandler(pRingHandler, pFileHandleIn),
        iCounter(CStart){  
        pFileHandleIn->setNonBlocking();
        pFileHandleOut->setNonBlocking();
          ReadHandler::iovec.iov_base = malloc(4096);
          ReadHandler::iovec.iov_len  = 4096;
          ReadHandler::prepare();
          WriteHandler::iovec.iov_base = malloc(4096);
          WriteHandler::iovec.iov_len  = 120;
        *(long*)(WriteHandler::iovec.iov_base) = 100*iCounter;
           WriteHandler::prepare();
          //write(this->WriteHandler::pFileHandle->iFileDescriptor, WriteHandler::iovec.iov_base, 40);
      };

     ~Handler(){
       free(ReadHandler::iovec.iov_base);
      }

    virtual void add(){
      iCounter+1;
    }
    virtual void dec(){
      add();
      add();
      iCounter-2;
    }
    
    virtual void handleRead(off_t iDataLen){
     dec();
      int rc = iDataLen ;
      // int rc =read(this->ReadHandler::pFileHandle->iFileDescriptor, ReadHandler::iovec.iov_base, 4000);

      if(iCounter % 10000 ==0 || rc != 120 || *(long*)WriteHandler::iovec.iov_base != iCounter*100 )
        std::cout<<"Got data: ["<<iDataLen<<"]"<<iCounter<<" rc= "<<rc
          <<" "<<*(long*)WriteHandler::iovec.iov_base<<std::endl;
      
      if(iCounter-- == 0)
        exit(1);

       ReadHandler::prepare();
      *(long*)(WriteHandler::iovec.iov_base) = 100*iCounter;
       WriteHandler::prepare();
    //  write(this->WriteHandler::pFileHandle->iFileDescriptor, WriteHandler::iovec.iov_base, 40);

    }

      virtual void handleWrite(off_t iDataLen){
        //std::cout<<"Send: ["<<iDataLen<<"]"<<std::endl;        
      }

    long iCounter;

    };

  Worker(int fdin,int fdout, int cpu):fdout(fdout),fdin(fdin),cpu(cpu),Thread(this){};

  virtual void run(){

    SYS::Signal::ThreadRegistration tr;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu, &cpuset);
    int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);

    std::unique_ptr<URing::RingHandler>  ptrRingHandler(new RingHandler);
    
    std::unique_ptr<FileHandle>  ptrFileHandleIn(new FileHandle(fdin, peer));
    std::unique_ptr<FileHandle>  ptrFileHandleOut(new FileHandle(fdout, peer));

    Handler h(ptrRingHandler.get(), ptrFileHandleIn.get(), ptrFileHandleOut.get());


    while(!SYS::Signal::GetInstance()->isStopping()){
       ptrRingHandler->loop();
      //  h.handleRead(1);
        
    }


  }

  protected:  
  int fdin, fdout, cpu;
 
};


}

using namespace MY;

/*************************************************************************/
int main(int argc, char* argv[]){

    SYS::Signal::ThreadRegistration tr;

    CStart = argc == 1 ? 10 : atol(argv[1]);
    
    std::unique_ptr<URing::RingHandler>  ptrRingHandler;

    ptrRingHandler.reset(new RingHandler);

    int req[2];
    int flags = 0 ;// | O_NONBLOCK;
    pipe2(req,  flags);

    int res[2];
    pipe2(res,  flags);

    std::unique_ptr<MY::Worker> ptrServer(new MY::Worker(req[1],res[0],1));
    std::unique_ptr<MY::Worker> ptrClient(new MY::Worker(res[1],req[0],2));


    // Net::Conn::TCP::Peer peer("localhost", 55558);
    // Net::Conn::TCP::Peer peerLocal("localhost", 55559);


    // std::unique_ptr<Server> ptrServerNet(new Server(peer, DefaultConnectionFactory::GetInstance()));
    // //ptrServerNet->bind();

    // std::unique_ptr<Client> ptrClientNet(new Client(peer, peerLocal, DefaultConnectionFactory::GetInstance()));
    // std::unique_ptr<FileHandle> ptrClientFile(ptrClientNet->connect());

    // std::unique_ptr<FileHandle> ptrServerFile(ptrServerNet->accept());

    // std::unique_ptr<MY::Worker> ptrServer(new MY::Worker(ptrServerFile->iFileDescriptor,ptrServerFile->iFileDescriptor,1));
    // std::unique_ptr<MY::Worker> ptrClient(new MY::Worker(ptrClientFile->iFileDescriptor,ptrClientFile->iFileDescriptor,2));
   
    ptrServer->start();
    ptrClient->start();

    ptrClient->join();
    ptrServer->join();  

};



/*************************************************************************/
