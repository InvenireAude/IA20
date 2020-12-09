/*
 * File: Logger.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_Mocker_Logger_H_
#define _IA20_Net_Engine_Raft_Mocker_Logger_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/net/conn/mcast/Sender.h>
#include <ia20/net/conn/mcast/Receiver.h>

#include "../RaftEngine.h"

#include <list>

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {
namespace Mocker {

/*************************************************************************/
/** The Logger class.
 *
 */
class Logger :
  public RaftEngine::Logger {
public:

	virtual ~Logger() throw();

  virtual const LogEntry* appendEntry(const LogEntryId& entryId,
                                      LogEntrySizeType  iEntryDataSize = 0,
                                      const void* pSrcData = 0);

  virtual void commit(const LogEntry* pLogEntry);

  const LogEntry* getLastCommit()const{
    return pLastCommit;
  }

	Logger(size_t iMemorySize = 1000000);

  void add(RaftEngine* pEngine);

  void simpleDump(std::ostream& os);

protected:

  typedef std::list<RaftEngine*> RaftEnginesList;
  RaftEnginesList lstRaftEngines;

  size_t iMemorySize;
  size_t iSpaceLeft;

  void *pMemory;
  void *pNextEntry;
  const LogEntry *pLastCommit;
  const LogEntry *pLastEntry;

};
/*************************************************************************/
}
}
}
}
}

#endif /* _IA20_Net_Engine_Raft_Mocker_Logger_H_ */
