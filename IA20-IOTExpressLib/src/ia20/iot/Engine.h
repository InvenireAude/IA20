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

namespace Memory{
  class SharableMemoryPool;
}

/*************************************************************************/
/** The Engine class.
 *
 */
class Engine {
  protected:

  struct ListenerDetails {
    
    Listener*             pListener;
    Listener::RingType*   pRingRequest;
    Listener::RingType*   pRingResponse;
    Memory::SharableMemoryPool* pMemoryPool;

  };

public:

	virtual ~Engine() throw();

  Engine(
    Listener*      pListener
   );

  void serve();
  
protected:

  typedef std::vector<ListenerDetails> ListenerVector;
  ListenerVector tabListners;

  void serveLister(ListenerDetails& ld);
  void addListener(Listener* pListener);
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Engine_H_ */
