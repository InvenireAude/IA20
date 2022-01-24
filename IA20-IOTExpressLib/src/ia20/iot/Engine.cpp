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
#include "Connection.h"
#include "SubscriptionsStore.h"
#include "MessageStore.h"
#include "Topic.h"

#include <ia20/iot/logger/LogLevel.h>
#include <ia20/iot/tools/sys/TaskPort.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
Engine::Engine(){

  IA20_TRACER;

  ptrConnectionsStore.reset(new ConnectionsStore());
  ptrTopicsStore.reset(new TopicsStore());
  ptrSubscriptionsStore.reset(
    new SubscriptionsStore(ptrConnectionsStore.get()));

  ptrMessageStore.reset(new MessageStore());

  ptrActivityStore.reset(new ActivityStore());

  ptrRingHandler.reset(new URing::RingHandler);

  tabListners.resize(1);
}
/*************************************************************************/
Engine::~Engine() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Engine::serve(){
  IA20_TRACER;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ************************");
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " **** Serve Listeners ***");
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ************************");

  for(auto& it : tabListners){
    it.ptrServerPort->schedule();
  }

  ptrRingHandler->handleAtLeastOnce();

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ************************");

  for(auto& it : tabListners){
    serveLister(it);
  }

  handleActivities();

  for(auto& it : tabListners){
    it.ptrServerPort->flush();
  }

  ptrRingHandler->handle();

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


  Listener::Task *pTask;
  while(ld.ptrServerPort->dequeue(pTask)){

    Context ctx(pTask, ld.pMemoryPool->getDeleter());

    switch (ctx.headerReader.getType()){

      case MQTT::Message::MT_PUBLISH:
        handlePublish(ld, ctx);
      break;

      case MQTT::Message::MT_PUBACK:
        handlePubAck(ld, ctx);
      break;

      case MQTT::Message::MT_PINGREQ:
        handlePing(ld, ctx);
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

}
/*************************************************************************/
void Engine::addListener(Listener* pListener){

	Tools::SYS::PipeFD::PeerType peerServer(pListener->getServerPeer());

  tabListners[0].iIdx = 0;
  tabListners[0].pListener = pListener;
  tabListners[0].ptrServerPort  = Tools::SYS::TaskPort<Listener::Task*>::Create(
      1000, ptrRingHandler.get(), peerServer.first, peerServer.second);

  tabListners[0].pMemoryPool = pListener->getMemoryPool();


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

    MQTT::FixedHeaderBuilder builder;

    Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
    Memory::StreamBufferList::Writer writer(sbl);
    MQTT::PropertiesComposer propertiesComposer;

    builder.setType(MQTT::Message::MT_CONNACK);

    if(iProtoVersion >= MQTT::Message::MV_MQTTv5){

      uint16_t iProp22Value = MQTT::hostToNet(0x000A);
      uint16_t iProp21Value = MQTT::hostToNet(0x0014);
      propertiesComposer.add(0x21,(const uint8_t*)&iProp21Value, 2);
      propertiesComposer.add(0x22,(const uint8_t*)&iProp22Value, 2);
      const Tools::StringRef strClientId(pConnection->getClientId());
      propertiesComposer.add(0x12, strClientId);
      builder.setLength(2 + propertiesComposer.computeLength());

    }else{
      builder.setLength(2);
    }

    builder.build(writer);

    writer.writeByte(0);
    writer.writeByte(0);

    if(iProtoVersion >= MQTT::Message::MV_MQTTv5){
      propertiesComposer.build(writer);
    }

    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "builder: "<<propertiesComposer.computeLength()<<" "<<(void*)sbl.getHead()<<", "<<MiscTools::BinarytoHex(sbl.getHead()+16, propertiesComposer.computeLength()));
    ptrTask2->setMessage(sbl.getHead());

    ld.ptrServerPort->enqueue(ptrTask2.release()); //TODO pass pointer l-ref - memory leak on errors

 }
