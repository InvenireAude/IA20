/*
 * File: Connection.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "Connection.h"
#include <ia20/commonlib/net/conn/Address.h>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
Connection::Connection():bInProgress(false){
  IA20_TRACER;
}
/*************************************************************************/
Connection::~Connection() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Connection::send(const Packet& packet){
	IA20_TRACER;

  lstPackets.push_back(packet);

  if(!bInProgress){

    bInProgress = true;

    while(lstPackets.size() > 0){

      const Packet& packet = lstPackets.front();

      for(auto pEngine : lstRaftEngines){
        if(setBlackList.count(pEngine) == 0)
          pEngine->onPacket(packet);
      }

      lstPackets.pop_front();
    }

    bInProgress = false;
  }

}
/*************************************************************************/
void Connection::add(RaftEngine* pEngine){
  IA20_TRACER;
  lstRaftEngines.push_back(pEngine);
}
/*************************************************************************/
void Connection::disable(RaftEngine* pEngine){
  IA20_TRACER;
  setBlackList.insert(pEngine);
}
/*************************************************************************/
void Connection::enable(RaftEngine* pEngine){
  IA20_TRACER;
  setBlackList.erase(pEngine);
}
/*************************************************************************/
}
}
}
}
}
