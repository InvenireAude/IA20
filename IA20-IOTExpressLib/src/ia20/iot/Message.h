/*
 * File: Message.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Message_H_
#define _IA20_IOT_Message_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Message class.
 *
 */
class MessageStore;
class Topic;

class Message {
public:
 public:
    
    inline const uint8_t* getData()const{
      return reinterpret_cast<const uint8_t*>(this+1);
    }

    typedef uint32_t HandleType;
    static const uint32_t CNullHandle = 0xffffffff;

    inline HandleType getHandle()const{
      return iHandle;
    }

    inline uint32_t getDataLength()const{
      return iDataLength;
    }

    inline Message(HandleType iHandle, 
                   uint32_t iDataLength, 
                   uint8_t iQoS):
        iHandle(iHandle),
        iDataLength(iDataLength),
        iQoS(iQoS){}

    inline uint8_t getQoS()const{
      return iQoS;
    }
    
    inline bool getRetentionFlag()const{
      return bRetentionFlag;
    }

    inline void setRetentionFlag(bool bRetentionFlag){
      this->bRetentionFlag = bRetentionFlag;
    }

    inline void incUsageCount(){
      iUsageCount++;
    }

    inline uint32_t decUsageCount(){
      return iUsageCount--;
    }

    inline uint32_t getUsageCount()const{
      return iUsageCount;
    }

    protected:

    HandleType iHandle;
    uint32_t   iDataLength;
    uint32_t   iUsageCount;
    uint8_t    iQoS;
    bool       bRetentionFlag;

    inline uint8_t* getDataPointer(){
      return reinterpret_cast<uint8_t*>(const_cast<Message*>(this + 1));
    }

    static const size_t CAlignedSize;

    static size_t ComputeRequiredMemory(uint32_t iDataLen){
      return CAlignedSize + iDataLen;
    };

    friend class MessageStore;
  };

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Message_H_ */
