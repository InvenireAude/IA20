/*
 * File: ConnectionWorker.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */


#include "ConnectionWorker.h"
#include "Connection.h"
#include "PacketFactory.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
ConnectionWorker::ConnectionWorker(Connection* pConnection, RaftEngine *pRaftEngine):
  pConnection(pConnection),
  Worker(pRaftEngine){
	IA20_TRACER;
}
/*************************************************************************/
ConnectionWorker::~ConnectionWorker() throw(){
	IA20_TRACER;
}
/*******************************************************s******************/
void ConnectionWorker::run(){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: ConnectionWorker thread started !!! ");

  Raft::PacketFactory::SetInstance(new PacketFactory());

  SYS::Signal::ThreadRegistration tr;

  {
    Mutex::Locker locker(TheMutex);
    pRaftEngine->onStart();
  }

  while(!SYS::Signal::GetInstance()->isStopping()){

    Packet packet;

    if(pConnection->receive(packet)){
      Mutex::Locker locker(TheMutex);
      pRaftEngine->onPacket(packet);
    }

  }
}
/*************************************************************************/
}
}
}
}
}
