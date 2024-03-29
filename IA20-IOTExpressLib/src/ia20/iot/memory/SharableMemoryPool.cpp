/*
 * File: SharableMemoryPool.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "SharableMemoryPool.h"


#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Memory {
using namespace Tools::SPIN;

SharableMemoryPool::SizeType   SharableMemoryPool::CSegmentSize =  0x00001000L;
SharableMemoryPool::SizeType   SharableMemoryPool::CSegmentDataSize = CSegmentSize - sizeof(SegmentList);
uint64_t                       SharableMemoryPool::CSegmentMask = ~((uint64_t)CSegmentSize - 1);

/*************************************************************************/

/*************************************************************************/
SharableMemoryPool::SharableMemoryPool():
 pFreeSegments(0),
 mDeleter(this){
	IA20_TRACER;
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory,"SharableMemoryPool created");
  stats.reset();

  const int iMaxSegments = 1000;

    //TODO provider class, temporary solution to get segmentsize rounded addressing ;)
  SegmentList *pResult = reinterpret_cast<SegmentList*>(std::aligned_alloc(CSegmentSize, iMaxSegments*CSegmentSize));

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "New memory at: "<<(void*)pResult<<", to "<<(void*)((uint8_t*)pResult+iMaxSegments*CSegmentSize));

  for(int i = 0; i<iMaxSegments; i++){
    SegmentList* pNew = GetSegmentAddress((uint8_t*)pResult + CSegmentSize * i);
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "New segment at: "<<(void*)pResult);
    new(pNew) SegmentList();
    freeSegment(pNew);
  }

}
/*************************************************************************/
SharableMemoryPool::~SharableMemoryPool() throw(){
	IA20_TRACER;
  std::cerr<<stats;
}
/*************************************************************************/
SharableMemoryPool::SegmentList* SharableMemoryPool::allocateSegment(){
  IA20_TRACER;

  Locker locker(mLock);

  if(pFreeSegments){
    
    SegmentList *pResult = pFreeSegments;
    IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "reused segment at: "<<(void*)pResult);

    if(pResult->isSingle()){
      pFreeSegments = NULL;
    }else{  
      pFreeSegments = pFreeSegments->getNext();
    }

    stats.iFreeSegments--;
    return new(pResult) SegmentList();
  }

  IA20_THROW(InternalException("out of memory in SMP")<<"free: ?? "<<stats.iFreeSegments);
}
/*************************************************************************/
void SharableMemoryPool::freeSegment(SharableMemoryPool::SegmentList* pSegment){
  IA20_TRACER;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Free segment at: "<<(void*)pSegment);
  
  SegmentList *pNext = pSegment->getNext();

  Locker locker(mLock);

  pSegment->setNext(pFreeSegments ? pFreeSegments : pSegment);
  pFreeSegments = pNext;

  stats.iFreeSegments++;
}
/*************************************************************************/
void *SharableMemoryPool::allocate(void* pAddress, uint32_t iSize){
  IA20_TRACER;
  uint32_t iBytesAllocated;
  return   allocate(pAddress, iSize, iSize, iBytesAllocated);
}
/*************************************************************************/
void *SharableMemoryPool::allocate(void* pAddress, uint32_t iMinSize, uint32_t iMaxSize, uint32_t& iBytesAllocated){
  IA20_TRACER;

  iMinSize = (iMinSize + 0x7) & ~0x07;

  if(iMinSize > iMaxSize)
   IA20_THROW(BadUsageException("Min. size is greater than max. size: [")<<(int)iMinSize<<","<<(int)iMaxSize<<"]");

  SegmentList *pStart = pAddress ? GetSegmentAddress(pAddress) : NULL;
  SegmentList *pCursor = pStart;
  SegmentList *pLast   = pStart;
  
  while(pCursor && pCursor->iUsed + iMinSize > CSegmentDataSize){

    pLast   = pCursor;

    pCursor = pCursor->getNext();

    if(pCursor == pStart)
      pCursor = NULL;

  }

  // IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Memory 1: "<<(void*)pStart);
  // IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Memory 1: "<<(void*)pCursor);
  // IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Memory 1: "<<(void*)pLast);
    
  if(!pCursor){
    
    pCursor = new(allocateSegment()) SegmentList();
    
    if(pLast){
      SegmentList *pLastNext = pLast->getNext();
      pLast->setNext(pCursor);
      pCursor->setNext(pLastNext);
    }

  }

  uint8_t* pResult = reinterpret_cast<uint8_t*>(pCursor) + sizeof(SegmentList) + pCursor->iUsed;

  uint32_t iSize = iMaxSize;

  if(iSize + pCursor->iUsed > CSegmentDataSize){
    iSize = CSegmentDataSize - pCursor->iUsed;
  }

  pCursor->iUsed += iSize;
  iBytesAllocated = iSize;

  IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Allocated "<<iSize<<" bytes at: "<<(void*)pResult);
  
  stats.iNumAllocations++;
  
  return pResult;
}
/*************************************************************************/
void SharableMemoryPool::free(void* pAddress){
  IA20_TRACER;

  SegmentList *pCursor = GetSegmentAddress(pAddress);
  freeSegment(pCursor);
  
  IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "Freed at: "<<(void*)pAddress<<", segment: "<<(void*)pCursor);
}
/*************************************************************************/
std::ostream& operator<<(std::ostream& os, const SharableMemoryPool::Stats& s){
  
  os<<"ShareableMemoryPool at:"<<(void*)&s<<std::endl;
  os<<"Allocations: \t"<<s.iNumAllocations<<std::endl;
  os<<"FreeSegments: \t"<<s.iFreeSegments<<std::endl;

  return os;
 }
/*************************************************************************/
}
}
}
