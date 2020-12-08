#include <memory>


#include <ia20/net/engine/raft/unix/Connection.h>
#include <ia20/net/engine/raft/unix/Worker.h>
#include <ia20/net/engine/raft/RaftEngine.h>
#include <unistd.h>

//#include "raft.h"

using namespace IA20;
using namespace IA20::Net::Conn;
using namespace IA20::Net::Engine;
using namespace std;


int main(int argc, char* argv[]){

  SYS::Signal::GetInstance();


  Raft::Logger::Configuration cfgLogger("/tmp/raft/",100000);

  Raft::ServerIdType iServerId = argc == 2 ? atoi(argv[1]) : 1;

  std::unique_ptr<Raft::Unix::Connection> ptrConnection(new Raft::Unix::Connection("127.0.0.1", "224.0.0.1", 5000));
  std::unique_ptr<Raft::RaftEngine>   ptrEngine(new Raft::RaftEngine(iServerId, cfgLogger, ptrConnection.get()));
  std::unique_ptr<Raft::Unix::Worker> ptrWorker(new Raft::Unix::Worker(ptrConnection.get(), ptrEngine.get()));

  ptrWorker->start();
  ptrWorker->join();
}