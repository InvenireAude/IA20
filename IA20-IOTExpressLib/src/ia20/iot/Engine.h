/*
 * File: Engine.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Engine_H_
#define _IA20_IOT_Engine_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/memory/SharableMemoryPool.h>

#include <ia20/iot/mqtt/HeaderReader.h>
#include <ia20/iot/mqtt/FixedHeaderBuilder.h>
#include <ia20/iot/mqtt/ContentComposer.h>

#include "Listener.h"
#include "ActivityStore.h"
#include "MessageStore.h"

namespace IA20 {
namespace IOT {


/*************************************************************************/
/** The Engine class.
 *
 */

class ConnectionsStore;
class TopicsStore;
class SubscriptionsStore;
class MessageStore;
class ActivityStore;

class Engine {
  protected:

  struct ListenerDetails {
    
    int                   iIdx;
    Listener*             pListener;
    Listener::RingType*   pRingRequest;
    Listener::RingType*   pRingResponse;
    Memory::SharableMemoryPool* pMemoryPool;

    union {
      uint32_t iCounter;  
    } tmp;

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

  struct Context {

    Context(Listener::Task*, Memory::SharableMemoryPool::Deleter&);

    Memory::SharableMemoryPool::unique_ptr<Listener::Task> ptrTask;
    Memory::StreamBufferList sbl;
    Memory::StreamBufferList::Reader reader;
    MQTT::HeaderReader headerReader;
  };

  void handleConnect(Engine::ListenerDetails& ld, Context& ctx);
  void handleSubscribe(Engine::ListenerDetails& ld, Context& ctx);
  void handlePublish(Engine::ListenerDetails& ld, Context& ctx);

  void handleActivities();
  
  uint8_t buf[1024];
  std::unique_ptr<ConnectionsStore>   ptrConnectionsStore;
  std::unique_ptr<TopicsStore>        ptrTopicsStore;
  std::unique_ptr<SubscriptionsStore> ptrSubscriptionsStore;
  std::unique_ptr<MessageStore>       ptrMessageStore;
  std::unique_ptr<ActivityStore>      ptrActivityStore;
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Engine_H_ */
