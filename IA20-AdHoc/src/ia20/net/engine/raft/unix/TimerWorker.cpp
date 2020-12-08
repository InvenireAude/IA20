/*
 * File: TimerWorker.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */


#include "TimerWorker.h"
#include "Connection.h"
#include "PacketFactory.h"

#include <unistd.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

/*************************************************************************/
TimerWorker::TimerWorker(RaftEngine *pRaftEngine):
  Worker(pRaftEngine){
	IA20_TRACER;
}
/*************************************************************************/
TimerWorker::~TimerWorker() throw(){
	IA20_TRACER;
}
/*******************************************************s******************/
void TimerWorker::run(){
	IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: TimerWorker thread started !!! ");

  SYS::Signal::ThreadRegistration tr;
  Raft::PacketFactory::SetInstance(new PacketFactory());

  while(!SYS::Signal::GetInstance()->isStopping()){

    sleep(1);
    {
      Mutex::Locker locker(TheMutex);
      pRaftEngine->onTimer();
    }

  }
}
/*************************************************************************/
}
}
}
}
}
