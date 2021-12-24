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

    inline void setMessage(MQTT::Message* pMessage){
     //IA20_LOG(true, "Message s at:"<<(void*)pMessage<<" "<<(void*)this);
      offsetToMessage.set(pMessage);
    }

    inline MQTT::Message* getMessage(){

      //IA20_LOG(true, "Message g at:"<<(void*)this);

      if(!offsetToMessage)
        IA20_THROW(InternalException("No MQTT::Message set in the listener task."));
      
      return offsetToMessage.get<MQTT::Message>();
    }
    
    inline Task(Action iAction):iAction(iAction){};

    inline Action getAction()const{
      return iAction;
    }

  protected:

    Action iAction;
    //padding ??
    MessageOffsetType offsetToMessage;
    
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
