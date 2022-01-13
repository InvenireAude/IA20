/*
 * File: Engine.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Engine.h"

#include<string.h>


#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/tools/StringRef.h>
#include <ia20/iot/mqtt/PropertiesComposer.h>

#include "ConnectionsStore.h"
#include "TopicsStore.h"
#include "Subscription.h"
#include "SubscriptionsStore.h"
#include "MessageStore.h"
#include "Topic.h"

#include <ia20/iot/logger/LogLevel.h>

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

  ptrMessageStore.reset(new MessageStore());

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

  handleActivities();
}
/*************************************************************************/
Engine::Context::Context(Listener::Task* pTask, Memory::SharableMemoryPool::Deleter& deleter):
  ptrTask(pTask, deleter),
  sbl(ptrTask->getMessage()),
  reader(sbl),
  headerReader(reader){
 IA20_TRACER;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MessageType:"<<(int)headerReader.getType());
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Length:"<<headerReader.getLength());
}
/*************************************************************************/
void Engine::serveLister(Engine::ListenerDetails& ld){

  IA20_TRACER;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ************************");
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " **** Serve Listener ****");
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ************************");

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
  ld.iIdx = tabListners.size();
  ld.pListener = pListener;
  ld.pRingRequest  = pListener->getInterface()->getRequests();
  ld.pRingResponse = pListener->getInterface()->getResponses();
  ld.pMemoryPool = pListener->getMemoryPool();

  tabListners.push_back(ld);
}
/*************************************************************************/
 void Engine::handleConnect(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server CONNECT");

   uint8_t iProtNameLen = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);

   if(iProtNameLen != 4)
    IA20_THROW(InternalException("Create exception for MQTT Parser errors 1 !!!")<<iProtNameLen);

    ctx.reader.copy(buf, iProtNameLen);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Prot name len: "<<(int)iProtNameLen);

   if(memcmp("MQTT",buf,4) != 0)
    IA20_THROW(InternalException("Create exception for MQTT Parser errors 2 !!!"));

    MQTT::Message::VersionType iProtoVersion = (MQTT::Message::VersionType)ctx.reader.readByte();
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "ProtoVersion: "<<(void*)(long)iProtoVersion);

    uint8_t iConnectionFlags = ctx.reader.readByte();
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Flags: "<<(void*)(long)iConnectionFlags);

    uint16_t iKeepAlive = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "KeepAlive: "<<iKeepAlive);

    // TODo utility check number of available bytes
    // uint32_t iPropertiesLen = Tools::MQTT::decodeVL(ctx.reader.getData());

    uint32_t iPropertiesLen = ctx.reader.readByte();
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "PropertiesLen: "<<(int)iPropertiesLen);

    Connection* pConnection = ptrConnectionsStore->create(ld.iIdx, nullptr);
    pConnection->setVersion(iProtoVersion);

    Memory::SharableMemoryPool::unique_ptr<Listener::Task>
        ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
            Listener::Task(Listener::Task::CA_SendDirect, *ctx.ptrTask), ld.pMemoryPool->getDeleter());

  ptrTask2->setConnectionHandle(pConnection->getHandle());
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  static const uint8_t* CResponse = (const uint8_t*)"\000\000";

  MQTT::FixedHeaderBuilder builder;

  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  MQTT::PropertiesComposer propertiesComposer;

  builder.setType(MQTT::Message::MT_CONNACK);
  builder.setID(ctx.headerReader.getID());
  
  uint16_t iProp22Value = MQTT::hostToNet(0x000A);
  uint16_t iProp21Value = MQTT::hostToNet(0x0014);

  propertiesComposer.add(0x21,(const uint8_t*)&iProp21Value, 2);
  propertiesComposer.add(0x22,(const uint8_t*)&iProp22Value, 2);
  
  const Tools::StringRef strClientId(pConnection->getClientId());

  propertiesComposer.add(0x12, strClientId);

  builder.setLength(2 + propertiesComposer.computeLength());
  builder.build(writer);
  writer.writeByte(0);
  writer.writeByte(0);
  propertiesComposer.build(writer);

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "builder: "<<propertiesComposer.computeLength()<<" "<<(void*)sbl.getHead()<<", "<<MiscTools::BinarytoHex(sbl.getHead()+16, propertiesComposer.computeLength()));
  ptrTask2->setMessage(sbl.getHead());

  ld.pRingResponse->enque(ptrTask2.release());

 }
