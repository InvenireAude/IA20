/*
 * File: RaftEngine.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RaftEngine.h"


namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
RaftEngine::RaftEngine(Sender* pSender):
  pSender(pSender){
	IA20_TRACER;
}

/*************************************************************************/
RaftEngine::~RaftEngine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
}
}
}
}
