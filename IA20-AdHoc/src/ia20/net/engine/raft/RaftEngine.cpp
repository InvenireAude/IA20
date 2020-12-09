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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: setup, servers: "<<(int)data.iMyServerId<<" / "<<(int)data.iNumServers);

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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: starting up ...");

  startElection();
}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::VoteRequest* pAction){
  IA20_TRACER;

  bool bVoteGranted = false;

  if(pAction->term() < data.p.iCurrentTerm){
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[1]");
      bVoteGranted = false;
  }else{
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[2]: votedFor: "<<(int)data.p.iVotedFor
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

  if(data.iState != ST_Candidate)
    return;

  if(pAction->granted() && pAction->term() == data.p.iCurrentTerm){

    data.v.iVoteCount++;

    if(data.v.iVoteCount * 2 >= data.iNumServers)
      convertToLeader();
  }

}
/*************************************************************************/
static inline FB::LogEntryId _LogKeyToFB(const LogEntryId& entryId){
  return FB::LogEntryId(entryId.iTerm, entryId.iIndex);
}
/*************************************************************************/
static inline FB::LogEntryId _LogKeyToFB(const LogEntry* pLogEntry){
  LogEntryId entryId( pLogEntry ? pLogEntry->getEntryId() : LogEntryId() );
  return FB::LogEntryId(entryId.iTerm, entryId.iIndex);
}
/*************************************************************************/
static inline LogEntryId _FBToLogKey(const FB::LogEntryId& entryId){
  return LogEntryId(entryId.term(), entryId.index());
}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::AppendLogRequest* pAction){
  IA20_TRACER;

  if(data.iState != ST_Follower)
    return;

  tsElapsed.start();

  const flatbuffers::Vector<uint8_t> *pActionData = pAction->data();

  LogEntrySizeType  iEntryDataSize = 0;
  const void*       pSrcData = NULL;

  if(pActionData){
    iEntryDataSize = pActionData->size();
    pSrcData = pActionData->data();
  };

  bool bResult = false;

  LogEntryId keyMatch( data.pLastLogEntry ? data.pLastLogEntry->getEntryId() : LogEntryId() );
  LogEntryId keyMessageMatch(_FBToLogKey(*pAction->matchLogEntry()));

  if(keyMatch == keyMessageMatch){

    LogEntryId entryId(_FBToLogKey(*pAction->dataLogEntry()));
    data.pLastLogEntry = pLogger->appendEntry(entryId, iEntryDataSize, pSrcData);

    bResult = true;

  }else{

  }

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(pHeader->srcServerId(), data.iMyServerId);

  auto response = FB::CreateAppendLogResponse(builder, pAction->dataLogEntry(), bResult);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogResponse, response.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);
}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::AppendLogResponse* pAction){
  IA20_TRACER;

  if(data.iState != ST_Leader)
    return;

  IA20_CHECK_IF_NULL(pAction->dataLogEntry());

   // TODO check term ? or entryID ?
   LogIndexType  iIndexId = pAction->dataLogEntry()->index();

   for(int iEntryIdx = 0; iEntryIdx < 1; iEntryIdx++){

    for(PendingEntriesList::iterator it = lstPendingEntries.begin(); it != lstPendingEntries.end(); it++){

      PendingEntry& entry = *it;

      LogEntryId entryId(entry.pEntry->getEntryId());

      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Match for commit:"<<entryId.iTerm<<","<<entryId.iIndex);

      if(entryId.iIndex <= iIndexId)
        entry.iNumConfirmations++;
        IA20_LOG(LogLevel::INSTANCE.isInfo(), "iNumConfirmations: "<<entry.iNumConfirmations);
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

  const FB::Message* pMessage = FB::GetMessage(pDataStart);

  if(!pMessage->header() || pMessage->header()->srcServerId() == data.iMyServerId ||
      (pMessage->header()->dstServerId() && pMessage->header()->dstServerId() != data.iMyServerId))
    return; /* dark side of broadcasting */

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: ********************************");
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: *** onPacket : "<<
      IA20::FB::Helpers::ToString(pDataStart, FB::MessageTypeTable()));

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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: *** timer, tsElapsed: "
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

  FB::LogEntryId lastEntryId(_LogKeyToFB(data.pLastLogEntry));

  auto request = FB::CreateVoteRequest(builder, data.p.iCurrentTerm, data.iMyServerId, &lastEntryId);
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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: I am a new leader, term: "<<data.p.iCurrentTerm);

  data.v.iLastApplied = 0;

  for(int i=0; i<data.iNumServers; i++){
    data.servers[i].pMatchEntry = data.pLastLogEntry;
  };

  lstPendingEntries.clear();

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId matchEntryId(_LogKeyToFB(data.pLastLogEntry));

  LogEntryId entryId(data.p.iCurrentTerm,++data.v.iLastApplied);
  data.pLastLogEntry = pLogger->appendEntry(entryId);


  lstPendingEntries.push_back({
        pEntry : data.pLastLogEntry,
        iNumConfirmations : 1});

  FB::LogEntryId dataEntryId(_LogKeyToFB(data.pLastLogEntry));

  auto request = FB::CreateAppendLogRequest(builder, data.iMyServerId, &matchEntryId, &dataEntryId, data.v.iCommitIndex, 0);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::sendHeartbeat(){
  IA20_TRACER;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Heartbeat, term: "<<data.p.iCurrentTerm);

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId matchLogEntry(_LogKeyToFB(data.pLastLogEntry));
  FB::LogEntryId dataLogEntry(_LogKeyToFB(data.pLastLogEntry));

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
    ///IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: I am not a leader !!!");
    return;
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: New entry, term: "<<data.p.iCurrentTerm<<", sz: "<<iEntrySize);

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId matchLogEntry(_LogKeyToFB(data.pLastLogEntry));

  LogEntryId entryId(data.p.iCurrentTerm,++data.v.iLastApplied);
  data.pLastLogEntry = pLogger->appendEntry(entryId, iEntrySize, pEntryData);

  lstPendingEntries.push_back({
        pEntry : data.pLastLogEntry,
       iNumConfirmations : 1});


  FB::LogEntryId dataLogEntry(_LogKeyToFB(data.pLastLogEntry));

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
