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

namespace IA20 {
namespace IOT {
namespace TCP {

const String Server::CStrData("123456789");

/*************************************************************************/
Server::Server(Net::Conn::TCP::FileHandle* pFileHandle,
		   	  const Listener *pListener):
      ReadHandler(pListener->getRingHandler(), pFileHandle),
      WriteHandler(pListener->getRingHandler(), pFileHandle),
      ShutdownHandler(pListener->getRingHandler(), pFileHandle),
	  pListener(pListener),
	  ctx(pListener->getMemoryPool()),
      ptrTask(nullptr, pListener->getMemoryPool()),
      ptrFileHandle(pFileHandle){

	ptrInputBuffer.reset((uint8_t*)malloc(4096));
	ptrOutputBuffer.reset((uint8_t*)malloc(4096));

	if(!ptrInputBuffer || !ptrOutputBuffer)
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
    
    IA20_LOG(true,"Waiting for data");
	ReadHandler::prepare();
};
/*************************************************************************/
void Server::handleRead(off_t iDataLen){
    IA20_TRACER;

    if(iDataLen == 0){
       ShutdownHandler::prepare(SHUT_RDWR);
       return;
    }

    IA20_LOG(true,"Got data[read]: "<<iDataLen<<" "<<ctx.iExpectingLength);

    iBufferDataLength += iDataLen;
	
    while(iBufferDataLength > 0){

       // IA20_LOG(true,"iBufferDataLength[read]: "<<iBufferDataLength<<" "<<ctx.iExpectingLength);
    
        if(ctx.iExpectingLength == 0){

            if(Tools::MQTT::hasFixedHeader(ptrInputBuffer.get(), iBufferDataLength)){
                
                MQTT::HeaderReader headerReader(ptrInputBuffer.get());
                ctx.iExpectingLength = headerReader.getTotalLength();
                
                IA20_LOG(true, "Header type: "<<(int)headerReader.getType()<<", len: "<<headerReader.getLength());
                
            }else{
                IA20_LOG(true,"Wants more");
                ReadHandler::iovec.iov_base = ptrInputBuffer.get() + iBufferDataLength;
                break;
            }

        }

        IA20_LOG(true,"Check:"<<(int)ctx.iExpectingLength<<" ? "<<(int)iBufferDataLength);

        if(ctx.iExpectingLength <= iBufferDataLength){             
                    
            ctx.writer.write(ptrInputBuffer.get(), ctx.iExpectingLength);
            //TODO first time only ?
            ctx.ptrTask->setReferenceId((uint64_t)this);
            ctx.ptrTask->setConnectionHandle(aConnectionHandle);
            pListener->getInterface()->getRequests()->enque(ctx.ptrTask.release());

            uint32_t iRemaining = iBufferDataLength - ctx.iExpectingLength;

            IA20_LOG(true,"Check1:"<<(int)iRemaining);

            if(iRemaining >= 0){
                uint8_t* pSrc =  ptrInputBuffer.get() + ctx.iExpectingLength;
                memcpy(ReadHandler::iovec.iov_base, pSrc, iRemaining);
            }            
                    
            iBufferDataLength -= ctx.iExpectingLength;
            
            ctx.~Context(); //this is stupid.
            new (&ctx)Context(pListener->getMemoryPool());

        }else{ 

            ctx.writer.write(ptrInputBuffer.get(), iBufferDataLength);
            ctx.iExpectingLength -= iBufferDataLength;
            iBufferDataLength = 0;

        }

    }

    ReadHandler::prepare();
}
/*************************************************************************/
void Server::sendMessage(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask){
    IA20_TRACER;

	IA20_LOG(true, "Sending message at: "<<(void*)ptrTask->getConnectionHandle());
    
    this->aConnectionHandle = ptrTask->getConnectionHandle();

    uint8_t *pMessage = ptrTask->getMessage();
    Memory::StreamBufferList sbl(pMessage);
    Memory::StreamBufferList::Reader reader(sbl);
    
    WriteHandler::iovec.iov_base = reader.getData();
    WriteHandler::iovec.iov_len  = reader.getLength();

    IA20_LOG(true, "Sending Data: "
    <<MiscTools::BinarytoHex(WriteHandler::iovec.iov_base, WriteHandler::iovec.iov_len));
    
    this->ptrTask = std::move(ptrTask);

    WriteHandler::prepare();
}
/*************************************************************************/
void Server::publishMessage(Memory::SharableMemoryPool::unique_ptr<IOT::Listener::Task>&& ptrTask){
    IA20_TRACER;

	IA20_LOG(true, "Sending message at: "<<(void*)ptrTask->getConnectionHandle());
    
    this->aConnectionHandle = ptrTask->getConnectionHandle();

    uint8_t *pMessage = ptrTask->getMessage();
    Memory::StreamBufferList sbl(pMessage);
    Memory::StreamBufferList::Writer writer(sbl);

    Memory::StreamBufferList sbl2(pListener->getContentPayload(ptrTask->getMessageHandle()));
    Memory::StreamBufferList::Reader reader2(sbl2);

    writer.write(reader2.getData(), reader2.getLength());
    
    Memory::StreamBufferList::Reader reader3(sbl);

    WriteHandler::iovec.iov_base = reader3.getData();
    WriteHandler::iovec.iov_len  = reader3.getLength();

    IA20_LOG(true, "Sending Data: "
        <<MiscTools::BinarytoHex(WriteHandler::iovec.iov_base, WriteHandler::iovec.iov_len));
    
    this->ptrTask = std::move(ptrTask);

    WriteHandler::prepare();
}
/*************************************************************************/
void Server::handleWrite(off_t iDataLen){
    std::cerr<<"Written: ["<<iDataLen<<"]"<<std::endl;
    ptrTask.release();
}
/*************************************************************************/
void Server::handleShutdown(int iResult){
    std::cerr<<"Shutdown: ["<<iResult<<"], msgs: "<<iMessages<<std::endl;
}
/*************************************************************************/
}
}
}
