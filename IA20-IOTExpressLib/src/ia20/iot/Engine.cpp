/*
 * File: Engine.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Engine.h"

#include<string.h>

//#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>
//#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/tools/StringRef.h>


#include "ConnectionsStore.h"
#include "TopicsStore.h"
#include "Subscription.h"
#include "SubscriptionsStore.h"
#include "ActionsStore.h"
#include "Topic.h"

namespace IA20 {
namespace IOT {

/*************************************************************************/
Engine::Engine(Listener*  pListener){
	
  IA20_TRACER;

  IA20_CHECK_IF_NULL(pListener);

  addListener(pListener);

  ptrConnectionsStore.reset(new ConnectionsStore());
  ptrTopicsStore.reset(new TopicsStore());
  ptrSubscriptionsStore.reset(
    new SubscriptionsStore(ptrConnectionsStore.get(), ptrTopicsStore.get()));
  
  ptrActionsStore.reset(new ActionsStore());

  ptrActivityStore.reset(new ActivityStore());
  
}
/*************************************************************************/
Engine::~Engine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Engine::serve(){
  IA20_TRACER;

  for(auto it : tabListners){
    serveLister(it);
  }

}
/*************************************************************************/
Engine::Context::Context(Listener::Task* pTask, Memory::SharableMemoryPool::Deleter& deleter):
  ptrTask(pTask, deleter),
  sbl(ptrTask->getMessage()),
  reader(sbl),
  headerReader(reader){
 IA20_TRACER;

  IA20_LOG(true, "MessageType:"<<(int)headerReader.getType());
  IA20_LOG(true, "Length:"<<headerReader.getLength());

}
/*************************************************************************/
void Engine::serveLister(Engine::ListenerDetails& ld){

  IA20_TRACER;
  IA20_LOG(true, "Serve Listener ");

  Context ctx(ld.pRingRequest->deque(), ld.pMemoryPool->getDeleter());

  switch (ctx.headerReader.getType()){

    case MQTT::Message::MT_PUBLISH:
      handlePublish(ld, ctx);
    break;

    case MQTT::Message::MT_SUBSCRIBE:
      handleSubscribe(ld, ctx);
    break;

    case MQTT::Message::MT_CONNECT:
      handleConnect(ld, ctx);
    break;

    default:
      break;
  }


} 
/*************************************************************************/
void Engine::addListener(Listener* pListener){

  ListenerDetails ld;
  ld.pListener = pListener;
  ld.pRingRequest  = pListener->getInterface()->getRequests();
  ld.pRingResponse = pListener->getInterface()->getResponses();
  ld.pMemoryPool = pListener->getMemoryPool();

  tabListners.push_back(ld);
}
/*************************************************************************/
 void Engine::handleConnect(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(true, "Server CONNECT");

   uint8_t iProtNameLen = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
   
   if(iProtNameLen != 4)
    IA20_THROW(InternalException("Create exception for MQTT Parser errors 1 !!!")<<iProtNameLen);

    ctx.reader.copy(buf, iProtNameLen);
    IA20_LOG(true, "Prot name len: "<<(int)iProtNameLen);

   if(memcmp("MQTT",buf,4) != 0)
    IA20_THROW(InternalException("Create exception for MQTT Parser errors 2 !!!"));

    uint8_t iProtoVersion = ctx.reader.readByte();
    IA20_LOG(true, "ProtoVersion: "<<(void*)(long)iProtoVersion);

    uint8_t iConnectionFlags = ctx.reader.readByte();
    IA20_LOG(true, "Flags: "<<(void*)(long)iConnectionFlags);

    uint16_t iKeepAlive = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
    IA20_LOG(true, "KeepAlive: "<<iKeepAlive);

    // TODo utility check number of available bytes
    // uint32_t iPropertiesLen = Tools::MQTT::decodeVL(ctx.reader.getData());

    uint32_t iPropertiesLen = ctx.reader.readByte();
    IA20_LOG(true, "PropertiesLen: "<<(int)iPropertiesLen);

    Connection* pConnection = ptrConnectionsStore->create(1, (wchar_t*)"CLIENT10");

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendMQTT), ld.pMemoryPool->getDeleter());

  ptrTask2->setHandle(pConnection->getHandle());

  uint8_t *pData2 = (uint8_t *)ld.pMemoryPool->allocate(ptrTask2.get(),120);
  
  ptrTask2->setMessage(pData2);
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;
  
  MQTT::FixedHeaderBuilder builder;
  
  builder.setType(MQTT::Message::MT_CONNACK);

  builder.setID(ctx.headerReader.getID());
  
  builder.build(pData2);
  
  ld.pRingResponse->enque(ptrTask2.release());

 }
