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
#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
/** The LogFileWriter class.
 *
 */
class LogFileWriter :
  public LogFile {
public:

	virtual ~LogFileWriter() throw();

	LogFileWriter(const String& strFileName, size_t iMemorySize = 0);

  inline size_t getSpaceLeft()const{
    return iSpaceLeft;
  };

  void commit(const LogEntry* pLogEntry);

  const LogEntry* appendEntry(const LogEntryId& entryId,
                              LogEntrySizeType  iEntryDataSize = 0,
                              const void* pSrcData = 0);


protected:

  size_t  iSpaceLeft;

  void*   pNextEntry;

  const LogEntry *pLastCommit;

};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFileWriter_H_ */
