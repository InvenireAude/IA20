/*
 * File: RaftEngine.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Definitions_H_
#define _IA20_Net_Engine_Raft_Definitions_H_

#include <ia20/commonlib/commonlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <iostream>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

  typedef uint32_t IndexType;
  typedef uint32_t TermType;
  typedef uint16_t ServerIdType;
  typedef uint32_t LogIndexType;
  typedef uint32_t LogEntrySizeType;
  typedef uint32_t CheckSumType;
  typedef uint32_t PrevOffsetType;

  struct LogEntryId {

    TermType  iTerm;
    IndexType iIndex;

    inline LogEntryId(const LogEntryId& other):
        iTerm(other.iTerm),iIndex(other.iIndex){};

    inline LogEntryId(TermType  iTerm = 0, IndexType iIndex = 0):
        iTerm(iTerm),iIndex(iIndex){};

    inline bool operator==(const LogEntryId& other){
      return iTerm == other.iTerm && iIndex == other.iIndex;
    }

    inline bool operator!=(const LogEntryId& other){
      return !(iTerm == other.iTerm && iIndex == other.iIndex);
    }

    inline bool operator<(const LogEntryId& other){
      return iTerm < other.iTerm || (iTerm == other.iTerm && iIndex < other.iIndex);
    }

    inline bool operator<=(const LogEntryId& other){
      return iTerm < other.iTerm || (iTerm == other.iTerm && iIndex <= other.iIndex);
    }

    inline bool operator>(const LogEntryId& other){
      return iTerm > other.iTerm || (iTerm == other.iTerm && iIndex > other.iIndex);
    }

   inline bool operator>=(const LogEntryId& other){
      return iTerm > other.iTerm || (iTerm == other.iTerm && iIndex >= other.iIndex);
    }

  };

  static const int CMaxServers = 1024;
  static const int CSeverNull  = 9999;

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Definitions_H_ */
