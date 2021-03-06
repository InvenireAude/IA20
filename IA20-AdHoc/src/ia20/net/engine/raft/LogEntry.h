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

	inline LogEntry(const LogEntryId&    entryId,
                  const LogEntry   *pPrevLogEntry,
                  LogEntrySizeType  iEntryDataSize,
                  const void* pSrcData):
    entryId(entryId),
    iEntryDataSize(iEntryDataSize),
    bCommited(false){

      void *pDstData = reinterpret_cast<uint8_t*>(this + 1);

      if(iEntryDataSize){
        memcpy(pDstData, pSrcData, iEntryDataSize);
      }

      iCheckSum = computeCheckSum();
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: LogEntry, at: "<<*this);


      if(pPrevLogEntry){
        iPrevOffset = reinterpret_cast<uint8_t*>(this) - reinterpret_cast<const uint8_t*>(pPrevLogEntry);
      }else{
        iPrevOffset = 0;
      }
    };


  inline const void *getData()const{
    return reinterpret_cast<const uint8_t*>(this + 1);
  }

  inline const LogEntry *getPrevOrNull()const{

    IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: LogEntry, getPrev: "<<(void*)this);
    IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: LogEntry, getPrev: "<<iPrevOffset);

     if(!iPrevOffset)
        return NULL;

     const uint8_t* pAddress = reinterpret_cast<const uint8_t*>(this);
     pAddress -= iPrevOffset;
     return reinterpret_cast<const LogEntry*>(pAddress);
  }

  inline LogEntry *next(){

    /* align to 8 bytes (64bits) */
    void *pNextData = reinterpret_cast<uint64_t*>(this + 1) +
              ((iEntryDataSize + 7) >> 3) ;

    return reinterpret_cast<LogEntry*>(pNextData);
  }

  inline const LogEntry* next()const{
    return const_cast<LogEntry*>(this)->next();
  }

  inline static LogEntrySizeType ComputeSpace(LogEntrySizeType  iEntryDataSize){
    return sizeof(LogEntry) + (iEntryDataSize + 7) && 0xfffffff8 ;
  }

  inline const LogEntryId& getEntryId()const{
    return entryId;
  }

  // inline TermType getTerm()const{
  //   return iTerm;
  // }

  // inline IndexType getIndex()const{
  //   return keyiIndex;
  // }

  inline LogEntrySizeType  getEntryDataSize()const{
    return iEntryDataSize;
  }

  //This is only for diagnostic, use getPrevOrNull() instead.
  inline PrevOffsetType getPrevOffset()const{
    return iPrevOffset;
  }


  inline uint32_t computeCheckSum()const{

      uint32_t iCheckSum = 0x0f0f0f0f;
      iCheckSum ^= entryId.iTerm;
      iCheckSum ^= entryId.iIndex;

      const uint32_t *pData = reinterpret_cast<const uint32_t*>(this + 1);
      uint32_t iDataLeft = iEntryDataSize;

      while(iDataLeft >= 4){
        iCheckSum ^= *pData++;
        iDataLeft -= 4;
      }

      if(iDataLeft > 0){
        const uint8_t *pData8 = reinterpret_cast<const uint8_t*>(pData);
        while(iDataLeft > 0){
          iCheckSum ^= (0x000000ff & *pData8++);
          iDataLeft--;
        }
      }

      return iCheckSum;
  }

  inline uint32_t getCheckSum()const{
    return iCheckSum;
  }

  inline bool isValid()const{
    return iCheckSum == computeCheckSum();
  }

  inline void commit(){
    //TODO ? recompute check sum ?? bCommited in check sum ??
    bCommited = true;
  }

  inline bool isCommited()const{
    return bCommited;
  }

protected:

  LogEntryId            entryId;
  LogEntrySizeType  iEntryDataSize;
  CheckSumType      iCheckSum;
  PrevOffsetType    iPrevOffset;
  uint8_t           bCommited;
  uint8_t           _pad[3];
  // TODO _pading uint32_t - align ?

  friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
};


std::ostream& operator<<(std::ostream& os, const LogEntryId& id);
/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogEntry_H_ */
