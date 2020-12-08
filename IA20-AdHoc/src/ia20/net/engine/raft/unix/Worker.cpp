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

#include <unistd.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Unix {

Mutex Worker::TheMutex;

/*************************************************************************/
Worker::Worker(RaftEngine *pRaftEngine):
  Thread(this),
   pRaftEngine(pRaftEngine){
	IA20_TRACER;
}
/*************************************************************************/
Worker::~Worker() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
}
}
