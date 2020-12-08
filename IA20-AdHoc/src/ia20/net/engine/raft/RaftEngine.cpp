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
RaftEngine::RaftEngine(ServerIdType iMyServerId, ServerIdType iNumServers, const Logger::Configuration& cfgLogger, Sender* pSender):
  pSender(pSender),
  ptrLogger(new Logger(cfgLogger, iMyServerId)){
	IA20_TRACER;

  IA20_CHECK_IF_NULL(pSender);

  data.pLastLogEntry = NULL;
  data.iMyServerId = iMyServerId;
  data.iNumServers = iNumServers;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: setup, servers: "<<(int)data.iMyServerId<<" / "<<(int)data.iNumServers);

  convertToFollower();
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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: VoteRequestMessage: from: "<<(int)message.request()->iCandidate);

  if(message.request()->iCandidate == data.iMyServerId)
    return; /* dark side of broadcasting */

  Packet packetResponse;
  VoteResponseMessage messageResponse(packetResponse);

  if(message.request()->iTerm < data.p.iCurrentTerm){
      messageResponse.response()->bVoteGranted = false; // par5.1
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Vote[1]");
  }else{

      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Vote[2]: votedFor: "<<(int)data.p.iVotedFor
                <<", iLastLogTerm: "<<message.request()->iLastLogTerm<<", iCurrentTerm: "<<data.p.iCurrentTerm
                <<", iLastLogIndexId: "<<message.request()->iLastLogIndexId<<", iLastApplied: "<<data.v.iLastApplied);

      if( (data.p.iVotedFor == CSeverNull ||
           data.p.iVotedFor == message.request()->iCandidate) && true
           //( //message.request()->iLastLogTerm > data.p.iCurrentTerm ||
           // (// message.request()->iLastLogTerm == data.p.iCurrentTerm  <--- log lookup
             //message.request()->iLastLogIndexId >= data.v.iLastApplied))
             ){
               messageResponse.response()->bVoteGranted = true; // par5.2-3
             }else{
               messageResponse.response()->bVoteGranted = false;
             }
  }

  messageResponse.response()->iTerm  = data.p.iCurrentTerm;
  messageResponse.response()->iVoter = data.iMyServerId;
  messageResponse.setDstServer(message.request()->iCandidate);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Vote[3], bVoteGranted: "<<messageResponse.response()->bVoteGranted);

  pSender->send(packetResponse);
}
/*************************************************************************/
void RaftEngine::onMessage(const VoteResponseMessage& message){
  IA20_TRACER;

  ServerIdType iDstServerId = message.getDstServer();

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: VoteResponseMessage: "<<message.response()->bVoteGranted<<" --> "<<(int)iDstServerId);

  if(iDstServerId != data.iMyServerId)
    return;

  // TODO check term

  if(message.response()->bVoteGranted){

    data.v.iVoteCount++;

    if(data.v.iVoteCount * 2 >= data.iNumServers)
      convertToLeader();
  }

}
/*************************************************************************/
void RaftEngine::onMessage(const AppendEntriesRequest& message){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: AppendEntriesRequest: leaderId: "<<message.request()->iLeaderId);

  if(message.request()->iLeaderId  == data.iMyServerId)
    return; /* dark side of broadcasting */

  tsElapsed.start();

  ptrLogger->appendEntry(data.p.iCurrentTerm,
                         data.v.iLastApplied++,
                         0,
                         0);

  Packet packet;

  AppendEntriesResponse messageResponse(packet);
  messageResponse.setDstServer(message.request()->iLeaderId);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: New remote entry, term: "<<data.p.iCurrentTerm);

  messageResponse.response()->bSuccess     = true;
  messageResponse.response()->currentTerm  = data.p.iCurrentTerm;
  messageResponse.response()->iIndexId     = message.request()->iIndexId;
  messageResponse.response()->iEntriesSize = message.request()->iEntriesSize;

  pSender->send(packet);
}
/*************************************************************************/
void RaftEngine::onMessage(const AppendEntriesResponse& message){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: AppendEntriesResponse: ");

  ServerIdType iDstServerId = message.getDstServer();
  if(iDstServerId != data.iMyServerId)
    return;

   LogIndexType   iIndexId = message.response()->iIndexId;

   for(int iEntryIdx = 0; iEntryIdx < message.response()->iEntriesSize; iEntryIdx++){

    for(PendingEntriesList::iterator it = lstPendingEntries.begin(); it != lstPendingEntries.end(); it++){
      PendingEntry& entry = *it;
      if(entry.pEntry->getIndex() <= iIndexId)

        entry.iNumConfirmations++;

        if(entry.iNumConfirmations * 2 > data.iNumServers){
          ptrLogger->commit(entry.pEntry);
          it = lstPendingEntries.erase(it);
        }
    }

    iIndexId++;
   };

}
/*************************************************************************/
void RaftEngine::onPacket(Packet& packet){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: *** onPacket, type: "<< Message::GetType(packet));

  switch(Message::GetType(packet)){

    case Message::MT_VoteRequest:{
      const VoteRequestMessage message(packet);
      onMessage(message);
    };
    break;

    case Message::MT_VoteResponse:{
      const VoteResponseMessage message(packet);
      onMessage(message);
    };
    break;

    case Message::MT_AppendEntriesRequest:{
      const AppendEntriesRequest message(packet);
      onMessage(message);
    };
    break;

  }
}
/*************************************************************************/
void RaftEngine::onTimer(){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: *** timer, tsElapsed: "
              <<tsElapsed.getSample()<<", state: "<<data.iState);

  switch(data.iState){

    case ST_Leader:
        sendHeartbeat();
      break;

    case ST_Follower:

      if(tsElapsed.getSample() > CAppendEntriesTimeoutNS)
        startElection(); /* append entries timeout */
      break;

    case ST_Candidate:
      if(tsElapsed.getSample() > CElectionTimeoutNS)
        startElection(); /* election timeout */
      break;
  }

}
/*************************************************************************/
void RaftEngine::startElection(){
  IA20_TRACER;

  data.iState = ST_Candidate;

  data.p.iCurrentTerm++;
  data.v.iVoteCount = 1;

  Packet packet;
  VoteRequestMessage message(packet);

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: New election, term: "<<data.p.iCurrentTerm);
  message.setDstServer(0);
  message.request()->iCandidate       = data.iMyServerId;
  message.request()->iTerm            = data.p.iCurrentTerm;
  message.request()->iLastLogTerm     = data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0 ;
  message.request()->iLastLogIndexId  = data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0 ;

  pSender->send(packet);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::convertToLeader(){
  IA20_TRACER;

  data.iState = ST_Leader;

  Packet packet;
  AppendEntriesRequest message(packet);
  message.setDstServer(0);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: I am a new leader, term: "<<data.p.iCurrentTerm);

  message.request()->iTerm = data.p.iCurrentTerm;
  message.request()->iLeaderId = data.iMyServerId;

  message.request()->iEntriesSize       = 0;
  message.request()->iLeaderCommitIndex = data.v.iCommitIndex;
  message.request()->iPrevLogIndex      = data.v.iLastApplied; //
  message.request()->iPrevLogTerm       = data.p.iCurrentTerm - 1; // TODO this is bad, use the logger class for this ?

  pSender->send(packet);

  data.v.iLastApplied = 1;

  lstPendingEntries.clear();

  lstPendingEntries.push_back({
        pEntry : ptrLogger->appendEntry(data.p.iCurrentTerm,
                         data.v.iLastApplied,
                         0,
                         0),
       iNumConfirmations : 1});


  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::sendHeartbeat(){
  IA20_TRACER;


  Packet packet;
  AppendEntriesRequest message(packet);
  message.setDstServer(0);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Heartbeat, term: "<<data.p.iCurrentTerm);

  message.request()->iTerm = data.p.iCurrentTerm;
  message.request()->iLeaderId = data.iMyServerId;

  message.request()->iEntriesSize       = 0;
  message.request()->iLeaderCommitIndex = data.v.iCommitIndex;
  message.request()->iPrevLogIndex      = data.v.iLastApplied - 1; //
  message.request()->iPrevLogTerm       = data.p.iCurrentTerm ;

  pSender->send(packet);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::convertToFollower(){
  IA20_TRACER;

  data.iState = ST_Follower;
  tsElapsed.start();

}
/*************************************************************************/
void RaftEngine::onData(void *pEntryData, LogEntrySizeType iEntrySize){
  IA20_TRACER;

  if(data.iState != ST_Leader){
    IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: I am not a leader !!!");
    return;
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: New entry, term: "<<data.p.iCurrentTerm<<", sz: "<<iEntrySize);

  lstPendingEntries.push_back({
        pEntry : ptrLogger->appendEntry(data.p.iCurrentTerm,
                         ++data.v.iLastApplied,
                         iEntrySize,
                         pEntryData),
       iNumConfirmations : 1});

  Packet packet;

  AppendEntriesRequest message(packet);
  message.setDstServer(0);


  message.request()->iTerm    = data.p.iCurrentTerm;
  message.request()->iIndexId = data.v.iLastApplied;

  message.request()->iLeaderId = data.iMyServerId;

  message.request()->iEntriesSize       = 1;
  message.request()->iLeaderCommitIndex = data.v.iCommitIndex;
  message.request()->iPrevLogIndex      = data.v.iLastApplied - 1;
  message.request()->iPrevLogTerm       = data.p.iCurrentTerm;

  pSender->send(packet);

  tsElapsed.start();
}
/*************************************************************************/
}
}
}
}