/*************************************************************************/
void Engine::RetainedPublisher::onTopic(const Topic* pTopic){

 IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Topic has retained: "<<(void*)(long)pTopic->getRetained());

  Connection *pConnection = pEngine->ptrConnectionsStore->get(pSubscription->getConnectionHandle());

  Activity* pActivity = pEngine->ptrActivityStore->createActivity(pSubscription->getHandle(),
                                                          pTopic->getNameHandle(),
                                                          pTopic->getRetained(),
                                                          Listener::Task::CA_SendDirect,
                                                          std::max(iQoS, pConnection->getMaxQoS()));

  Message *pMessage = pEngine->ptrMessageStore->lookup(pTopic->getRetained());

  pEngine->buildAndSendRetained(&pEngine->tabListners[pConnection->getListenerIdx()],
        pSubscription, pConnection, pMessage, pActivity);

}
/*************************************************************************/
 void Engine::handleSubscribe(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server SUBSCRIBE, len: "<<ctx.headerReader.getLength());

   uint8_t iQoS = ctx.headerReader.getQoS();
   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet QoS: "<<(int)iQoS);

    int iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet Identfier: "<<(int)iPacketIdentfier);

    Connection* pConnection = ptrConnectionsStore->get(ctx.ptrTask->getConnectionHandle());
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MQTT version: "<<(int)pConnection->getMQTTVersion());

   if(iQoS >= pConnection->getMaxQoS()){
      iQoS  = pConnection->getMaxQoS();
   }

    if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
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
      Topic* pTopic = ptrTopicsStore->getOrCreateTopic(strTopic);
      Subscription *pSubscription = ptrSubscriptionsStore->addSubscription(ctx.ptrTask->getConnectionHandle(), pTopic, strTopic, iOptions);

      RetainedPublisher publisher(this, pSubscription, iQoS);
      ptrTopicsStore->iterateRetained(strTopic, &publisher);
    }

  Memory::SharableMemoryPool::unique_ptr<Listener::Task>
  ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
      Listener::Task(Listener::Task::CA_SendDirect), ld.pMemoryPool->getDeleter());


  ptrTask2->setConnectionHandle(ctx.ptrTask->getConnectionHandle());
  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  MQTT::FixedHeaderBuilder builder;

  builder.setType(MQTT::Message::MT_SUBACK);

  uint8_t buf2[100];
  uint8_t* pEnd = builder.build(buf2);
  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  MQTT::PropertiesComposer propertiesComposer;

  if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
      builder.setLength(3 + propertiesComposer.computeLength());
  }else{
      builder.setLength(3);
  }

  builder.build(writer);

  writer.writeTwoBytes(iPacketIdentfier);

  if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
    propertiesComposer.build(writer);
  }

  writer.writeByte(iQoS);

  ptrTask2->setMessage(sbl.getHead());

  ld.ptrServerPort->enqueue(ptrTask2.release());

 }

