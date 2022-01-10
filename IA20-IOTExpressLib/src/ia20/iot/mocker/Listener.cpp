/*
 * File: Listener.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Listener.h"

#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/mqtt/HeaderReader.h>

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
  aConnectionHandle = 999999;
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


  
   IA20_LOG(true, "*** Mocker got    ");
  //  IA20_LOG(true, "HEX:  "<<(void*)ptrTask.get()<<" "<<MiscTools::BinarytoHex((void*)ptrTask.get(), sizeof(Listener::Task)));

   IA20_LOG(true, "Command:           "<<(int)ptrTask->getCommand());


   switch(ptrTask->getCommand()){

     case Task::CA_SendDirect :
        {
          uint8_t *pMessage = ptrTask->getMessage();
          MQTT::HeaderReader headerReader(pMessage);
    
          IA20_LOG(true, "* CA_SendDirect    ");
          IA20_LOG(true, "Connection Handle: "<<(void*)(long)ptrTask->getConnectionHandle());
          IA20_LOG(true, "MessageType:       "<<(int)headerReader.getType());
          IA20_LOG(true, "Length:            "<<headerReader.getLength());

          if(headerReader.getType() == MQTT::Message::MT_CONNACK){
            aConnectionHandle = ptrTask->getConnectionHandle();          
          }
        }
    
     break;
 
     case Task::CA_SetContent :
        {
          uint8_t *pMessage = ptrTask->getMessage();
          // IA20_LOG(true, "CA_SetContent 1:  "<<(void*)(long)ptrTask->getMessageHandle());
          // IA20_LOG(true, "CA_SetContent 2:  "<<(void*)(long)ptrTask->getConnectionHandle());

          Memory::StreamBufferList sbl(ptrTask->getMessage());
          Memory::StreamBufferList::Reader reader(sbl);
          MQTT::HeaderReader headerReader (reader);
    
          IA20_LOG(true, "* CA_SetContent    "<<(void*)pMessage);
          IA20_LOG(true, "ContentUsageCount: "<<ptrTask->getContentUsageCount());
          IA20_LOG(true, "Message Handle:    "<<(void*)(long)ptrTask->getMessageHandle());
          IA20_LOG(true, "MessageType:       "<<(int)headerReader.getType());
          IA20_LOG(true, "Length:            "<<headerReader.getLength());
        }
     break;

     case Task::CA_SendShared :
        IA20_LOG(true, "* CA_SendShared    ");
        IA20_LOG(true, "Connection Handle: "<<(void*)(long)ptrTask->getConnectionHandle());
        IA20_LOG(true, "Messagee Handle:   "<<(void*)(long)ptrTask->getMessageHandle());
     break;

   }



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
    new(pMemoryPool->allocate<Task>(0))Task(Listener::Task::CA_InputMsg), pMemoryPool->getDeleter());

  // if(!theMessage){
  //   theMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get())) MQTT::Message(strHex);
  // }

//  MQTT::Message* pMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get()))MQTT::Message();;
 
  IA20_LOG(true, "*** Mocker send: "<<strHex);

  Memory::StreamBufferList sbl(pMemoryPool, ptrTask.get());
  //TODO helper StringToSBL

  int iMessageLen = strHex.length()/2;

  std::unique_ptr<uint8_t> ptr(new uint8_t[iMessageLen]);
  
  MiscTools::HexToBinary(strHex, ptr.get(), iMessageLen);

  Memory::StreamBufferList::Writer writer(sbl);
  int i=0;

  while(i < iMessageLen){
    IA20_LOG(true, "A1");
    writer.next(32);
    IA20_LOG(true, "B1");
    int iLength = (iMessageLen - i < writer.getAvailableLength()) ?  iMessageLen - i : writer.getAvailableLength();
    IA20_LOG(true, "C: "<<iLength<<", "<<iMessageLen);
    IA20_LOG(true, "Writing: "<<iLength<<",     "<<(void*)writer.getCursor());
  //  IA20_LOG(true, "Writing: "<<iLength<<" bytes, to: "<<(void*)writer.getCursor());
    memcpy(writer.getCursor(), ptr.get() + i ,iLength);
    IA20_LOG(true, "Data: "<<MiscTools::BinarytoHex(writer.getCursor(), iLength));
    writer.addData(iLength);
    i += iLength;
  }

  ptrTask->setMessage(sbl.getHead());
  ptrTask->setConnectionHandle(aConnectionHandle);
  static int iMessageId = 0;

  ptrTask->iMessageId = ++iMessageId;

  IA20_LOG(true, "Task set msg: "<<(void*)ptrTask->getMessage());
  
  
  ptrInterface->getRequests()->enque(ptrTask.release());

}
/*************************************************************************/
}
}
}
