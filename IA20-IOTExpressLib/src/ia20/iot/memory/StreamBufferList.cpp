/*
 * File: StreamBufferList.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "StreamBufferList.h"
#include "SharableMemoryPool.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Memory {

StreamBufferList::DataLengthType StreamBufferList::Chunk::CMaxChunkSize(2000);

/*************************************************************************/
StreamBufferList::StreamBufferList(SharableMemoryPool* pMemoryPool, void* pOnwerAddress):
	pHead(0),
	pMemoryPool(pMemoryPool){
	IA20_TRACER;

	IA20_CHECK_IF_NULL(pMemoryPool);
	IA20_CHECK_IF_NULL(pOnwerAddress);

	pHead = allocateChunk(pOnwerAddress);

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "SBL head1: "<<(void*)pHead<<", owner: "<<(void*)pOnwerAddress);
}
/*************************************************************************/
StreamBufferList::StreamBufferList(void *pFromMemory):
	pHead(reinterpret_cast<Chunk*>(pFromMemory)),
	pMemoryPool(NULL){
	IA20_TRACER;

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "SBL head2: "<<(void*)pHead);
}
/*************************************************************************/
StreamBufferList::~StreamBufferList() throw(){
	IA20_TRACER;
	//if(pHead && !pOnwerAddress)
	//	pMemoryPool->free(pHead);
}
/*************************************************************************/
StreamBufferList::Chunk* StreamBufferList::allocateChunk(void* pOnwerAddress)const{
	IA20_TRACER;

	uint32_t iBytesAllocated;

	Chunk *pResult = reinterpret_cast<Chunk*>(pMemoryPool->allocate(pOnwerAddress,
							 Chunk::CMaxChunkSize / 4 + sizeof(Chunk),
							 Chunk::CMaxChunkSize + sizeof(Chunk),
							 iBytesAllocated));


	return new(pResult)Chunk(iBytesAllocated - sizeof(Chunk));
}
/*************************************************************************/
void StreamBufferList::Reader::getNext(){

	if(bFinished)
		return;

	if(pChunk->isLast()){
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "isLast()");
		bFinished = true;
		iDataLength = 0;
	}else{
		pChunk = pChunk->getNext();
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, (*pChunk)<<", getDataStart: "<<(void*)pChunk->getDataStart());
		pData  = pChunk->getDataStart();
		iDataLength = pChunk->iDataLength;
	}

}
/*************************************************************************/
void StreamBufferList::Writer::next(DataLengthType iMinDataLength){

	if(iMinDataLength < sizeof(Chunk))
		iMinDataLength = sizeof(Chunk);

	if(!pChunk || pChunk->iChunkSize - pChunk->iDataLength < iMinDataLength){

		if(!sbl.pMemoryPool)
			IA20_THROW(InternalException("Writing not allowed to this SBL object."));

		Chunk *pNew = sbl.allocateChunk(pChunk);

		if(pChunk){
			pChunk->setNext(pNew);
		}

		pChunk = pNew;
	}

	IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "pCursor2:"<<(void*)pChunk->getDataStart()<<", "<<pChunk->iDataLength);

	pCursor           = pChunk->getDataStart() + pChunk->iDataLength;
	iAvailableLength  = pChunk->iChunkSize - pChunk->iDataLength;
}

/*************************************************************************/
}
}
}