/*************************************************************************/
void Engine::Publisher::onSubscription(const Subscription* pSubscription){

  Connection *pConnection = pEngine->ptrConnectionsStore->get(pSubscription->getConnectionHandle());

  pEngine->ptrActivityStore->createActivity( pSubscription->getHandle(),
                                    aNameHandle,
                                    pMessage->getHandle(),
                                    Listener::Task::CA_SendShared,
                                    std::max(iQoS, pConnection->getMaxQoS()));

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "getListenerIdx: "<<(int)pConnection->getListenerIdx());

  pEngine->tabListners[pConnection->getListenerIdx()].tmp.iCounter++;
  pMessage->incUsageCount();

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

  uint16_t iPacketIdentfier = 0xffff;

  if(iQoS >= 1){
   iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
  }

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet Identfier: "<<(int)iPacketIdentfier);

  Connection* pConnection = ptrConnectionsStore->get(ctx.ptrTask->getConnectionHandle());
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MQTT version: "<<(int)pConnection->getMQTTVersion());

  if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
    uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "PropertiesLen: "<<(int)iPropertiesLen);
  }

  uint32_t iDataLen = ctx.headerReader.getTotalLength() - ctx.reader.getConsumedBytes();

  Message* pMessage = ptrMessageStore->createMessage(ctx.reader, iDataLen, ctx.headerReader.getQoS());

  Topic* pTopic = ptrTopicsStore->getOrCreateTopic(strTopic);

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Topic name:"<<pTopic->getNameHandle());

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Retain message ?");

    if( ctx.headerReader.testFlag(MQTT::Message::MF_RETAIN) ){
      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Retain message");
      if(pTopic->hasRetained()){
        Message* pOldRetained = ptrMessageStore->lookup(pTopic->getRetained());
        pOldRetained->setRetentionFlag(false);
        ptrMessageStore->disposeIfUnused(pOldRetained);
      }
      pTopic->setRetained(pMessage->getHandle());
    }

    for(auto& l : tabListners){
      l.tmp.iCounter = 0;
    }


  Publisher publisher(this, pTopic->getNameHandle(), pMessage, ctx.headerReader.getQoS());
  ptrTopicsStore->iterate(strTopic, &publisher);

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "counter: "<<(int)tabListners[0].tmp.iCounter);

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
        l.ptrServerPort->enqueue(ptrTask2.release());
      }


  if(iQoS == 0){
  //  ptrMessageStore->dispose(pMessage);
    return;
  }

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Create a reply");

  Memory::SharableMemoryPool::unique_ptr<Listener::Task>
  ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendDirect, *ctx.ptrTask), ld.pMemoryPool->getDeleter());


  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  MQTT::FixedHeaderBuilder builder;

  builder.setType(MQTT::Message::MT_PUBACK);
  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  MQTT::PropertiesComposer propertiesComposer;

  if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
      builder.setLength(2 + propertiesComposer.computeLength());
  }else{
      builder.setLength(2);
  }

  builder.build(writer);
  writer.writeTwoBytes(iPacketIdentfier);

  if(pConnection->getMQTTVersion() >= MQTT::Message::MV_MQTTv5){
    propertiesComposer.build(writer);
  }

  ptrTask2->setMessage(sbl.getHead());
  ld.ptrServerPort->enqueue(ptrTask2.release());

 }
 /*************************************************************************/
 void Engine::handlePubAck(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server PUBACK, len: "<<ctx.headerReader.getLength());

   uint16_t iPacketIdentfier = MQTT::HeaderReader::ReadTwoBytes(ctx.reader);
   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Packet Identfier: "<<(int)iPacketIdentfier);

   Connection* pConnection = ptrConnectionsStore->get(ctx.ptrTask->getConnectionHandle());
   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MQTT version: "<<(int)pConnection->getMQTTVersion());

    if(ctx.reader.hasData()){
      uint16_t iReason = ctx.reader.readByte();
      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " Reason: "<<(int)iReason);
    }

  try{
    Activity* pActivity = pConnection->getOutputActivity(iPacketIdentfier);
    pConnection->removeOutputActivity(iPacketIdentfier);
    ptrActivityStore->dispose(pActivity);
  }catch(Exception& e){
   IA20_LOG(IOT::LogLevel::INSTANCE.bIsError, " ERROR: unknow packet identifires in PUBACK"<<e.getInfo());   
  }
  // TODO handle reason  and properties
  // if(pConnection->getMQTTVersion() >= 5){
  //   uint32_t iPropertiesLen = MQTT::HeaderReader::DecodeVL(ctx.reader);
  //   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "PropertiesLen: "<<(int)iPropertiesLen);
  // }

 }

 /*************************************************************************/
 void Engine::handlePing(Engine::ListenerDetails& ld, Context& ctx){
   IA20_TRACER;

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Server PING, len: "<<ctx.headerReader.getLength());

  Memory::SharableMemoryPool::unique_ptr<Listener::Task>
  ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendDirect, *ctx.ptrTask), ld.pMemoryPool->getDeleter());

  ptrTask2->iMessageId = ctx.ptrTask->iMessageId + 10000000;

  MQTT::FixedHeaderBuilder builder;

  builder.setType(MQTT::Message::MT_PINGRESP);
  
  Memory::StreamBufferList sbl(ld.pMemoryPool, ptrTask2.get());
  Memory::StreamBufferList::Writer writer(sbl);
  
  builder.setLength(0);
  builder.build(writer);

  ptrTask2->setMessage(sbl.getHead());
  ld.ptrServerPort->enqueue(ptrTask2.release());

 }
