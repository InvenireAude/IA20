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
  //IA20_LOG(true, "Serve Listener ");

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask(ld.pRingRequest->deque(), ld.pMemoryPool->getDeleter());

  MQTT::Message *pMessage = reinterpret_cast<MQTT::Message*>(ptrTask.get() + 1);
  //IA20_LOG(true, "Got1: "<<(void*)pMessage);
  //IA20_LOG(true, "Got1: "<<pMessage->iMessageId);
 
  Memory::StreamBufferList sbl(reinterpret_cast<uint8_t*>(pMessage + 1));
  Memory::StreamBufferList::Reader reader(sbl);
  
  int i=0;
  static uint8_t pData[40000];
  uint8_t *pCursor =  pData;
  uint8_t *pFrom;
  uint32_t iLength;
  
  while(reader.getNext(pFrom, iLength)){
    IA20_LOG(false, "Reading: "<<iLength<<" bytes.");
    memcpy(pCursor, pFrom, iLength);
    pCursor += iLength;
  }

//  String strHEX = MiscTools::BinarytoHex(pData, pCursor - pData);
//  IA20_LOG(true, "Message: "<<strHEX);

  Memory::SharableMemoryPool::unique_ptr<Listener::Task> 
    ptrTask2(ld.pMemoryPool->allocate<Listener::Task>(NULL), ld.pMemoryPool->getDeleter());

  MQTT::Message* pMessage2 = new(ld.pMemoryPool->allocate<MQTT::Message>(ptrTask2.get()))MQTT::Message();;
  pMessage2->iMessageId = pMessage->iMessageId + 10000000;

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
