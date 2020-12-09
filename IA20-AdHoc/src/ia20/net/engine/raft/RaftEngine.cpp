/*
 * File: RaftEngine.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RaftEngine.h"

#include <ia20/fb/Helpers.h>

#include "Logger.h"
#include "Packet.h"

#include "PacketFactory.h"
#include "LogEntry.h"


namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
RaftEngine::RaftEngine(ServerIdType iMyServerId, ServerIdType iNumServers, Logger* pLogger, Sender* pSender):
  pSender(pSender),
  pLogger(pLogger){
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
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::VoteRequest* pAction){
  IA20_TRACER;

  if(pHeader->srcServerId() == data.iMyServerId)
    return; /* dark side of broadcasting */

  bool bVoteGranted = false;

  if(pAction->term() < data.p.iCurrentTerm){
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Vote[1]");
      bVoteGranted = false;
  }else{
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Vote[2]: votedFor: "<<(int)data.p.iVotedFor
                <<", iLastLogTerm: "<<pAction->lastLogEntry()->term()<<", iCurrentTerm: "<<data.p.iCurrentTerm
                <<", iLastLogIndexId: "<<pAction->lastLogEntry()->index()<<", iLastApplied: "<<data.v.iLastApplied);

      if( (data.p.iVotedFor == CSeverNull ||
           data.p.iVotedFor == pAction->candidate()) && true
           //( //message.request()->iLastLogTerm > data.p.iCurrentTerm ||
           // (// message.request()->iLastLogTerm == data.p.iCurrentTerm  <--- log lookup
             //message.request()->iLastLogIndexId >= data.v.iLastApplied))
             ){
               bVoteGranted = true; // par5.2-3
             }else{
               bVoteGranted = false;
             }
  }

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(pHeader->srcServerId(), data.iMyServerId);
  auto response = FB::CreateVoteResponse(builder, pAction->term(), pAction->candidate(), bVoteGranted);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_VoteResponse, response.Union()));


  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);

}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::VoteResponse* pAction){
  IA20_TRACER;

  if(pHeader->srcServerId() == data.iMyServerId || data.iState != ST_Candidate)
    return; /* dark side of broadcasting */

  if(pAction->granted() && pAction->term() == data.p.iCurrentTerm){

    data.v.iVoteCount++;

    if(data.v.iVoteCount * 2 >= data.iNumServers)
      convertToLeader();
  }

}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::AppendLogRequest* pAction){
  IA20_TRACER;

  if(pHeader->srcServerId() == data.iMyServerId || data.iState != ST_Follower)
    return; /* dark side of broadcasting */

  tsElapsed.start();

  pLogger->appendEntry(data.p.iCurrentTerm,
                         data.v.iLastApplied++,
                         0,
                         0);

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(pHeader->srcServerId(), data.iMyServerId);

  auto response = FB::CreateAppendLogResponse(builder, pAction->dataLogEntry(), true);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogResponse, response.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);
}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::AppendLogResponse* pAction){
  IA20_TRACER;

  if(pHeader->srcServerId() == data.iMyServerId || data.iState != ST_Follower)
    return; /* dark side of broadcasting */

  IA20_CHECK_IF_NULL(pAction->dataLogEntry());

   LogIndexType  iIndexId = pAction->dataLogEntry()->index();

   for(int iEntryIdx = 0; iEntryIdx < 1; iEntryIdx++){

    for(PendingEntriesList::iterator it = lstPendingEntries.begin(); it != lstPendingEntries.end(); it++){
      PendingEntry& entry = *it;
      if(entry.pEntry->getIndex() <= iIndexId)

        entry.iNumConfirmations++;

        if(entry.iNumConfirmations * 2 > data.iNumServers){
          pLogger->commit(entry.pEntry);
          it = lstPendingEntries.erase(it);
        }
    }

    iIndexId++;
   };

}
/*************************************************************************/
void RaftEngine::onPacket(const Packet& packet){
  IA20_TRACER;

  const uint8_t* pDataStart = packet.getDataStart();

  flatbuffers::Verifier verifier(pDataStart, packet.getLength());

//  if(!FB::VerifyMessageBuffer(verifier))
//    IA20_THROW(BadUsageException("Verify message failed"));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: *** onPacket : "<<
      IA20::FB::Helpers::ToString(pDataStart, FB::MessageTypeTable()));


  const FB::Message* pMessage = FB::GetMessage(pDataStart);

  switch(pMessage->action_type()){

    case FB::Action_VoteRequest:
      onMessage(pMessage->header(), pMessage->action_as_VoteRequest());
    break;

    case FB::Action_VoteResponse:
      onMessage(pMessage->header(), pMessage->action_as_VoteResponse());
    break;

    case FB::Action_AppendLogRequest:
      onMessage(pMessage->header(), pMessage->action_as_AppendLogRequest());
    break;

    case FB::Action_AppendLogResponse:
      onMessage(pMessage->header(), pMessage->action_as_AppendLogResponse());
    break;

    default:
      IA20_THROW(InternalException("Unsupported FB/RPC in Raft Engine."));
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

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId lastLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                              data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);

  auto request = FB::CreateVoteRequest(builder, data.p.iCurrentTerm, data.iMyServerId, &lastLogEntry);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_VoteRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::convertToLeader(){
  IA20_TRACER;

  data.iState = ST_Leader;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: I am a new leader, term: "<<data.p.iCurrentTerm);

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  //TODO use values form Logger instance

  FB::LogEntryId matchLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);
  FB::LogEntryId dataLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);

  auto request = FB::CreateAppendLogRequest(builder, data.iMyServerId, &matchLogEntry, &dataLogEntry, data.v.iCommitIndex, 0);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);

  data.v.iLastApplied = 1;

  lstPendingEntries.clear();

  lstPendingEntries.push_back({
        pEntry : pLogger->appendEntry(data.p.iCurrentTerm,
                         data.v.iLastApplied,
                         0,
                         0),
       iNumConfirmations : 1});


  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::sendHeartbeat(){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Heartbeat, term: "<<data.p.iCurrentTerm);

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  //TODO use values form Logger instance

  FB::LogEntryId matchLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);
  FB::LogEntryId dataLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);

  auto request = FB::CreateAppendLogRequest(builder, data.iMyServerId, &matchLogEntry, &dataLogEntry, data.v.iCommitIndex, 0);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
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
    ///IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: I am not a leader !!!");
    return;
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: New entry, term: "<<data.p.iCurrentTerm<<", sz: "<<iEntrySize);

  lstPendingEntries.push_back({
        pEntry : pLogger->appendEntry(data.p.iCurrentTerm,
                         ++data.v.iLastApplied,
                         iEntrySize,
                         pEntryData),
       iNumConfirmations : 1});

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  //TODO use values form Logger instance

  FB::LogEntryId matchLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);
  FB::LogEntryId dataLogEntry(data.pLastLogEntry ? data.pLastLogEntry->getTerm() : 0,
                               data.pLastLogEntry ? data.pLastLogEntry->getIndex() : 0);

  auto entryData = builder.CreateVector<uint8_t>(reinterpret_cast<uint8_t*>(pEntryData), iEntrySize);

  auto request = FB::CreateAppendLogRequest(builder, data.iMyServerId, &matchLogEntry, &dataLogEntry, data.v.iCommitIndex,  entryData);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);


  tsElapsed.start();
}
/*************************************************************************/
}
}
}
}
