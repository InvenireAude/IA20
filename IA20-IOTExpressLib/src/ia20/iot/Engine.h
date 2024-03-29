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
    
    int                                 iIdx;
    Listener*                           pListener;
    std::unique_ptr<Listener::PortType> ptrServerPort;
    Memory::SharableMemoryPool*         pMemoryPool;

    union {
      uint32_t iCounter;  
    } tmp;

  };


public:

	virtual ~Engine() throw();

  Engine();

  void serve();  

  URing::RingHandler* getRingHandler()const{
    return ptrRingHandler.get();
  }

  void addListener(Listener* pListener);

protected:

  typedef std::vector<ListenerDetails> ListenerVector;
  ListenerVector tabListners;

  void serveLister(ListenerDetails& ld);

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
  void handlePing(Engine::ListenerDetails& ld, Context& ctx);

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

  std::unique_ptr<URing::RingHandler> 		  ptrRingHandler;

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


  struct Stats{
    
    uint64_t tNumMessagesByType[MQTT::Message::MT_NUM_TYPES];
    uint64_t iNumPublicationsByQoS[3];    
    uint64_t iNumListenerInCmds;
    uint64_t iNumListenerOutCmds;
    uint64_t iNumFullConnections;

    Stats(){ 
      reset();
    }

    inline void reset(){
      
      IA20_LOG(true, "size: "<<sizeof(Stats));

      for(int i=0; i<MQTT::Message::MT_NUM_TYPES; i++)
        tNumMessagesByType[i] = 0L;
      
      for(int i=0; i<MQTT::Message::MQOS_NUM_TYPES; i++)  
        iNumPublicationsByQoS[i] = 0L;

     iNumListenerInCmds = iNumListenerOutCmds = 0L;
     iNumFullConnections = 0L;
    }

    friend std::ostream& operator<<(std::ostream& os, const Stats& s){
      
      for(int i=0; i<MQTT::Message::MT_NUM_TYPES; i++)
        os<<"\t NumMessagesByType["<<i<<"]:\t"<<s.tNumMessagesByType[i]<<std::endl;

      for(int i=0; i<MQTT::Message::MQOS_NUM_TYPES; i++)  
        os<<"\t NumPublicationsByQoS["<<i<<"]:\t"<<s.iNumPublicationsByQoS[i]<<std::endl;

      os<<"\t NumListenerInCmds:  \t"<<s.iNumListenerInCmds<<std::endl;
      os<<"\t NumListenerOutCmds: \t"<<s.iNumListenerOutCmds<<std::endl;
      os<<"\t NumFullConnections: \t"<<s.iNumFullConnections<<std::endl;
  
      os<<std::endl;

      return os;
    }

  };

  void serveStats();

  Stats stats;
};

/*************************************************************************/
}
}

#endif /* _IA20_IOT_Engine_H_ */
