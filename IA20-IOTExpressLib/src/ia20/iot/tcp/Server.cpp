/*
 * File: Server.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "Server.h"
#include "Listener.h"

#include <ia20/iot/memory/SharableMemoryPool.h>

#include <ia20/iot/tools/MQTT.h>
#include <ia20/iot/mqtt/HeaderReader.h>

#include "ContextOutputDirect.h"
#include "ContextOutputShared.h"

namespace IA20 {
namespace IOT {
namespace TCP {

const String Server::CStrData("123456789");

/*************************************************************************/
Server::Server(Net::Conn::TCP::FileHandle* pFileHandle,
		   	   Listener *pListener):
      ReadHandler(pListener->getRingHandler(), pFileHandle),
      WriteHandler(pListener->getRingHandler(), pFileHandle),
      ShutdownHandler(pListener->getRingHandler(), pFileHandle),
	  pListener(pListener),
	  ctx(pListener->getMemoryPool()),
      ptrFileHandle(pFileHandle){

	ptrInputBuffer.reset((uint8_t*)malloc(4096));

	if(!ptrInputBuffer)
		IA20_THROW(InternalException("Not enought memory !!!"));

	prepareRead();
}		
/*************************************************************************/
Server::~Server() throw(){
	IA20_TRACER;
}
/*************************************************************************/
void Server::prepareRead(){
	IA20_TRACER;
    
    ReadHandler::iovec.iov_base = ptrInputBuffer.get();
	ReadHandler::iovec.iov_len  = 4096;
	
    iBufferDataLength = 0;  
    
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Waiting for data");
	ReadHandler::prepare();
};
/*************************************************************************/
void Server::handleRead(off_t iDataLen){
    IA20_TRACER;

    if(iDataLen == 0){
       ShutdownHandler::prepare(SHUT_RDWR);
       return;
    }

    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Got data[read]: "<<iDataLen<<" "<<ctx.getExpectedLength());

    iBufferDataLength += iDataLen;
	
    while(iBufferDataLength > 0){

        IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"iBufferDataLength[read]: "<<iBufferDataLength<<" "<<ctx.getExpectedLength());
    
        /* ctx.getExpectedLength() == 0 => no header yet */

        if(ctx.getExpectedLength() == 0){ 

            if(Tools::MQTT::hasFixedHeader(ptrInputBuffer.get(), iBufferDataLength)){
                
                MQTT::HeaderReader headerReader(ptrInputBuffer.get());
                ctx.setExpectedLength(headerReader.getTotalLength());
                
                IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "LISTENER: Header type: "<<(int)headerReader.getType()<<", len: "<<headerReader.getLength());
                
            }else{
                IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Wants more");
                ReadHandler::iovec.iov_base = ptrInputBuffer.get() + iBufferDataLength;
                break;
            }

        }

        IA20_LOG(IOT::LogLevel::INSTANCE.bIsDetailedInfo|true,"Check:"<<(int)ctx.getExpectedLength()<<" ? "<<(int)iBufferDataLength);

        if(ctx.getExpectedLength() <= iBufferDataLength){             
                    
            const uint32_t iCompletedLength = ctx.getExpectedLength();

            ctx.appendExpected(ptrInputBuffer.get());

            pListener->getPort()->enqueue(ctx.finish((uint64_t)this, aConnectionHandle).release()); //TODO enqueue unique_ptr            
            
            uint32_t iRemaining = iBufferDataLength - iCompletedLength;

            if(iRemaining >= 0){
                uint8_t* pSrc =  ptrInputBuffer.get() + iCompletedLength;
                memcpy(ReadHandler::iovec.iov_base, pSrc, iRemaining);
            }            
                    
            iBufferDataLength -= iCompletedLength;

        }else{ 

            ctx.append(ptrInputBuffer.get(), iBufferDataLength);
            
            iBufferDataLength = 0;

        }

    }

    ReadHandler::prepare();

    pListener->getPort()->flush();
}
/*************************************************************************/
void Server::serveNextOutputTask(){
    IA20_TRACER;

    Listener::Task *pTask = queueTasks.front().get();

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Sending message at: "<<(void*)pTask->getConnectionHandle());
    
    this->aConnectionHandle = pTask->getConnectionHandle();
    uint8_t *pMessage = pTask->getMessage();

    switch(pTask->getCommand()){

    case Listener::Task::CA_SendDirect:
        ptrContextOutput.reset(new ContextOutputDirect(pTask->getMessage()));
    break;

    case Listener::Task::CA_SendShared:
        ptrContextOutput.reset(new ContextOutputShared(pTask->getMessage(), 
                                                        pListener->getContentPayload(pTask->getMessageHandle())));
    break;

    default:
        IA20_THROW(InternalException("Unsupported task type in  ContextOutput::Create"));
	}

    prepareWrite();
}
/*************************************************************************/
void Server::prepareWrite(){
    
    IA20_TRACER;

    WriteHandler::iovec.iov_base = ptrContextOutput->getData();
    WriteHandler::iovec.iov_len  = ptrContextOutput->getDataLength();

    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo, "Sending Data: "
        <<MiscTools::BinarytoHex(WriteHandler::iovec.iov_base, WriteHandler::iovec.iov_len));
    
    WriteHandler::prepare();

}
/*************************************************************************/
void Server::sendMessage(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask){
    IA20_TRACER;

    queueTasks.emplace_back(std::move(ptrTask));

     if(queueTasks.size() == 1)
        serveNextOutputTask();
}
/*************************************************************************/
void Server::handleWrite(off_t iDataLen){
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Written: ["<<iDataLen<<"]");;
    
    if(ptrContextOutput->next()){
        prepareWrite();
        return;
    }

    if(queueTasks.front()->getCommand() == Listener::Task::CA_SendShared){
        pListener->decUsageCount(queueTasks.front()->getMessageHandle());
    }

    queueTasks.pop_front();
   
    if(queueTasks.size())
        serveNextOutputTask();

}
/*************************************************************************/
void Server::handleShutdown(int iResult){
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsInfo,"Shutdown: ["<<iResult<<"], msgs: "<<iMessages);;
}
/*************************************************************************/
}
}
}
