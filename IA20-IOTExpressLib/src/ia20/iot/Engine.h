/*
 * File: Engine.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Engine_H_
#define _IA20_IOT_Engine_H_

#include <ia20/commonlib/commonlib.h>

#include "Listener.h"
#include "ActivityStore.h"
#include "ActionsStore.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
/** The Engine class.
 *
 */
class Engine {
public:

	virtual ~Engine() throw();

  Engine(
    Listener::RingType::Interface*      pListenerInterface,
    ActivityStore::RingType::Interface* pActivityStoreInterface,
    ActionsStore::RingType::Interface*  pActionsStoreInterface
  );

  void serveListener();
  void serveActivityStore();
  void serveActionsStore();

protected:
  Listener::RingType::Interface*      pListenerInterface;
  ActivityStore::RingType::Interface* pActivityStoreInterface;
  ActionsStore::RingType::Interface*  pActionsStoreInterface;

  Listener::RingType* pListenerRingRequest;
  Listener::RingType* pListenerRingResponse;


  ActivityStore::RingType* pActivityStoreRingRequest;
  ActivityStore::RingType* pActivityStoreRingResponse;

  ActionsStore::RingType*  pActionsStoreRingRequest;
  ActionsStore::RingType*  pActionsStoreRingResponse;
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Engine_H_ */
