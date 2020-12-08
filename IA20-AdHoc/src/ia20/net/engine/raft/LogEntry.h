/*
 * File: LogEntry.h
 *
 * Copyright (C) 2020, Invenire Aude, Albert Krzymowski
 *

 */


#ifndef _IA20_Net_Engine_Raft_LogEntry_H_
#define _IA20_Net_Engine_Raft_LogEntry_H_

#include <ia20/commonlib/commonlib.h>

#include "Definitions.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
/** The LogEntry class.
 *
 */
class LogEntry {
public:

	inline LogEntry(TermType  iTerm,
                  IndexType iIndex,
                  LogEntrySizeType  iEntryDataSize,
                  const void* pSrcData):
    iTerm(iTerm),
    iIndex(iIndex),
    iEntryDataSize(iEntryDataSize){

      void *pDstData = reinterpret_cast<uint8_t*>(this + 1);
      memcpy(pDstData, pSrcData, iEntryDataSize);

      //TODO checksum ..
    };

  inline LogEntry *next(){
    void *pNextData = reinterpret_cast<uint8_t*>(this + 1) + iEntryDataSize;
    return reinterpret_cast<LogEntry*>(pNextData);
  }

  inline static LogEntrySizeType ComputeSpace(LogEntrySizeType  iEntryDataSize){
    return sizeof(LogEntry) + iEntryDataSize;
  };

  inline TermType getTerm()const{
    return iTerm;
  };

  inline IndexType getIndex()const{
    return iIndex;
  };

  inline LogEntrySizeType  getEntryDataSize()const{
    return iEntryDataSize;
  };

  inline uint32_t getCheckSum()const{
    return iCheckSum;
  };

protected:

  TermType          iTerm;
  IndexType         iIndex;
  LogEntrySizeType  iEntryDataSize;
  uint32_t          iCheckSum;

  // TODO _pading uint32_t - align ?

};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogEntry_H_ */
