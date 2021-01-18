/*
 * File: RaftEngine.cpp
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *
 */

#include "RaftEngine.h"


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
  data.pLastCommitLogEntry = pLogFileWriter->commit(data.pLastLogEntry);

  data.iMyServerId = iMyServerId;
  data.iNumServers = iNumServers;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: setup, servers: "<<(int)data.iMyServerId<<" / "<<(int)data.iNumServers);

  data.iState = ST_Follower;
  tsElapsed.start();
}
/*************************************************************************/
RaftEngine::~RaftEngine() throw(){
	IA20_TRACER;
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

  if(pAction->term() < pPersistentData->iCurrentTerm ||
     data.iState == ST_Leader  // ????
     ){
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[1]");
      bVoteGranted = false;
  }else{
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Vote[2]: votedFor: "<<(int)pPersistentData->iVotedFor
                <<", iLastLogTerm: "<<pAction->lastLogEntry()->term()<<", iCurrentTerm: "<<pPersistentData->iCurrentTerm
                <<", iLastLogIndexId: "<<pAction->lastLogEntry()->index()<<", iLastApplied: "<<data.pLastLogEntry->getEntryId());

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


  FBMessageSender<FB::VoteResponse, FB::Action_VoteResponse> sender(this, pHeader->srcServerId());
  sender.setAction(&FB::CreateVoteResponse,pAction->term(), pAction->candidate(), bVoteGranted);
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
void RaftEngine::checkLeaderChange(TermType iNewDataTerm){

 IA20_LOG(LogLevel::INSTANCE.isInfo(), "Check if there is a leader change: state: "<<data.iState<<", check: "<<
          iNewDataTerm<<" ? "<<pLogFileWriter->getPersistentData()->iCurrentTerm);

  if(data.iState == ST_Candidate && false){

    if(iNewDataTerm >= pLogFileWriter->getPersistentData()->iCurrentTerm){
      convertToFollower(iNewDataTerm, false);
    }

  }else if(data.iState == ST_Leader){

    if(iNewDataTerm > pLogFileWriter->getPersistentData()->iCurrentTerm){
      convertToFollower(iNewDataTerm, true);
    }

  }
}
/*************************************************************************/
void RaftEngine::onMessage(const FB::Header* pHeader, const FB::AppendLogRequest* pAction){
  IA20_TRACER;

  checkLeaderChange(pAction->dataLogEntry()->term());

   IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: state:"<<data.iState);


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

  const LogEntry *pMatchingEntry = data.pLastLogEntry;
  LogEntryId keyMatch(_LogEntryToKey(pMatchingEntry));
  LogEntryId keyMessageMatch(_FBToLogKey(*pAction->matchLogEntry()));

  LogEntryId entryId(_FBToLogKey(*pAction->dataLogEntry()));

  if(entryId == keyMessageMatch){ // skip heartbeats in log .
      data.pLastCommitLogEntry = pLogFileWriter->commit(_FBToLogKey(*pAction->leaderCommitEntry()));
      return;
    }

 if(keyMatch == keyMessageMatch){

    data.pLastLogEntry = pLogFileWriter->appendEntry(entryId, iEntryDataSize, pSrcData);
    pLogFileWriter->getPersistentData()->iVotedFor    = CSeverNull;
    pLogFileWriter->getPersistentData()->iCurrentTerm = entryId.iTerm;

    data.pLastCommitLogEntry = pLogFileWriter->commit(_FBToLogKey(*pAction->leaderCommitEntry()));

    bResult = true;

  }else if(keyMatch > keyMessageMatch){

    // The first entry [0,0] is commited and acts as a guard.

    IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft:: rewinding ...");

    while(keyMatch > keyMessageMatch && !pMatchingEntry->isCommited()){
      pMatchingEntry = pMatchingEntry->getPrevOrNull();
      keyMatch =_LogEntryToKey(pMatchingEntry);
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft:: trace back to: "<<keyMatch<<" ?? "<<keyMessageMatch);
    }

    if(keyMatch == keyMessageMatch){

      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft:: has match: "<<keyMatch);

      const LogEntry *pNewMatchData = pMatchingEntry->getPrevOrNull();

      bResult = true;

      if(pNewMatchData && pNewMatchData->getEntryId() == entryId){
        IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft:: ignore");
        return; // we have this entry already - ignore ?
      }else{
        pLogFileWriter->rewind(pMatchingEntry);
        data.pLastLogEntry = pLogFileWriter->appendEntry(entryId, iEntryDataSize, pSrcData);
      }

    }else{
      return;
    }

  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft:: bResult: "<<bResult);

  FBMessageSender<FB::AppendLogResponse, FB::Action_AppendLogResponse> sender(this, pHeader->srcServerId());
  sender.setAction(&FB::CreateAppendLogResponse, pAction->matchLogEntry(),
                              pAction->dataLogEntry(), bResult, pHeader->dstServerId() != 0);

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

      if(entryId.iIndex == iIndexId)
        entry.iNumConfirmations++;
        IA20_LOG(LogLevel::INSTANCE.isInfo(), "iNumConfirmations: "<<entry.iNumConfirmations);
        if(entry.iNumConfirmations * 2 > data.iNumServers){
          data.pLastCommitLogEntry = pLogFileWriter->commit(entry.pEntry);
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

        IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: loop ?"<<serverMatchEntryId<<" "<<matchEntryId);
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
void RaftEngine::resendDirect(ServerIdType iServerId)const{

   const LogEntry* pServerMatchEntry = data.servers[iServerId].pMatchEntry;

  if(!pServerMatchEntry)
    return;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: resendDirect, match: "<<pServerMatchEntry->getEntryId());

  const LogEntry *pDataEntry = pServerMatchEntry->next();
  sendLogEntryImpl(pServerMatchEntry, pDataEntry, iServerId);
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
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Last log entry : "<<data.pLastLogEntry->getEntryId());
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: Current Term   : "<<pLogFileWriter->getPersistentData()->iCurrentTerm);
  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: *** onPacket : "<<
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

  const FB::LogEntryId lastEntryId(_LogKeyToFB(data.pLastLogEntry));

  FBMessageSender<FB::VoteRequest, FB::Action_VoteRequest> sender(this);

  sender.setAction(&FB::CreateVoteRequest, pLogFileWriter->getPersistentData()->iCurrentTerm, data.iMyServerId, &lastEntryId);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::convertToLeader(){
  IA20_TRACER;

  data.iState = ST_Leader;

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: I am a new leader, term: "<<pLogFileWriter->getPersistentData()->iCurrentTerm);

  for(int i=1; i <= data.iNumServers; i++){
    data.servers[i].pMatchEntry = data.pLastLogEntry;
  };

  lstPendingEntries.clear();

  const LogEntry* pMatchEntry = data.pLastLogEntry;

  LogEntryId entryId(pLogFileWriter->getPersistentData()->iCurrentTerm, 1);
  data.pLastLogEntry = pLogFileWriter->appendEntry(entryId);

  lstPendingEntries.push_back({ pEntry : data.pLastLogEntry, iNumConfirmations : 1 });

  sendLogEntryImpl(pMatchEntry, data.pLastLogEntry);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::sendHeartbeat(){
  IA20_TRACER;

  if(data.iState != ST_Leader){
    return;
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: Heartbeat, term: "<<pLogFileWriter->getPersistentData());

  sendLogEntryImpl(data.pLastLogEntry);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::convertToFollower(TermType iCurrentTerm, bool bRewind){
  IA20_TRACER;

  pLogFileWriter->getPersistentData()->iCurrentTerm = iCurrentTerm;
  pLogFileWriter->getPersistentData()->iVotedFor = CSeverNull;
  pLogFileWriter->syncPersistentData();
  data.iState = ST_Follower;

  if(bRewind){
    pLogFileWriter->rewind(data.pLastCommitLogEntry);
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "I am a follower, now.");
}
/*************************************************************************/
void RaftEngine::onData(void *pEntryData, LogEntrySizeType iEntrySize){
  IA20_TRACER;

  if(data.iState != ST_Leader){
    return;
  }

  IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft["<<data.iMyServerId<<"]:: New entry, term: "<<pLogFileWriter->getPersistentData()->iCurrentTerm<<", sz: "<<iEntrySize);

  const LogEntry* pMatchEntry = data.pLastLogEntry;

  LogEntryId entryId(pLogFileWriter->getPersistentData()->iCurrentTerm,
                     data.pLastLogEntry->getEntryId().iIndex + 1);

  data.pLastLogEntry = pLogFileWriter->appendEntry(entryId, iEntrySize, pEntryData);

  lstPendingEntries.push_back({ pEntry : data.pLastLogEntry, iNumConfirmations : 1});

  sendLogEntryImpl(pMatchEntry, data.pLastLogEntry);

  tsElapsed.start();
}
/*************************************************************************/
void RaftEngine::sendLogEntryImpl(const LogEntry* pMatchEntry, const LogEntry* pDataEntry, ServerIdType iServerId)const{
  IA20_TRACER;


  FBMessageSender<FB::AppendLogRequest, FB::Action_AppendLogRequest> sender(this, iServerId);

  const FB::LogEntryId matchLogEntry(_LogKeyToFB(pMatchEntry));
  const FB::LogEntryId dataLogEntry(_LogKeyToFB(pDataEntry ? pDataEntry : pMatchEntry));
  const FB::LogEntryId commitLogEntry(_LogKeyToFB(data.pLastCommitLogEntry));

  auto entryData = pDataEntry && pDataEntry->getEntryDataSize() ?
        sender.CreateVector<uint8_t>(reinterpret_cast<const uint8_t*>(pDataEntry->getData()), pDataEntry->getEntryDataSize()) : 0;

  sender.setAction(&FB::CreateAppendLogRequest, data.iMyServerId, &matchLogEntry, &dataLogEntry, &commitLogEntry,  entryData);

}
/*************************************************************************/
}
}
}
}
