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

#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Mocker {

/*************************************************************************/
Listener::Listener( int iMaxConnections):
  tabConnections(iMaxConnections),
  ptrMemoryPoolHolder(new Memory::SharableMemoryPool){
    
	IA20_TRACER;

  for(auto& c: tabConnections){
    c.aConnectionHandle = 0x999999;
  }

  setupPort();
}
/*************************************************************************/
Listener::~Listener() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Listener::dump(std::ostream& os){
	IA20_TRACER;

  for(ConnectionEntry& c: tabConnections){
    std::cerr<<"MQTT Connection: "<<c.aConnectionHandle<<std::endl;

    for(const String& s: c.lstWire){
      std::cerr<<s<<std::endl;
    }
  }
}
/*************************************************************************/
bool Listener::serve(bool bWait){
	IA20_TRACER;

  static int icount = 0;

  Task* pTask;
  bool  bGotIt;

  if(!bWait && !(bGotIt = ptrPort->dequeue(pTask)))
    return false;

  if(!bGotIt){
    while( bWait && ptrPort->dequeue(pTask)){}
  }

  Memory::SharableMemoryPool::unique_ptr<Task> ptrTask( pTask, ptrMemoryPoolHolder->getDeleter());

  uint8_t buf[5000];

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "*** Mocker got    ");
  //  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "HEX:  "<<(void*)ptrTask.get()<<" "<<MiscTools::BinarytoHex((void*)ptrTask.get(), sizeof(Listener::Task)));

   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Command:           "<<(int)ptrTask->getCommand());


   switch(ptrTask->getCommand()){

     case Task::CA_SendDirect :
        {
          uint8_t *pMessage = ptrTask->getMessage();
          Memory::StreamBufferList sbl(pMessage);
          Memory::StreamBufferList::Reader reader(sbl);
          MQTT::HeaderReader headerReader(reader);

          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "* CA_SendDirect    ");
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Reference Id:      "<<(void*)(long)ptrTask->getReferenceId());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Connection Handle: "<<(void*)(long)ptrTask->getConnectionHandle());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MessageType:       "<<(int)headerReader.getType());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Length:            "<<headerReader.getLength());

          if(headerReader.getType() == MQTT::Message::MT_CONNACK){
            hmConnectionHandles[ptrTask->getConnectionHandle()] = &tabConnections[ptrTask->getReferenceId()];
            tabConnections[ptrTask->getReferenceId()].aConnectionHandle = ptrTask->getConnectionHandle();
          }

          Memory::StreamBufferList::Reader reader2(sbl);
          int rc = reader2.copy(buf, 5000);
          //IA20_LOG(true, "rc "<<rc);
          hmConnectionHandles[ptrTask->getConnectionHandle()]->lstWire.push_back("OUTPUT1: "+MiscTools::BinarytoHex(buf,rc));

        }

     break;

     case Task::CA_SetContent :
        {
          uint8_t *pMessage = ptrTask->getMessage();
          // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "CA_SetContent 1:  "<<(void*)(long)ptrTask->getMessageHandle());
          // IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "CA_SetContent 2:  "<<(void*)(long)ptrTask->getConnectionHandle());

          Memory::StreamBufferList sbl(ptrTask->getMessage());
          Memory::StreamBufferList::Reader reader(sbl);
          MQTT::HeaderReader headerReader (reader);

          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "* CA_SetContent    "<<(void*)pMessage);
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "ContentUsageCount: "<<ptrTask->getContentUsageCount());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Message Handle:    "<<(void*)(long)ptrTask->getMessageHandle());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MessageType:       "<<(int)headerReader.getType());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Length:            "<<headerReader.getLength());

          hmContent[ptrTask->getMessageHandle()].pPayLoad    = ptrTask->getMessage();
          hmContent[ptrTask->getMessageHandle()].iUsageCount = ptrTask->getContentUsageCount();

        }
     break;

     case Task::CA_SendShared :
        {

        uint8_t *pMessage = ptrTask->getMessage();
        Memory::StreamBufferList sbl(pMessage);
        Memory::StreamBufferList::Reader reader(sbl);
        MQTT::HeaderReader headerReader(reader);

        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "* CA_SendShared    ");
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Reference Id:      "<<(void*)(long)ptrTask->getReferenceId());
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Connection Handle: "<<(void*)(long)ptrTask->getConnectionHandle());
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "MessageType:       "<<(int)headerReader.getType());
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Length:            "<<headerReader.getLength());


        Memory::StreamBufferList::Reader reader2(sbl);
        int rc = reader2.copy(buf, 5000);
        //IA20_LOG(true, "rc "<<rc);

        if(hmContent.find(ptrTask->getMessageHandle()) == hmContent.end()){
          IA20_LOG(true, "Content not found: "<<(void*)(long)ptrTask->getMessageHandle());
          return false;
        }

        hmConnectionHandles[ptrTask->getConnectionHandle()]->lstWire.push_back("OUTPUT2: "+MiscTools::BinarytoHex(buf,rc));
        Memory::StreamBufferList sbl3(hmContent[ptrTask->getMessageHandle()].pPayLoad);
        Memory::StreamBufferList::Reader reader3(sbl3);
         rc = reader3.copy(buf, 5000);

        hmConnectionHandles[ptrTask->getConnectionHandle()]->lstWire.push_back("OUTPUT3: "+MiscTools::BinarytoHex(buf,rc));

        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Content usage count: "<<hmContent[ptrTask->getMessageHandle()].iUsageCount);

        if(--hmContent[ptrTask->getMessageHandle()].iUsageCount == 0){
          hmContent.erase(ptrTask->getMessageHandle());
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ! content removed.");
        }

        }
     break;

   }


  return true;

  //IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Response: "<<pMessage->iMessageId<<" "<<icount++);
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

  //   IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Waiting ...");
    serve(true);

  }

}
/*************************************************************************/
void Listener::serveUntilEmptyQueue(){
	IA20_TRACER;

  while(serve(false)){    
  }

}
/*************************************************************************/
void Listener::sendMessage(const String& strHex, int iConnectionId){
  IA20_TRACER;

  static MQTT::Message *theMessage = NULL;

  Memory::SharableMemoryPool::unique_ptr<Task> ptrTask(
    new(ptrMemoryPoolHolder->allocate<Task>(0))Task(Listener::Task::CA_InputMsg), ptrMemoryPoolHolder->getDeleter());

  // if(!theMessage){
  //   theMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get())) MQTT::Message(strHex);
  // }

//  MQTT::Message* pMessage = new(pMemoryPool->allocate<MQTT::Message>(ptrTask.get()))MQTT::Message();;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "*** Mocker send: "<<strHex);

  Memory::StreamBufferList sbl(ptrMemoryPoolHolder.get(), ptrTask.get());
  //TODO helper StringToSBL

  int iMessageLen = strHex.length()/2;

  std::unique_ptr<uint8_t> ptr(new uint8_t[iMessageLen]);

  MiscTools::HexToBinary(strHex, ptr.get(), iMessageLen);

  Memory::StreamBufferList::Writer writer(sbl);
  int i=0;

  while(i < iMessageLen){
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "A1");
    writer.next(32);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "B1");
    int iLength = (iMessageLen - i < writer.getAvailableLength()) ?  iMessageLen - i : writer.getAvailableLength();
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "C: "<<iLength<<", "<<iMessageLen);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Writing: "<<iLength<<",     "<<(void*)writer.getCursor());
  //  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Writing: "<<iLength<<" bytes, to: "<<(void*)writer.getCursor());
    memcpy(writer.getCursor(), ptr.get() + i ,iLength);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Data: "<<MiscTools::BinarytoHex(writer.getCursor(), iLength));
    writer.addData(iLength);
    i += iLength;
  }

  ptrTask->setMessage(sbl.getHead());
  ptrTask->setConnectionHandle(tabConnections[iConnectionId].aConnectionHandle);
  ptrTask->setReferenceId(iConnectionId);

  static int iMessageId = 0;

  ptrTask->iMessageId = ++iMessageId;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Task set msg: "<<(void*)ptrTask->getMessage());
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Port: "<<(void*)ptrPort.get());

  ptrPort->enqueue(ptrTask.release());
  ptrPort->flush();

  tabConnections[iConnectionId].lstWire.push_back("INPUT:   "+strHex);

}
/*******************************************************************/
}
}
}