/*************************************************************************/
void Engine::handleActivities(){
   IA20_TRACER;


   while(ptrActivityStore->hasActivities()){
     Activity* pActivity = ptrActivityStore->back();

     IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "** handleActivities loop **");

      ListenerDetails& ld(tabListners[0]);

      Subscription* pSubsciption = ptrSubscriptionsStore->lookup(pActivity->getSubscriptionHandle());
      Connection*   pConnection  = ptrConnectionsStore->get(pSubsciption->getConnectionHandle());
      Message*      pMessage     = ptrMessageStore->lookup(pActivity->getMessageHandle());

      buildAndSendShared(&ld, pSubsciption, pConnection, pMessage, pActivity);

      ptrActivityStore->next();

      if(pActivity->getQoS() == 0){
        ptrActivityStore->dispose(pActivity);
        ptrMessageStore->decUsageAndDespose(pMessage);
      }
   }

}
/*************************************************************************/
void Engine::buildAndSendShared(ListenerDetails* pListenerDetails,
                                Subscription*    pSubsciption,
                                Connection*      pConnection,
                                Message*         pMessage,
                                Activity*        pActivity){
   IA20_TRACER;


     Listener::Task::Command iCommand = pActivity->getCommand();

     Memory::SharableMemoryPool::unique_ptr<Listener::Task>
      ptrTask(new (pListenerDetails->pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(iCommand), pListenerDetails->pMemoryPool->getDeleter());

      ptrTask->iMessageId = 20000000;

      ptrTask->setConnectionHandle(pConnection->getHandle());
      ptrTask->setMessageHandle(pMessage->getHandle());

      MQTT::FixedHeaderBuilder builder;
      Memory::StreamBufferList sbl(pListenerDetails->pMemoryPool, ptrTask.get());
      Memory::StreamBufferList::Writer writer(sbl);
      MQTT::PropertiesComposer propertiesComposer;

      builder.setType(MQTT::Message::MT_PUBLISH);
     // builder.setID(pConnection->getNextId());
      builder.setFlags((MQTT::Message::Flag)0, pMessage->getQoS());

      Topic::FullTopicNameHandle aNameHandle = pActivity->getTopicNameHandle();
      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Name handle :: **"<<(long)aNameHandle);

      Tools::StringRef strTopic(
          aNameHandle != Topic::CNullName ?
          ptrTopicsStore->getFullTopicName(aNameHandle) :
          pSubsciption->getTopic()
        );

      builder.setLength(pMessage->getDataLength() + 2 + (2 + strTopic.getLength()) + propertiesComposer.computeLength());
      builder.build(writer);
      writer.write(strTopic);
      writer.writeTwoBytes(pConnection->addOutputActivity(pActivity));
      propertiesComposer.build(writer);

      if(iCommand == Listener::Task::CA_SendDirect){
        writer.write(pMessage->getData(), pMessage->getDataLength());
      }

      IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "publish: "<<propertiesComposer.computeLength()<<" "<<(void*)sbl.getHead()<<", "<<MiscTools::BinarytoHex(sbl.getHead()+16, propertiesComposer.computeLength()));
      ptrTask->setMessage(sbl.getHead());

      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "serve 1:  "<<(void*)(long)ptrTask->getMessageHandle());
      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "server 2:  "<<(void*)(long)ptrTask->getConnectionHandle());
      // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "HEX:  "<<(void*)ptrTask.get()<<" "<<MiscTools::BinarytoHex((void*)ptrTask.get(), sizeof(Listener::Task)));

      pListenerDetails->ptrServerPort->enqueue(ptrTask.release());

}
/*************************************************************************/
void Engine::buildAndSendRetained(ListenerDetails* pListenerDetails,
                                  Subscription*    pSubsciption,
                                  Connection*      pConnection,
                                  Message*         pMessage,
                                  Activity*        pActivity){
   IA20_TRACER;


    //  Memory::SharableMemoryPool::unique_ptr<Listener::Task>
    //   ptrTask(new (pListenerDetails->pMemoryPool->allocate<Listener::Task>(NULL))
    //     Listener::Task(Listener::Task::CA_SendDirect), pListenerDetails->pMemoryPool->getDeleter());

    //   ptrTask->iMessageId = 20000000;

    //   ptrTask->setConnectionHandle(pConnection->getHandle());
    //   ptrTask->setMessageHandle(pMessage->getHandle());

    //   MQTT::FixedHeaderBuilder builder;
    //   Memory::StreamBufferList sbl(pListenerDetails->pMemoryPool, ptrTask.get());
    //   Memory::StreamBufferList::Writer writer(sbl);
    //   MQTT::PropertiesComposer propertiesComposer;

    //   builder.setType(MQTT::Message::MT_PUBLISH);
    //  // builder.setID(pConnection->getNextId());
    //   builder.setFlags((MQTT::Message::Flag)0, pMessage->getQoS());
    //   Tools::StringRef strTopic(pSubsciption->getTopic());

    //   builder.setLength(pMessage->getDataLength() + 2 + (2 + strTopic.getLength()) + propertiesComposer.computeLength());
    //   builder.build(writer);
    //   writer.write(strTopic);
    //   writer.writeTwoBytes(pConnection->addOutputActivity(pActivity));
    //   propertiesComposer.build(writer);

    //   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "publish on subscribe: "<<propertiesComposer.computeLength()<<" "<<(void*)sbl.getHead()<<", "<<MiscTools::BinarytoHex(sbl.getHead()+16, propertiesComposer.computeLength()));
    //   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "data: "<<MiscTools::BinarytoHex(sbl.getHead()+16, (pMessage->getDataLength() + 2 + (2 + strTopic.getLength()) + propertiesComposer.computeLength())));
    //   ptrTask->setMessage(sbl.getHead());


    //   pListenerDetails->ptrServerPort->enqueue(ptrTask.release());

}
/*************************************************************************/
}
}
