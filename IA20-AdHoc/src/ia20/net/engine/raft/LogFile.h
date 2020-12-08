/*
 * File: LogFile.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_LogFile_H_
#define _IA20_Net_Engine_Raft_LogFile_H_

#include <ia20/commonlib/commonlib.h>

#include "Definitions.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

class LogEntry;

/*************************************************************************/
/** The LogFile class.
 *
 */
class LogFile {
public:

	virtual ~LogFile() throw();

	LogFile(const String& strName, size_t iSize);

  inline size_t getSpaceLeft()const{
    return iSpaceLeft;
  };

  void commit(const LogEntry* pLogEntry);

  const LogEntry* appendEntry(TermType  iTerm,
                              IndexType iIndex,
                              LogEntrySizeType  iEntryDataSize,
                              const void* pSrcData);

protected:

  String  strName;
  size_t  iSize;
  size_t  iSpaceLeft;
  void*   pMemory;
  void*   pNext;
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFile_H_ */