/*************************************************************************/
 void Engine::handleSubscribe(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

    IA20_LOG(true, "Server SUBSCRIBE, len: "<<ctx.headerReader.getLength());

    int iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);

    IA20_LOG(true, "Packet Identfier: "<<(int)iPacketIdentfier);

      //TODO version check 
     uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
    IA20_LOG(true, "PropertiesLen: "<<(int)iPropertiesLen);


    while(ctx.reader.hasData()){
       int iTopicLen = MQTT::HeaderReader::ReadTwoBytes(ctx.reader); 
      //int iTopicLen = MQTT::HeaderReader::DecodeVL(ctx.reader);

      IA20_LOG(true, "Topic Len: "<<(int)iTopicLen);
      static uint8_t buf[64000]; //TODO more elegant way.
      int rc = ctx.reader.copy(buf, iTopicLen);
      uint8_t iOptions = ctx.reader.readByte();
      IA20_LOG(true, "Topic ["<<iTopicLen<<":"<<rc<<"]"
        <<String((char*)buf,iTopicLen)<<"("<<(int)iOptions<<")");
      Tools::StringRef strTopic(buf,iTopicLen);
      ptrSubscriptionsStore->addSubscription(0, strTopic, iOptions);
    }
    
    Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendMQTT), ld.pMemoryPool->getDeleter());

  uint8_t *pData2 = (uint8_t *)ld.pMemoryPool->allocate(ptrTask2.get(),120);
  
  ptrTask2->setMessage(pData2);
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;
  
  MQTT::FixedHeaderBuilder builder;
  
  builder.setType(MQTT::Message::MT_SUBACK);

  builder.setID(ctx.headerReader.getID());
  
  builder.build(pData2);
  
  ld.pRingResponse->enque(ptrTask2.release());

 }
/*************************************************************************/
 void Engine::handlePublish(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(true, "Server PUBLISH, len: "<<ctx.headerReader.getLength());

   uint8_t iQoS = ctx.headerReader.getQoS();
   IA20_LOG(true, "Packet QoS: "<<(int)iQoS);

   int iTopicLen = MQTT::HeaderReader::ReadTwoBytes(ctx.reader); 
  
   IA20_LOG(true, "Topic Len: "<<(int)iTopicLen);
    
   static uint8_t buf[64000]; //TODO more elegant way.
   int rc = ctx.reader.copy(buf, iTopicLen); 
 
  IA20_LOG(true, "Topic ["<<iTopicLen<<":"<<rc<<"]"
        <<String((char*)buf,iTopicLen));

  Tools::StringRef strTopic(buf,iTopicLen);

  int iPacketIdentfier = 666;
  
  if(iQoS >= 1){
   iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
  }
  
  IA20_LOG(true, "Packet Identfier: "<<(int)iPacketIdentfier);

  //TODO version check 
  uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
  IA20_LOG(true, "PropertiesLen: "<<(int)iPropertiesLen);

  uint32_t iDataLen = ctx.headerReader.getTotalLength();

  Memory::StreamBufferList::Reader reader(ctx.sbl);

  const ActionsStore::Action* pAction = ptrActionsStore->createAction(reader, iDataLen);

	Tools::WordTokens tokens;
	tokens.read(strTopic, ptrTopicsStore->getWordsMap());

  Topic* pCurrent = ptrTopicsStore->getRoot();

	for(Tools::WordTokens::const_iterator it = tokens.begin(); it != tokens.end(); ++it){
		pCurrent = ptrTopicsStore->lookup(pCurrent, *it);
	}

  if(pCurrent->hasFirstSubscription()){
    Subscription* pSubscription = pCurrent->getFirstSubscription();
   
    while(pSubscription){
       ptrActivityStore->createActivity(pSubscription->getHandle(), pAction->getHandle());
       pSubscription = pSubscription->getNext();
    }
  }

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
  ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendMQTT), ld.pMemoryPool->getDeleter());

  uint8_t *pData2 = (uint8_t *)ld.pMemoryPool->allocate(ptrTask2.get(),120);
  
  ptrTask2->setMessage(pData2);
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;
  
  MQTT::FixedHeaderBuilder builder;
  
  builder.setType(MQTT::Message::MT_PUBACK);

  builder.setID(ctx.headerReader.getID());
  
  builder.build(pData2);
  
  ld.pRingResponse->enque(ptrTask2.release());
 }
/*************************************************************************/
}
}
