/*
 * File: Worker.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */


#include "Worker.h"
#include "../Message.h"
#include "Connection.h"
#include "PacketFactory.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
Worker::Worker(Connection* pConnection, RaftEngine *pRaftEngine):
  Thread(this),
  pConnection(pConnection),
  pRaftEngine(pRaftEngine){
	IA20_TRACER;
}
/*************************************************************************/
Worker::~Worker() throw(){
	IA20_TRACER;
}
/*******************************************************s******************/
void Worker::run(){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Worker thread started !!! ");

  Raft::PacketFactory::SetInstance(new PacketFactory());

  SYS::Signal::ThreadRegistration tr;

  pRaftEngine->onStart();

  while(!SYS::Signal::GetInstance()->isStopping()){

    Packet packet;

    if(pConnection->receive(packet)){
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
