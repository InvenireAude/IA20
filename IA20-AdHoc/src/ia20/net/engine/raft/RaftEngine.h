/*
 * File: RaftEngine.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_RaftEngine_H_
#define _IA20_Net_Engine_Raft_RaftEngine_H_

#include <ia20/commonlib/commonlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#include "Definitions.h"
#include "Packet.h"

#include "fb/rpc_generated.h"

#include <ia20/fb/Helpers.h>


#include <list>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogEntry;
class LogFileWriter;

/*************************************************************************/
/** The RaftEngine class.
 *
 */
class RaftEngine {
public:

	virtual ~RaftEngine() throw();

  class Sender {
    public:
    virtual void send(const Packet& packet) = 0;
  };


	RaftEngine(ServerIdType iMyServerId, ServerIdType iNumServers, LogFileWriter *pLogFileWriter, Sender* pSender);

  void onStart();
  void onMessage();

  void onMessage(const FB::Header* pHeader, const FB::VoteRequest* pAction);
  void onMessage(const FB::Header* pHeader, const FB::VoteResponse* pAction);
  void onMessage(const FB::Header* pHeader, const FB::AppendLogRequest* pAction);
  void onMessage(const FB::Header* pHeader, const FB::AppendLogResponse* pAction);

  void onPacket(const Packet& packet);
  void onTimer();

  void onData(void *pEntryData, LogEntrySizeType iEntrySize);

  void startElection();
  void sendHeartbeat();

protected:

  Sender        *pSender;
  LogFileWriter *pLogFileWriter;

  enum State {
    ST_NONE       = 0,
    ST_Leader     = 1,
    ST_Follower   = 2,
    ST_Candidate  = 3
  };


/*****************************************************************************/

  struct VolatileData {

    //VolatileData():iCommitIndex(0),iLastApplied(0){};

    //IndexType iCommitIndex;
    //IndexType iLastApplied;

    ServerIdType iVoteCount;
  };

  struct ServerData {
    struct sockaddr_in ipAddress;

    ServerData():pMatchEntry(NULL){
      ::bzero(&ipAddress, sizeof(ipAddress));
    }

    const LogEntry* pMatchEntry;
  };

/*****************************************************************************/

  struct PendingEntry{
    const LogEntry *pEntry;
    ServerIdType   iNumConfirmations;
  };

  typedef std::list<PendingEntry> PendingEntriesList;

  PendingEntriesList lstPendingEntries;

/*****************************************************************************/
  struct Data {
    State           iState;

    const LogEntry *pLastLogEntry;
    const LogEntry *pLastCommitLogEntry;

    ServerIdType    iMyServerId;
    VolatileData    v;
    ServerIdType    iNumServers;
    ServerData      servers[CMaxServers];
    bool            votes[CMaxServers];
  };

  Data data;

  static const int CElectionTimeoutNS = 1000000;
  static const int CAppendEntriesTimeoutNS = 10000000;

  TimeSample tsElapsed;

  void resendDirect(ServerIdType iServerId)const;

  void checkLeaderChange(TermType iNewDataTerm);
  void convertToLeader();
  void convertToFollower(TermType iCurrentTerm, bool bRewind);

  void sendLogEntryImpl(const LogEntry* pMatchEntry, const LogEntry* pDataEntry = NULL, ServerIdType iServerId = CSeverBroadcast)const;


  template<class T, FB::Action action_type>
  class FBMessageSender : public flatbuffers::FlatBufferBuilder {

      FB::Header header;
      const RaftEngine* pRaftEngine;


      flatbuffers::Offset<T> action;

    public:
      FBMessageSender(const RaftEngine* pRaftEngine, ServerIdType iDstServerId = 0):
            header(iDstServerId, pRaftEngine->data.iMyServerId),
            pRaftEngine(pRaftEngine){}

     template<class ...Args>
     void setAction(flatbuffers::Offset<T> (*f)(flatbuffers::FlatBufferBuilder& fb, Args...), Args... args){
        action = (*f)((flatbuffers::FlatBufferBuilder&)(*this), args...);
      }

      ~FBMessageSender(){

          if(action.IsNull())
            return;

          Finish(FB::CreateMessage(*this, &header, action_type, action.Union()));
          IA20_LOG(LogLevel::INSTANCE.isInfo(), IA20::FB::Helpers::ToString(GetBufferPointer(), FB::MessageTypeTable()));
          Packet packet(*this);
          pRaftEngine->pSender->send(packet);

      }

    };

/*****************************************************************************/
};

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_RaftEngine_H_ */
