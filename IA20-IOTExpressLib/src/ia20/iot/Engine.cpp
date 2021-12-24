/*
 * File: Engine.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Engine.h"

#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>
#include <ia20/iot/mqtt/HeaderReader.h>
#include <ia20/iot/mqtt/FixedHeaderBuilder.h>

#include<string.h>

namespace IA20 {
namespace IOT {

/*************************************************************************/
Engine::Engine(Listener*  pListener){
	
  IA20_TRACER;

  IA20_CHECK_IF_NULL(pListener);
  addListener(pListener);

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
void Engine::serveLister(Engine::ListenerDetails& ld){

  IA20_TRACER;
  // IA20_LOG(true, "Serve Listener ");

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask(ld.pRingRequest->deque(), ld.pMemoryPool->getDeleter());

  // IA20_LOG(true, "Task get: "<<(void*)ptrTask.get());
  
  uint8_t *pMessage = ptrTask->getMessage();
  
  IA20_LOG(true, "Task get msg: "<<(void*)ptrTask->getMessage());

  // IA20_LOG(true, "Got1: "<<(void*)pMessage);
  // IA20_LOG(true, "Got1: "<<pMessage->iMessageId);
 
  Memory::StreamBufferList sbl(reinterpret_cast<uint8_t*>(pMessage));
  Memory::StreamBufferList::Reader reader(sbl);
  
  uint8_t* pData;
  Memory::StreamBufferList::DataLengthType iDataLength;

  if(!reader.getNext(pData, iDataLength)){
    IA20_LOG(true, "Empty message ?");
  }
  IA20_LOG(true, (void*)pData);
  
  IA20_LOG(true, MiscTools::BinarytoHex(pData,iDataLength));

  MQTT::HeaderReader headerReader(pData);

  IA20_LOG(true, "MessageType:"<<(int)headerReader.getType());
  IA20_LOG(true, "Length:"<<headerReader.getLength());

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask2(new (ld.pMemoryPool->allocate<Listener::Task>(NULL))
        Listener::Task(Listener::Task::CA_SendMQTT), ld.pMemoryPool->getDeleter());

  uint8_t *pData2 = (uint8_t *)ld.pMemoryPool->allocate(ptrTask2.get(),120);
  
  ptrTask2->setMessage(pData2);
  ptrTask2->iMessageId = ptrTask->iMessageId + 10000000;
  
  MQTT::FixedHeaderBuilder builder;
  builder.setType(MQTT::Message::MT_CONNACK);
  builder.setID(headerReader.getID());
  builder.build(pData2);
  ld.pRingResponse->enque(ptrTask2.release());
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
}
}
