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
    
    enum Action : uint8_t {
      CA_None             = 0,
      CA_ReceiveMQTT      = 3,
      CA_SendMQTT         = 4
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
    
    inline Task(Action iAction):iAction(iAction){};

    inline Action getAction()const{
      return iAction;
    }

    long iMessageId;

    void setHandle(Tools::IdentifiedByHandle::HandleType aHandle){
      this->mHandle = aHandle;
    }

    Tools::IdentifiedByHandle::HandleType getHandle()const{
      return mHandle;
    }

  protected:

    Action iAction;
    MessageOffsetType offsetToMessage;
    Tools::IdentifiedByHandle::HandleType mHandle;
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
