/*
 * File: LogFileWriter.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_LogFileWriter_H_
#define _IA20_Net_Engine_Raft_LogFileWriter_H_

#include <ia20/commonlib/commonlib.h>

#include "Definitions.h"
#include "LogFile.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogEntry;

/*************************************************************************/
/** The LogFileWriter class.
 *
 */
class LogFileWriter :
  public LogFile,
  public DoubleLinkedList<LogFileWriter> {
public:

	virtual ~LogFileWriter() throw();

	LogFileWriter(const String& strFileName, size_t iSequenceId);

  inline size_t getSpaceLeft()const{
    return iSpaceLeft;
  };

  void commit(const LogEntry* pLogEntry);

  const LogEntry* appendEntry(TermType  iTerm,
                              IndexType iIndex,
                              LogEntrySizeType  iEntryDataSize,
                              const void* pSrcData);

  inline size_t getSequenceId()const{
    return iSequenceId;
  }

  static String CreateFileName(const String& strPath, ServerIdType iMyServerId, size_t iFileIdx);

  inline const LogEntry* getLastLogEntryOrNull()const{
    return pLastEntry;
  }

protected:

  String  strFileName;
  size_t  iSpaceLeft;

  size_t  iSequenceId;
  void*   pMemory;
  void*   pNextEntry;

  const LogEntry *pLastEntry;

  std::unique_ptr<SharedMemoryFile> ptrSHM;
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFileWriter_H_ */
