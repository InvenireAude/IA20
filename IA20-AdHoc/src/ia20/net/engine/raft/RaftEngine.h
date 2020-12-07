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

namespace IA20 {
namespace Net {
namespace Conn {
namespace MCast {
  class Socket;
}
}
namespace Engine {
namespace Raft {

/*************************************************************************/
/** The RaftEngine class.
 *
 */
class RaftEngine {
public:

	virtual ~RaftEngine() throw();


  typedef uint32_t IndexType;
  typedef uint16_t TermType;
  typedef uint8_t  ServerIdType;

  static const int CMaxServers = 255;
  static const int CSeverNull  = 255;

  struct Message {

  };

  class Sender {
    public:
    virtual void send(const Message* pMessage) = 0;
  };

	RaftEngine(Sender* pSender);

protected:

  Sender* pSender;

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

  class Data {
    PersistentData p;
    VolatileData   v;
    ServerData     servers[CMaxServers];
  };

/*****************************************************************************/
};

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_RaftEngine_H_ */
