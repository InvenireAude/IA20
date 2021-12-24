/*
 * File: Listener.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Listener.h"

#include <ia20/iot/mqtt/Message.h>

#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>

#include <string.h>

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
Listener::Listener(std::unique_ptr<RingType::Interface>&& ptrInterface):
  IOT::Listener(std::move(ptrInterface), NULL),
  ptrMemoryPoolHolder(new Memory::SharableMemoryPool){
	IA20_TRACER;

  pMemoryPool = ptrMemoryPoolHolder.get();
  
}

/*************************************************************************/
Listener::~Listener() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Listener::serve(){
	IA20_TRACER;

  static int icount = 0;

  Memory::SharableMemoryPool::unique_ptr<Task> ptrTask(
    ptrInterface->getResponses()->deque(), pMemoryPool->getDeleter());

  MQTT::Message *pMessage = ptrTask->getMessage();
  
  //IA20_LOG(true, "Response: "<<pMessage->iMessageId<<" "<<icount++);
}
/*************************************************************************/
void Listener::run(){
	IA20_TRACER;

  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);
  CPU_SET(2, &cpuset);
  int rc1 = pthread_setaffinity_np(pthread_self(),sizeof(cpu_set_t), &cpuset);


  SYS::Signal::ThreadRegistration tr;
  Thread::Cancellation ca(true);


  while(!SYS::Signal::GetInstance()->isStopping()){
    
  //   IA20_LOG(true, "Waiting ...");
    serve();
    
  }

}
/*************************************************************************/
void Listener::sendMessage(const String& strHex){
  IA20_TRACER;

  static MQTT::Message *theMessage = NULL;

  Memory::SharableMemoryPool::unique_ptr<Task> ptrTask(
    new(pMemoryPool->allocate<Task>(0))Task(Listener::Task::CA_ReceiveMQTT), pMemoryPool->getDeleter());

  // if(!theMessage){
  //   theMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get())) MQTT::Message(strHex);
  // }

  MQTT::Message* pMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get()))MQTT::Message();;
  ptrTask->setMessage(pMessage);

//  IA20_LOG(true, "Send1: "<<(void*)pMessage);

  Memory::StreamBufferList sbl(pMemoryPool, pMessage);
  //TODO helper StringToSBL

  int iMessageLen = strHex.length()/2;
  static std::unique_ptr<uint8_t> ptr(new uint8_t[iMessageLen]);
  if(ptr.get()[0] == 0)
    MiscTools::HexToBinary(strHex, ptr.get(), iMessageLen);

  Memory::StreamBufferList::Writer writer(sbl);
  int i=0;

  while(i < iMessageLen){
    writer.next(32);
    int iLength = (iMessageLen - i < writer.getAvailableLength()) ?  iMessageLen - i : writer.getAvailableLength();
    //IA20_LOG(true, "Writing: "<<iLength<<" bytes.");
    memcpy(writer.getCursor(), ptr.get() + i ,iLength);
    writer.addData(iLength);
    i += iLength;
  }

  static int iMessageId = 0;
  pMessage->iMessageId = ++iMessageId;

  //IA20_LOG(true, "Task set: "<<(void*)ptrTask.get());
  
  ptrInterface->getRequests()->enque(ptrTask.release());


}
/*************************************************************************/
}
}
}
