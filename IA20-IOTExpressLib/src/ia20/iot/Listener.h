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

  class Task {
    public:
    
    enum Action {
      CA_None             = 0,
      CA_ReceiveMQTT      = 3,
      CA_SendMQTT         = 4
    };

    Action iAction;
    uint8_t _pad[4];
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
