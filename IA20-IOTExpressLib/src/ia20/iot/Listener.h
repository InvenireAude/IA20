/*
 * File: Listener.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Listener_H_
#define _IA20_IOT_Listener_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/tools/TasksRing.h>
#include <ia20/iot/tools/OffsetPtr.h>
#include <ia20/iot/tools/IdentifiedByHandle.h>

#include "MessageStore.h"
#include "Connection.h"

namespace IA20 {
namespace IOT {
namespace MQTT {
  class Message;
}
namespace Memory{
  class SharableMemoryPool;
}
/*************************************************************************/
/** The Listener class.
 *
 */
class Listener {
public:

  typedef Tools::OffsetPtr<0> MessageOffsetType;

  class Task {
    public:

    enum Command : uint8_t {
      CA_None             = 0,
      CA_InputMsg         = 3,
      CA_SendDirect       = 4,
      CA_SetContent       = 5,
      CA_SendShared       = 6
    };

    inline void setMessage(uint8_t* pMessage){
     //IA20_LOG(true, "Message s at:"<<(void*)pMessage<<" "<<(void*)this);
      offsetToMessage.set(pMessage);
    }

    inline uint8_t* getMessage(){

     // IA20_LOG(true, "Message g at:"<<(void*)this<<" "<<offsetToMessage.get<uint8_t>());

      if(!offsetToMessage)
        IA20_THROW(InternalException("No MQTT::Message set in the listener task."));

      return offsetToMessage.get<uint8_t>();
    }

    inline Task(Command iCommand):
      iCommand(iCommand),
      iContentUsageCount(0),
      iReferenceId(0),
      aConnectionHandle(0L),
      aMessageHandle(0x55555555){};

    inline Task(Command iCommand, const Task& o):
      iCommand(iCommand),
      iContentUsageCount(0),
      iReferenceId(o.iReferenceId),
      aConnectionHandle(o.aConnectionHandle),
      aMessageHandle(o.aMessageHandle){};


    inline Command getCommand()const{
     // IA20_LOG(true,"command addr:"<<(void*)&iCommand);
      return iCommand;
    }

    long iMessageId;

    void setConnectionHandle(Connection::HandleType aConnectionHandle){
      this->aConnectionHandle = aConnectionHandle;
    }

    Connection::HandleType getConnectionHandle()const{
      return aConnectionHandle;
    }

    void setMessageHandle(Message::HandleType aMessageHandle){
      this->aMessageHandle = aMessageHandle;

            //           IA20_LOG(true, "set message handle:  "
            // <<(void*)(long)this->aMessageHandle);

    }

    Message::HandleType getMessageHandle()const{

    // IA20_LOG(true,"message addr:"<<(void*)&aMessageHandle);
    //                 IA20_LOG(true, "get message handle:  "
    //       <<(void*)(long)this->aMessageHandle);
      return aMessageHandle;
    }


    void setContentUsageCount(uint32_t iContentUsageCount){
      this->iContentUsageCount = iContentUsageCount;
    }

    uint32_t getContentUsageCount()const{
      return iContentUsageCount;
    }

    void setReferenceId(uint64_t iReferenceId){
      this->iReferenceId = iReferenceId;
    }

    uint64_t getReferenceId()const{
      return iReferenceId;
    }

  protected:

    Command iCommand;
    uint64_t                              iReferenceId;

    Connection::HandleType                aConnectionHandle;
    Message::HandleType                   aMessageHandle;
    uint32_t                              iContentUsageCount;

    MessageOffsetType                     offsetToMessage;
  };


  typedef IA20::IOT::Tools::TasksRing<Task> RingType;

	virtual ~Listener() throw();

  Listener(std::unique_ptr<RingType::Interface>&& ptrInterface,
           Memory::SharableMemoryPool* pMemoryPool);

  inline RingType::Interface* getInterface()const{
    return ptrInterface.get();
  }

  inline Memory::SharableMemoryPool *getMemoryPool()const{
    return pMemoryPool;
  }

protected:

  std::unique_ptr<RingType::Interface> ptrInterface;

  Memory::SharableMemoryPool* pMemoryPool;
};
/*************************************************************************/
}
}

#endif /* _IA20_IOT_Listener_H_ */
