#include <memory>
#include <ia20/net/conn/mcast/Socket.h>
#include <ia20/net/engine/raft/RaftEngine.h>
#include <unistd.h>

//#include "raft.h"

using namespace IA20;
using namespace IA20::Net::Conn;
using namespace IA20::Net::Engine;
using namespace std;

class Test : public Raft::RaftEngine::Sender{
  public:

  virtual void send(const Raft::RaftEngine::Message* pMessage){
    ptrSocket->send(pMessage, sizeof(*pMessage));
  }

  void run(){

    struct Message {
      uint16_t iValue;
    };

    Message m;

    m.iValue = getpid();

    while(!SYS::Signal::GetInstance()->isStopping()){
      Address address;
      ptrSocket->send(&m,sizeof(m));
      uint32_t iDataLen;
      if(ptrSocket->receive(&m, iDataLen, address)){
        std::cout<<"Message value: "<<m.iValue<<std::endl;
      };
      Thread::Cancellation ca(true);
      sleep(1);
    }
  }

  Test():
   ptrSocket(new MCast::Socket("127.0.0.1", "224.0.0.1", 5000)),
   ptrEngine(new Raft::RaftEngine(this)){
  }

  protected:
   unique_ptr<MCast::Socket> ptrSocket;
   unique_ptr<Raft::RaftEngine> ptrEngine;
};

int main(int argc, char* argv[]){

  std::cout<<IA20::LogLevel::INSTANCE.isSystem()<<std::endl;

  SYS::Signal::GetInstance();

  Test test;
  test.run();
}