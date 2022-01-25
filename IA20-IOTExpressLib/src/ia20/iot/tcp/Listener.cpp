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
	#include <ia20/iot/tools/sys/TaskPort.h>

	#include <ia20/iot/logger/LogLevel.h>

	#include "Acceptor.h"
	#include "Server.h"

	namespace IA20 {
	namespace IOT {
	namespace TCP {

	/*************************************************************************/
	Listener::Listener(int iMaxConnections):
		ptrMemoryPoolHolder(new Memory::SharableMemoryPool),
		Thread(this){
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

		setupPort();

		Net::Conn::TCP::Peer peer("0.0.0.0", 55556);

		std::unique_ptr<URing::IO::TCP::AsyncServer> ptrAsyncServer(new
			URing::IO::TCP::AsyncServer(peer, Net::Conn::TCP::DefaultConnectionFactory::GetInstance()));

		std::unique_ptr<URing::IO::TCP::AsyncServer::Acceptor>
			ptrAcceptor(new Acceptor(ptrAsyncServer.get(), this));

		try{

			ptrAcceptor->prepare();

			while(!SYS::Signal::GetInstance()->isStopping()){
				static long iCounter=0;

				if(iCounter++ % 100== 0)
					IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Spining");
				serve();

			}

		}catch(Exception& e){
			e.printToStream(std::cerr);
		}
	}
	/*************************************************************************/
	inline void Listener::serve(){
		IA20_TRACER;

			try{
				ptrRingHandler->handleAtLeastOnce();
			}catch(Exception& e){
				e.printToStream(std::cerr);
			}

			Task* pTask;

			while(ptrPort->dequeue(pTask)){

				Memory::SharableMemoryPool::unique_ptr<Task> ptrTask(pTask, ptrMemoryPoolHolder->getDeleter());

				IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Got message");

				if(ptrTask->getReferenceId()){

					IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Setting reference at: "<<(void*)ptrTask->getReferenceId()
					<<" "<<(void*)ptrTask->getConnectionHandle());

					hmConnectionHandle[ptrTask->getConnectionHandle()] = (Server*)ptrTask->getReferenceId();
				}


				switch(ptrTask->getCommand()){
					case Task::CA_SendDirect :
					case Task::CA_SendShared :
						send(std::move(ptrTask));
					break;

					case Task::CA_SetContent :
						setContent(std::move(ptrTask));
					break;

					default:
						IA20_THROW(InternalException("Unknown listner command: ")<<ptrTask->getCommand());
				}


			}

	}

/*************************************************************************/
inline void Listener::send(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask){

	ConnectionHandleMap::iterator it = hmConnectionHandle.find(ptrTask->getConnectionHandle());

	if(it == hmConnectionHandle.end()){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Connection missing for handle: "<<(int)ptrTask->getConnectionHandle());
		return;
	}

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Connection at: "<<(void*)it->second);
	it->second->sendMessage(std::move(ptrTask));
}
/*************************************************************************/
inline void Listener::setContent(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask){
	uint8_t *pMessage = ptrTask->getMessage();

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
/*************************************************************************/
void Listener::decUsageCount(Message::HandleType aMessageHandle){

	//TODO call back from server/connection remove when sent
	if(--hmContent[aMessageHandle].iUsageCount == 0){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Free content usage");
		ptrMemoryPoolHolder->free(hmContent[aMessageHandle].pPayLoad);
		hmContent.erase(aMessageHandle);
		// IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, " ! content removed.");
	}
}

/*************************************************************************/
void Listener::removeServer(Server* pServer){


	Connection::HandleType aConnectionHandle = pServer->getConnectionHandle();

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Remove server: "<<(void*)(long)aConnectionHandle);

	 hmConnectionHandle.erase(aConnectionHandle);

	for(std::list< std::unique_ptr<Server> >::iterator it = lstServers.begin();
		it != lstServers.end(); it++){
			if( (*it)->getConnectionHandle() == aConnectionHandle){
				lstServers.erase(it);
				return;
			}
		}

}
/*************************************************************************/
  
}
}
}
