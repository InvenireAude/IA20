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

class Message {
public:
 public:
    
    inline const uint8_t* getData()const{
      return reinterpret_cast<const uint8_t*>(this+1);
    }

    typedef uint32_t HandleType;

    inline HandleType getHandle()const{
      return iHandle;
    }

    inline uint32_t getDataLength()const{
      return iDataLength;
    }

    inline Message(HandleType iHandle, uint32_t iDataLength):
        iHandle(iHandle),
        iDataLength(iDataLength){};

    protected:

    HandleType iHandle;
    uint32_t   iDataLength;
  
    inline uint8_t* getDataPointer(){
      return reinterpret_cast<uint8_t*>(const_cast<Message*>(this+1));
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
