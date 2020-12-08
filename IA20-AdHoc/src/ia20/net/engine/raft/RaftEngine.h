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
#include "Logger.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {


class VoteRequestMessage;
class VoteResponseMessage;
class AppendEntriesRequest;
class AppendEntriesResponse;

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

	RaftEngine(ServerIdType iMyServerId, const Logger::Configuration& cfgLogger, Sender* pSender);

  void onStart();
  void onMessage();

  void onMessage(const VoteRequestMessage& message);
  void onMessage(const VoteResponseMessage& message);
  void onMessage(const AppendEntriesRequest& message);
  void onMessage(const AppendEntriesResponse& message);

  void onPacket(Packet& packet);

  void startElection();

protected:

  Sender* pSender;

  enum State {
    ST_NONE       = 0,
    ST_Leader     = 1,
    ST_Follower   = 2,
    ST_Candidate  = 3
  };


/*****************************************************************************/
  struct PersistentData {

    PersistentData():iTerm(0),iVotedFor(CSeverNull){};

    TermType     iTerm;
    ServerIdType iVotedFor;
  };

  struct VolatileData {

    VolatileData():iCommitIndex(0),iLastApplied(0){};

    IndexType iCommitIndex;
    IndexType iLastApplied;
  };

  struct ServerData {
    struct sockaddr_in ipAddress;

    ServerData():iNextIndex(0),iNextMatch(0){
      ::bzero(&ipAddress, sizeof(ipAddress));
    }

    IndexType iNextIndex;
    IndexType iNextMatch;
  };

/*****************************************************************************/

  struct Data {
    State          iState;
    LogEntry      *pLastLogEntry;
    ServerIdType   iMyServerId;
    PersistentData p;
    VolatileData   v;
    ServerData     servers[CMaxServers];
  };

  Data data;



  std::unique_ptr<Logger> ptrLogger;

/*****************************************************************************/
};

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_RaftEngine_H_ */
