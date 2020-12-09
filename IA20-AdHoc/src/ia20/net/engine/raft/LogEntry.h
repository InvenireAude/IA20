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
    iEntryDataSize(iEntryDataSize),
    bCommited(false){

      void *pDstData = reinterpret_cast<uint8_t*>(this + 1);

      if(iEntryDataSize){
        memcpy(pDstData, pSrcData, iEntryDataSize);
      }

      iCheckSum = computeCheckSum();
      IA20_LOG(LogLevel::INSTANCE.isInfo(), "Raft :: LogEntry, at: "<<*this);
    };


  inline const void *getData()const{
    return reinterpret_cast<const uint8_t*>(this + 1);
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

  inline TermType getTerm()const{
    return iTerm;
  }

  inline IndexType getIndex()const{
    return iIndex;
  }

  inline LogEntrySizeType  getEntryDataSize()const{
    return iEntryDataSize;
  }

  inline uint32_t computeCheckSum()const{

      uint32_t iCheckSum = 0x0f0f0f0f;
      iCheckSum ^= iTerm;
      iCheckSum ^= iIndex;

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

  TermType          iTerm;
  IndexType         iIndex;
  LogEntrySizeType  iEntryDataSize;
  uint32_t          iCheckSum;
  uint8_t           bCommited;
  uint8_t           _pad[3];
  // TODO _pading uint32_t - align ?

  friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
};

/*************************************************************************/

}
}
}
}

#endif /* _IA20_Net_Engine_Raft_LogEntry_H_ */
