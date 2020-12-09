/*
 * File: Connection.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Connection.h"
#include <ia20/net/conn/Address.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
Connection::Connection(){
  IA20_TRACER;
}
/*************************************************************************/
Connection::~Connection() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Connection::send(const Packet& packet){
	IA20_TRACER;

  for(auto pEngine : lstRaftEngines){
    pEngine->onPacket(packet);
  }

}
/*************************************************************************/
void Connection::add(RaftEngine* pEngine){
  IA20_TRACER;
  lstRaftEngines.push_back(pEngine);
}
/*************************************************************************/
}
}
}
}
}
