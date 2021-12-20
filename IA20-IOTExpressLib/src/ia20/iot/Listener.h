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

/*************************************************************************/
/** The Listener class.
 *
 */
class Listener {
public:

  class Task {
    public:
    MQTT::Message *pMessage;
  };

  typedef IA20::IOT::Tools::TasksRing<Task> RingType;

	virtual ~Listener() throw();

  Listener(std::unique_ptr<RingType::Interface>&& ptrInterface);

  inline RingType::Interface* getInterface()const{
    return ptrInterface.get();
  }

protected:

  std::unique_ptr<RingType::Interface> ptrInterface;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Listener_H_ */