/*************************************************************************/
 void Engine::handleSubscribe(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server SUBSCRIBE, len: "<<ctx.headerReader.getLength());

    int iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);

    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet Identfier: "<<(int)iPacketIdentfier);

    Connection* pConnection = ptrConnectionsStore->get(ctx.ptrTask->getConnectionHandle());
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MQTT version: "<<(int)pConnection->getMQTTVersion());

    if(pConnection->getMQTTVersion() >= 5){
      uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "PropertiesLen: "<<(int)iPropertiesLen);
    }

    while(ctx.reader.hasData()){

       int  iTopicLen =  MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
      
      static uint8_t buf[64000]; //TODO more elegant way.

      int rc = ctx.reader.copy(buf, iTopicLen);
      uint8_t iOptions = ctx.reader.readByte();

      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Topic ["<<iTopicLen<<":"<<rc<<"]"<<String((char*)buf,iTopicLen)<<"("<<(int)iOptions<<")");

      Tools::StringRef strTopic(buf,iTopicLen);
      ptrSubscriptionsStore->addSubscription(ctx.ptrTask->getConnectionHandle(), strTopic, iOptions);
    }

    Memory::SharableMemoryPool::unique_ptr<Listener::Task>
    ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendDirect), ld.pMemoryPool->getDeleter());


  ptrTask2->setConnectionHandle(ctx.ptrTask->getConnectionHandle());
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  MQTT::FixedHeaderBuilder builder;

  builder.setType(MQTT::Message::MT_SUBACK);

  builder.setID(ctx.headerReader.getID());
  builder.setLength(6);

  uint8_t buf2[100];
  uint8_t* pEnd = builder.build(buf2);
  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  writer.write(buf2,pEnd - buf2);
  writer.write((uint8_t*)"\000",1);
  writer.write((uint8_t*)"\000\000\000",3);

  ptrTask2->setMessage(sbl.getHead());

  ld.pRingResponse->enque(ptrTask2.release());

 }
/*************************************************************************/
 void Engine::handlePublish(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server PUBLISH, len: "<<ctx.headerReader.getLength());

   uint8_t iQoS = ctx.headerReader.getQoS();
   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet QoS: "<<(int)iQoS);
   
   int iTopicLen =  MQTT::HeaderReader::ReadTwoBytes(ctx.reader);

   static uint8_t buf[64000]; //TODO more elegant way.
   int rc = ctx.reader.copy(buf, iTopicLen);

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Topic ["<<iTopicLen<<":"<<rc<<"]"<<String((char*)buf,iTopicLen));

  Tools::StringRef strTopic(buf,iTopicLen);

  int iPacketIdentfier = 666;

  if(iQoS >= 1){
   iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
  }

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet Identfier: "<<(int)iPacketIdentfier);

  Connection* pConnection = ptrConnectionsStore->get(ctx.ptrTask->getConnectionHandle());
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MQTT version: "<<(int)pConnection->getMQTTVersion());

  if(pConnection->getMQTTVersion() >= 5){
    uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "PropertiesLen: "<<(int)iPropertiesLen);
  }
  
  uint32_t iDataLen = ctx.headerReader.getTotalLength() - ctx.reader.getConsumedBytes();

  const Message* pMessage = ptrMessageStore->createMessage(ctx.reader, iDataLen, ctx.headerReader.getQoS());

	Tools::WordTokens tokens;
	tokens.read(strTopic, ptrTopicsStore->getWordsMap());

  Topic* pCurrent = ptrTopicsStore->getRoot();

	for(Tools::WordTokens::const_iterator it = tokens.begin(); it != tokens.end(); ++it){
		pCurrent = ptrTopicsStore->lookup(pCurrent, *it);
	}

  for(auto& l : tabListners){
    l.tmp.iCounter = 0;
  }

  if(pCurrent->hasFirstSubscription()){
    Subscription* pSubscription = pCurrent->getFirstSubscription();

    while(pSubscription){

       ptrActivityStore->createActivity(pSubscription->getHandle(), pMessage->getHandle());

       tabListners[pConnection->getListenerIdx()].tmp.iCounter++;

       pSubscription = pSubscription->getNext();
    }

  for(auto& l : tabListners)
    if(l.tmp.iCounter > 0){

      Memory::SharableMemoryPool::unique_ptr<Listener::Task>
          ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
              Listener::Task(Listener::Task::CA_SetContent), ld.pMemoryPool->getDeleter());

      uint8_t *pData2 = (uint8_t *)ld.pMemoryPool->allocate(ptrTask2.get(),120);

      Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
      Memory::StreamBufferList::Writer writer(sbl);

      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Write, set_content: "<<(int)pMessage->getDataLength());

      writer.write(pMessage->getData(), pMessage->getDataLength());
      ptrTask2->setMessage(sbl.getHead());
      ptrTask2->setMessageHandle(pMessage->getHandle());
      ptrTask2->setContentUsageCount(l.tmp.iCounter);
      ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 90000000;
      l.pRingResponse->enque(ptrTask2.release());
    }

  }

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Create a reply");

  Memory::SharableMemoryPool::unique_ptr<Listener::Task>
  ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendDirect), ld.pMemoryPool->getDeleter());


  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  MQTT::FixedHeaderBuilder builder;

  builder.setType(MQTT::Message::MT_PUBACK);
  ptrTask2->setConnectionHandle(ctx.ptrTask->getConnectionHandle());
  builder.setID(ctx.headerReader.getID());

  uint8_t buf2[100];
  uint8_t* pEnd = builder.build(buf2);
  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  writer.write(buf2,pEnd - buf2);
  ptrTask2->setMessage(sbl.getHead());
  ld.pRingResponse->enque(ptrTask2.release());
 }
