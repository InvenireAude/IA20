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
class LogFile  {
public:

  inline size_t getSequenceId()const{
    return iSequenceId;
  }

  static String CreateFileName(const String& strPath, ServerIdType iMyServerId, size_t iFileIdx);

protected:

  static const size_t CNotAllocatedSequenceId = 0xffffffffffffffff;

  LogFile(size_t iSequenceId = CNotAllocatedSequenceId);

  size_t  iSequenceId;

  static const int CTagLength = 4;
  static const char *CTag;

  struct MetaData {
    char      sTag[CTagLength];
    uint8_t   bUsed;
    uint8_t   _pad[3];
    size_t    iSequenceId;
    size_t    iFileSize;
    Timestamp tsStarted;
  };

  static inline void* GetDataStart(void* pMemory){
    return reinterpret_cast<MetaData*>(pMemory) + 1;
  };

  static inline const void* GetDataStart(const void* pMemory){
    return reinterpret_cast<const MetaData*>(pMemory) + 1;
  };

  std::unique_ptr<SharedMemoryFile> ptrSHM;
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFile_H_ */
