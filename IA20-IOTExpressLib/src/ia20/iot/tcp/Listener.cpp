/*
 * File: Listener.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Listener.h"
#include <ia20/commonlib/uring/uring.h>

#include <ia20/iot/mqtt/Message.h>
#include <ia20/iot/mqtt/HeaderReader.h>

#include <ia20/iot/memory/SharableMemoryPool.h>
#include <ia20/iot/memory/StreamBufferList.h>

#include <ia20/iot/logger/LogLevel.h>

#include "Acceptor.h"
#include "Server.h"

namespace IA20 {
namespace IOT {
namespace TCP {

/*************************************************************************/
Listener::Listener(std::unique_ptr<RingType::Interface>&& ptrInterface, int iMaxConnections):
  	IOT::Listener(std::move(ptrInterface), NULL),
	ptrMemoryPoolHolder(new Memory::SharableMemoryPool),
	Thread(this){
	IA20_TRACER;

  	pMemoryPool = ptrMemoryPoolHolder.get();

	IA20_TRACER;
}
/*************************************************************************/
Listener::~Listener() throw(){
	IA20_TRACER;

     for(std::list< std::unique_ptr<Server> >::iterator it = lstServers.begin();
         it != lstServers.end(); it++){
        (*it).reset();
	}
}
/*************************************************************************/
void Listener::createServer(Net::Conn::TCP::FileHandle* pFileHandle){
	IA20_TRACER;
	 std::unique_ptr<Server> ptrServer(new Server(pFileHandle, this));    
    lstServers.push_back(std::move(ptrServer));
}
/*************************************************************************/
void Listener::run(){
	IA20_TRACER;

	SYS::Signal::ThreadRegistration tr;

    ptrRingHandler.reset(new URing::RingHandler);

    Net::Conn::TCP::Peer peer("127.0.0.1", 55556);

	std::unique_ptr<URing::IO::TCP::AsyncServer> ptrAsyncServer(new 
    	URing::IO::TCP::AsyncServer(peer, Net::Conn::TCP::DefaultConnectionFactory::GetInstance()));

    std::unique_ptr<URing::IO::TCP::AsyncServer::Acceptor>
         ptrAcceptor(new Acceptor(ptrAsyncServer.get(), this));

    try{
      ptrAcceptor->prepare();

	  while(!SYS::Signal::GetInstance()->isStopping()){
      	
		static long iCounter=0;

		if(iCounter++ % 100000000== 0)
			IA20_LOG(true, "Spining");

		try{
			ptrRingHandler->handle();
    	}catch(Exception& e){
        	e.printToStream(std::cerr);
    	}
		
				Memory::SharableMemoryPool::unique_ptr<Task> ptrTask(
    	 	ptrInterface->getResponses()->dequeNoWait(), pMemoryPool->getDeleter());
	
		
		if(!ptrTask)
			continue;
		IA20_LOG(true, "Got message");

		if(ptrTask->getReferenceId()){

			IA20_LOG(true, "Setting reference at: "<<(void*)ptrTask->getReferenceId()
				<<" "<<(void*)ptrTask->getConnectionHandle());

			hmConnectionHandle[ptrTask->getConnectionHandle()] = (Server*)ptrTask->getReferenceId();
		}


			 switch(ptrTask->getCommand()){
     			case Task::CA_SendDirect :
					{
						ConnectionHandleMap::iterator it = hmConnectionHandle.find(ptrTask->getConnectionHandle());
						if(it == hmConnectionHandle.end()){
							IA20_LOG(true,"Connection missing for  handle: "<<(int)ptrTask->getConnectionHandle());
							return;
						}

						IA20_LOG(true, "Connection at: "<<(void*)it->second);
						it->second->sendMessage(std::move(ptrTask));
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
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Release:            "<<(void*)ptrTask->getMessage());

		  ptrTask.release(); // TODO nicer solution, for now we delete it later in CA_SendShared
          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Released!");

        }



     break;

		case Task::CA_SendShared :
        {

        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "* CA_SendShared    ");
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Connection Handle: "<<(void*)(long)ptrTask->getConnectionHandle());
        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Messagee Handle:   "<<(void*)(long)ptrTask->getMessageHandle());

		ConnectionHandleMap::iterator it = hmConnectionHandle.find(ptrTask->getConnectionHandle());
		if(it == hmConnectionHandle.end()){
			IA20_LOG(true,"Connection missing for  handle: "<<(void*)ptrTask->getConnectionHandle());
			return;
		}

		IA20_LOG(true, "Connection at: "<<(void*)it->second);
		Message::HandleType aMessageHandle = ptrTask->getMessageHandle();
		
		it->second->publishMessage(std::move(ptrTask));
			IA20_LOG(true, "Check content usage");

        if(--hmContent[aMessageHandle].iUsageCount == 0){
          IA20_LOG(true, "Free content usage");
		  pMemoryPool->free(hmContent[aMessageHandle].pPayLoad);

		  hmContent.erase(aMessageHandle);

          IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ! content removed.");
        }

        }
     break;

	 }
			 }

    }catch(Exception& e){
        e.printToStream(std::cerr);
    }
}
/*************************************************************************/
}
}
}
