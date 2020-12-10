#include <memory>

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/engine/raft/unix/Connection.h>
#include <ia20/net/engine/raft/unix/ConnectionWorker.h>
#include <ia20/net/engine/raft/unix/TimerWorker.h>
#include <ia20/net/engine/raft/unix/PacketFactory.h>
#include <ia20/net/engine/raft/RaftEngine.h>
#include <ia20/net/engine/raft/LogFileWriter.h>
#include <ia20/net/engine/raft/LogFileAllocator.h>

#include <unistd.h>

//#include "raft.h"

using namespace IA20;
using namespace IA20::Net::Conn;
using namespace IA20::Net::Engine;
using namespace std;

class DataWorker : public Raft::Unix::Worker{
  public:

	DataWorker(Raft::RaftEngine *pRaftEngine, Raft::ServerIdType iServerId):
    Worker(pRaftEngine),
    iServerId(iServerId){}

  virtual void run(){

      SYS::Signal::ThreadRegistration tr;
      Raft::PacketFactory::SetInstance(new  Raft::Unix::PacketFactory());

      int i=0;

      while(!SYS::Signal::GetInstance()->isStopping()){

        char b[256];
        sprintf(b,"Item %d from: %d", i++, iServerId);
        sleep(1);

        {
          Mutex::Locker  locker(TheMutex);
          pRaftEngine->onData(b,strlen(b));
        }

      }
    }


    Raft::ServerIdType iServerId ;
};

int main(int argc, char* argv[]){

  SYS::Signal::GetInstance();

  Raft::ServerIdType iServerId   = argc >= 2 ? atoi(argv[1]) : 1;
  Raft::ServerIdType iNumServers = argc >= 3 ? atoi(argv[2]) : 3;

  String strLogFile(Raft::LogFile::CreateFileName("/tmp/raft/",iServerId));

  std::unique_ptr<Raft::Unix::Connection> ptrConnection(new Raft::Unix::Connection("127.0.0.1", "224.0.0.1", 5000));

  std::unique_ptr<Raft::LogFileWriter>ptrLogFileWriter(new Raft::LogFileWriter(strLogFile, 1000000));
  std::unique_ptr<Raft::RaftEngine>   ptrEngine(new Raft::RaftEngine(iServerId, iNumServers, ptrLogFileWriter.get(), ptrConnection.get()));

  std::unique_ptr<Raft::Unix::ConnectionWorker> ptrConnectionWorker(new Raft::Unix::ConnectionWorker(ptrConnection.get(), ptrEngine.get()));
  std::unique_ptr<Raft::Unix::TimerWorker> ptrTimerWorker(new Raft::Unix::TimerWorker(ptrEngine.get()));
  std::unique_ptr<DataWorker> ptrDataWorker(new DataWorker(ptrEngine.get(), iServerId));

  ptrConnectionWorker->start();
  ptrTimerWorker->start();
  ptrDataWorker->start();

  ptrConnectionWorker->join();
  ptrTimerWorker->join();
  ptrDataWorker->join();

}