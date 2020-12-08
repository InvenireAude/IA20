/*
 * File: RaftEngine.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RaftEngine.h"

#include "Logger.h"
#include "Message.h"
#include "Packet.h"

#include "PacketFactory.h"
#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
RaftEngine::RaftEngine(ServerIdType iMyServerId, const Logger::Configuration& cfgLogger, Sender* pSender):
  pSender(pSender),
  ptrLogger(new Logger(cfgLogger, iMyServerId)){
	IA20_TRACER;

  IA20_CHECK_IF_NULL(pSender);

  data.iState = ST_NONE;
  data.pLastLogEntry = NULL;
  data.iMyServerId = iMyServerId;
}
/*************************************************************************/
RaftEngine::~RaftEngine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void RaftEngine::onStart(){
  IA20_TRACER;

  if(data.iState != ST_NONE)
    return;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: starting up ...");

  startElection();
}
/*************************************************************************/
void RaftEngine::onMessage(const VoteRequestMessage& message){
  IA20_TRACER;
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: VoteRequestMessage: "<<message.getServer());

  Packet packetResponse;
  VoteResponseMessage messageResponse(packetResponse);

  messageResponse.response()->bVoteGranted = true;
  messageResponse.response()->iTerm        = data.p.iTerm;

  pSender->send(packetResponse);
}
/*************************************************************************/
void RaftEngine::onMessage(const VoteResponseMessage& message){
  IA20_TRACER;
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: VoteResponseMessage: "<<message.getServer());
}
/*************************************************************************/
void RaftEngine::onMessage(const AppendEntriesRequest& message){
  IA20_TRACER;
}
/*************************************************************************/
void RaftEngine::onMessage(const AppendEntriesResponse& message){
  IA20_TRACER;
}
/*************************************************************************/
void RaftEngine::onPacket(Packet& packet){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: onPacket, type: "<< Message::GetType(packet));

  switch(Message::GetType(packet)){

    case Message::MT_VoteRequest:{
      const VoteRequestMessage message(packet);
      onMessage(message);
    };

  }
}
/*************************************************************************/
void RaftEngine::startElection(){
  IA20_TRACER;

  data.iState = ST_Candidate;

  Packet packet;
  VoteRequestMessage message(packet);

  data.p.iTerm++;
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: New elections, term: "<<data.p.iTerm);

  message.request()->iCandidate       = data.iMyServerId;
  message.request()->iTerm            = data.p.iTerm;
  message.request()->iLastLogTerm     = data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0 ;
  message.request()->iLastLogIndexId  = data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0 ;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Message Factory: "<<(void*)PacketFactory::GetInstance());
  pSender->send(packet);
}
/*************************************************************************/
}
}
}
}