/*************************************************************************/
void Engine::handleActivities(){
   IA20_TRACER;


   while(ptrActivityStore->hasActivities()){
     Activity* pActivity = ptrActivityStore->back();

     IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "** handleActivities loop **");

     ListenerDetails& ld(tabListners[0]);

     Memory::SharableMemoryPool::unique_ptr<Listener::Task>
      ptrTask(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendShared), ld.pMemoryPool->getDeleter());

      ptrTask->iMessageId = 20000000;

      Subscription* pSubsciption = ptrSubscriptionsStore->lookup(pActivity->getSubscriptionHandle());
      Connection*   pConnection  = ptrConnectionsStore->get(pSubsciption->getConnectionHandle());
      const Message* pMessage     = ptrMessageStore->lookup(pActivity->getMessageHandle());

      ptrTask->setConnectionHandle(pConnection->getHandle());
      ptrTask->setMessageHandle(pActivity->getMessageHandle());

      MQTT::FixedHeaderBuilder builder;
      Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask.get());
      Memory::StreamBufferList::Writer writer(sbl);
      MQTT::PropertiesComposer propertiesComposer;

      builder.setType(MQTT::Message::MT_PUBLISH);
      builder.setID(pConnection->getNextId());
      builder.setFlags((MQTT::Message::Flag)0, pMessage->getQoS());
      Tools::StringRef strTopic(pSubsciption->getTopic());

      builder.setLength(pMessage->getDataLength() + 2 + (2 + strTopic.getLength()) + propertiesComposer.computeLength());
      builder.build(writer);
      writer.write(strTopic);
      writer.writeTwoBytes(pConnection->getNextId()); //TODO how it works ??
      propertiesComposer.build(writer);

      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "publish: "<<propertiesComposer.computeLength()<<" "<<(void*)sbl.getHead()<<", "<<MiscTools::BinarytoHex(sbl.getHead()+16, propertiesComposer.computeLength()));
      ptrTask->setMessage(sbl.getHead());


      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "serve 1:  "<<(void*)(long)ptrTask->getMessageHandle());
      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "server 2:  "<<(void*)(long)ptrTask->getConnectionHandle());

      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "HEX:  "<<(void*)ptrTask.get()<<" "<<MiscTools::BinarytoHex((void*)ptrTask.get(), sizeof(Listener::Task)));


      ld.pRingResponse->enque(ptrTask.release());

      ptrActivityStore->next();
   }

}
/*************************************************************************/
}
}
