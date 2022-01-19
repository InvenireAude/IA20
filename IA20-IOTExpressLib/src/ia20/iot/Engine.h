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
#include "Topic.h"

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
class Message;
class Topic;
class Subscription;

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
  void handlePubAck(Engine::ListenerDetails& ld, Context& ctx);

  void handleActivities();

  void buildAndSendShared(ListenerDetails* pListenerDetails,
                         Subscription*    pSubsciption,
                         Connection*      pConnection,
                         Message*         pMessage,
                         Activity*        pActivity);

  void buildAndSendRetained(ListenerDetails* pListenerDetails,
                            Subscription*    pSubsciption,
                            Connection*      pConnection,
                            Message*         pMessage,
                            Activity*        pActivity);

  uint8_t buf[1024];
  std::unique_ptr<ConnectionsStore>   ptrConnectionsStore;
  std::unique_ptr<TopicsStore>        ptrTopicsStore;
  std::unique_ptr<SubscriptionsStore> ptrSubscriptionsStore;
  std::unique_ptr<MessageStore>       ptrMessageStore;
  std::unique_ptr<ActivityStore>      ptrActivityStore;

  class Publisher : public Topic::Callback {
    public:
      
      inline Publisher(Engine  *pEngine,
                      Topic::FullTopicNameHandle aNameHandle,
                      Message *pMessage,
                      uint8_t  iQoS):
        pMessage(pMessage),
        aNameHandle(aNameHandle),
        pEngine(pEngine),
        iQoS(iQoS){};

      virtual void onSubscription(const Subscription* pSubscription);
    
    protected:
      Message *pMessage;
      Engine  *pEngine;
      Topic::FullTopicNameHandle aNameHandle;
      uint8_t iQoS;
  };

  class RetainedPublisher : public Topic::RetainCallback {
    public:
      
      inline RetainedPublisher(Engine  *pEngine,
                              Subscription *pSubscription,
                              uint8_t  iQoS):
        pSubscription(pSubscription),
        pEngine(pEngine),
        iQoS(iQoS){};

      virtual void onTopic(const Topic* pTopic);
    
    protected:
      Subscription  *pSubscription;
      Engine        *pEngine;
      uint8_t       iQoS;
  };

};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Engine_H_ */
