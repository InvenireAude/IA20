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
#include "LogEntry.h"

namespace IA20 {
namespace Net {
namespace Engine {
namespace Raft {

/*************************************************************************/
/** The LogFile class.
 *
 */
class LogFile  {
public:

  static String CreateFileName(const String& strPath, ServerIdType iMyServerId);


  inline const LogEntry* getNextOrNull(const LogEntry* pLogEntry){
    return pLogEntry->next();
  }

  inline LogEntry* getNextOrNull(LogEntry* pLogEntry){
    return pLogEntry->next();
  }

  inline const LogEntry* getPrevOrNull(const LogEntry* pLogEntry){
    return pLogEntry->getPrevOrNull();
  }

  inline const LogEntry* getLastLogEntryOrNull()const{
    return pLastEntry;
  }

  struct PersistentData {

    PersistentData():iCurrentTerm(0),iVotedFor(CSeverNull){};

    TermType     iCurrentTerm;
    ServerIdType iVotedFor;
  };

  inline PersistentData* getPersistentData()const{
    return &pMetaData->data;
  }

  inline void syncPersistentData(){
    SharedMemoryFile::Sync(pMetaData, sizeof(MetaData));
  };

protected:

  LogFile(const String& strFileName);

  static const int CTagLength = 4;
  static const char *CTag;

  struct MetaData {
    char      sTag[CTagLength];
    uint8_t   bUsed;
    uint8_t   _pad[3];
    size_t    iFileSize;
    Timestamp      tsStarted;
    PersistentData data;
  };

  static inline void* GetDataStart(void* pMemory){
    return reinterpret_cast<MetaData*>(pMemory) + 1;
  };

  static inline const void* GetDataStart(const void* pMemory){
    return reinterpret_cast<const MetaData*>(pMemory) + 1;
  };

  String  strFileName;
  void*   pMemory;

  LogEntry *pFirstEntry;
  LogEntry *pLastEntry;

  size_t    iSize;
  MetaData *pMetaData;

  std::unique_ptr<SharedMemoryFile> ptrSHM;
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogFile_H_ */
