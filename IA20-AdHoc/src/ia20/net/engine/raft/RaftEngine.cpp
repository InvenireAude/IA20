/*
 * File: RaftEngine.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RaftEngine.h"

#include <ia20/fb/Helpers.h>


#include "Packet.h"
#include "PacketFactory.h"
#include "LogEntry.h"
#include "LogFileWriter.h"


namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
RaftEngine::RaftEngine(ServerIdType iMyServerId, ServerIdType iNumServers, LogFileWriter *pLogFileWriter, Sender* pSender):
  pSender(pSender),
  pLogFileWriter(pLogFileWriter){
	IA20_TRACER;

  IA20_CHECK_IF_NULL(pSender);

  data.pLastLogEntry = pLogFileWriter->appendEntry(LogEntryId()); // initialize with [0,0]
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

  LogFile::PersistentData* pPersistentData = pLogFileWriter->getPersistentData();

  if(pAction->term() < pPersistentData->iCurrentTerm){
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[1]");
      bVoteGranted = false;
  }else{
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[2]: votedFor: "<<(int)pPersistentData->iVotedFor
                <<", iLastLogTerm: "<<pAction->lastLogEntry()->term()<<", iCurrentTerm: "<<pPersistentData->iCurrentTerm
                <<", iLastLogIndexId: "<<pAction->lastLogEntry()->index()<<", iLastApplied: "<<data.v.iLastApplied);

      if( (pPersistentData->iVotedFor == CSeverNull ||
           pPersistentData->iVotedFor == pAction->candidate()) && true
           //( //message.request()->iLastLogTerm > data.p.iCurrentTerm ||
           // (// message.request()->iLastLogTerm == data.p.iCurrentTerm  <--- log lookup
             //message.request()->iLastLogIndexId >= data.v.iLastApplied))
             ){
               bVoteGranted = true; // par5.2-3
               pPersistentData->iCurrentTerm = pAction->term();
               pPersistentData->iVotedFor    = pAction->candidate();
               pLogFileWriter->syncPersistentData();
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

  if(pAction->granted() && pAction->term() == pLogFileWriter->getPersistentData()->iCurrentTerm){

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
static inline LogEntryId _LogEntryToKey(const LogEntry* pLogEntry){
  return pLogEntry ? pLogEntry->getEntryId() : LogEntryId() ;
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

  LogEntryId keyMatch(_LogEntryToKey(data.pLastLogEntry));
  LogEntryId keyMessageMatch(_FBToLogKey(*pAction->matchLogEntry()));

  if(keyMatch == keyMessageMatch){

    LogEntryId entryId(_FBToLogKey(*pAction->dataLogEntry()));

    if(entryId == keyMessageMatch) // skip heartbeats in log .
      return;

    data.pLastLogEntry = pLogFileWriter->appendEntry(entryId, iEntryDataSize, pSrcData);
    pLogFileWriter->getPersistentData()->iCurrentTerm = entryId.iTerm;
    bResult = true;

  }else if(keyMatch > keyMessageMatch){
    return;
  }

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(pHeader->srcServerId(), data.iMyServerId);

  auto response = FB::CreateAppendLogResponse(builder, pAction->matchLogEntry(),
                              pAction->dataLogEntry(), bResult, pHeader->dstServerId() != 0);

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

  LogIndexType  iIndexId = pAction->dataLogEntry()->index();

  if(pAction->success()){

   for(int iEntryIdx = 0; iEntryIdx < 1; iEntryIdx++){

    for(PendingEntriesList::iterator it = lstPendingEntries.begin(); it != lstPendingEntries.end(); it++){

      PendingEntry& entry = *it;

      LogEntryId entryId(entry.pEntry->getEntryId());

      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Match for commit:"<<entryId.iTerm<<","<<entryId.iIndex);

      if(entryId.iIndex <= iIndexId)
        entry.iNumConfirmations++;
        IA20_LOG(LogLevel::INSTANCE.isInfo(), "iNumConfirmations: "<<entry.iNumConfirmations);
        if(entry.iNumConfirmations * 2 > data.iNumServers){
          pLogFileWriter->commit(entry.pEntry);
          it = lstPendingEntries.erase(it);
        }
    }

    iIndexId++;
   };

  }

  LogEntryId matchEntryId(_FBToLogKey(*pAction->matchLogEntry()));
  const LogEntry* pServerMatchEntry = data.servers[pHeader->srcServerId()].pMatchEntry;
  LogEntryId serverMatchEntryId(_LogEntryToKey(pServerMatchEntry));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error : "<<pAction->success());
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error : "<<matchEntryId);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error : "<<data.pLastLogEntry->getEntryId());
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error : "<<(void*)data.pLastLogEntry);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error : "<<serverMatchEntryId<<", ptr: "<<(void*)pServerMatchEntry);

  if(pAction->success()){

    while(serverMatchEntryId <= matchEntryId){

        pServerMatchEntry = pLogFileWriter->getNextOrNull(pServerMatchEntry);

        IA20_CHECK_IF_NULL(pServerMatchEntry); //There should be a data item we have just confirmed.

        data.servers[pHeader->srcServerId()].pMatchEntry = pServerMatchEntry;

        if(pServerMatchEntry != data.pLastLogEntry && pAction->resend()){
          resendDirect(pHeader->srcServerId());
        }

        serverMatchEntryId = pServerMatchEntry->getEntryId();
    }

  }else{

    IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Handling error 1 ");

    if(serverMatchEntryId == matchEntryId){
      pServerMatchEntry = pServerMatchEntry->getPrevOrNull();
      if(pServerMatchEntry)
        data.servers[pHeader->srcServerId()].pMatchEntry = pServerMatchEntry;
    }

    resendDirect(pHeader->srcServerId());
  }
}
/*************************************************************************/
void RaftEngine::resendDirect(ServerIdType iServerId){

   const LogEntry* pServerMatchEntry = data.servers[iServerId].pMatchEntry;

  if(!pServerMatchEntry)
    return;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: resendDirect, match: "<<pServerMatchEntry->getEntryId());

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(iServerId, data.iMyServerId);

  FB::LogEntryId matchLogEntry(_LogKeyToFB(pServerMatchEntry));
  const LogEntry *pDataEntry = pServerMatchEntry->next();
  FB::LogEntryId dataLogEntry(_LogKeyToFB(pDataEntry));

  auto entryData = builder.CreateVector<uint8_t>(reinterpret_cast<const uint8_t*>(pDataEntry->getData()), pDataEntry->getEntryDataSize());

  auto request = FB::CreateAppendLogRequest(builder, data.iMyServerId, &matchLogEntry, &dataLogEntry, data.v.iCommitIndex,  entryData);
  builder.Finish(FB::CreateMessage(builder, &header, FB::Action_AppendLogRequest, request.Union()));

  IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(builder.GetBufferPointer(), FB::MessageTypeTable()));

  Packet packet(builder);
  pSender->send(packet);

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

  pLogFileWriter->getPersistentData()->iCurrentTerm++;
  pLogFileWriter->getPersistentData()->iVotedFor = data.iMyServerId;
  pLogFileWriter->syncPersistentData();
  data.v.iVoteCount = 1;

  flatbuffers::FlatBufferBuilder builder;

  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId lastEntryId(_LogKeyToFB(data.pLastLogEntry));

  auto request = FB::CreateVoteRequest(builder, pLogFileWriter->getPersistentData()->iCurrentTerm, data.iMyServerId, &lastEntryId);
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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: I am a new leader, term: "<<pLogFileWriter->getPersistentData()->iCurrentTerm);

  data.v.iLastApplied = 0;

  for(int i=1; i <= data.iNumServers; i++){
    data.servers[i].pMatchEntry = data.pLastLogEntry;
  };

  lstPendingEntries.clear();

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId matchEntryId(_LogKeyToFB(data.pLastLogEntry));

  LogEntryId entryId(pLogFileWriter->getPersistentData()->iCurrentTerm,++data.v.iLastApplied);
  data.pLastLogEntry = pLogFileWriter->appendEntry(entryId);

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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Heartbeat, term: "<<pLogFileWriter->getPersistentData());

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

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: New entry, term: "<<pLogFileWriter->getPersistentData()->iCurrentTerm<<", sz: "<<iEntrySize);

  flatbuffers::FlatBufferBuilder builder;
  FB::Header header(0, data.iMyServerId);

  FB::LogEntryId matchLogEntry(_LogKeyToFB(data.pLastLogEntry));

  LogEntryId entryId(pLogFileWriter->getPersistentData()->iCurrentTerm,++data.v.iLastApplied);
  data.pLastLogEntry = pLogFileWriter->appendEntry(entryId, iEntrySize, pEntryData);

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
