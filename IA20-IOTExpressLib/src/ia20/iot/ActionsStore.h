/*
 * File: ActionsStore.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_ActionsStore_H_
#define _IA20_IOT_ActionsStore_H_

#include <ia20/commonlib/commonlib.h>

#include <ia20/iot/tools/TasksRing.h>

namespace IA20 {
namespace IOT {
namespace MQTT {
  class Message;
}

/*************************************************************************/
/** The ActionsStore class.
 *
 */
class ActionsStore {
public:

  class Task {
    public:
    MQTT::Message *pMessage;
  };

  typedef IA20::IOT::Tools::TasksRing<Task> RingType;

	virtual ~ActionsStore() throw();

  ActionsStore(std::unique_ptr<RingType::Interface>&& ptrInterface);

  inline RingType::Interface* getInterface()const{
    return ptrInterface.get();
  }

protected:

  std::unique_ptr<RingType::Interface> ptrInterface;

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_ActionsStore_H_ */
