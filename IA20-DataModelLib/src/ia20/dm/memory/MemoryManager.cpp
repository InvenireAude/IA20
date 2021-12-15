/*
 * File: MemoryManager.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
 *
 */

#include "MemoryManager.h"


namespace IA20 {
namespace DM {
namespace Memory {

MemoryManager::SizeType           MemoryManager::CSegmentSize =  0x00010000L;
MemoryManager::AddressIntegerType MemoryManager::CSegmentMask = ~0x0000ffffL;

thread_local MemoryManager* MemoryManager::TheThreadLocal = NULL;
/*************************************************************************/

/*************************************************************************/
MemoryManager::MemoryManager(){
	IA20_TRACER;
}
/*************************************************************************/
MemoryManager::~MemoryManager() throw(){
	IA20_TRACER;
  IA20_LOG(false,"MM Stacked segments: "<<stackSegments.size());
}
/*************************************************************************/
MemoryManager::SegmentList* MemoryManager::allocateSegment(){
  IA20_TRACER;

  if(!stackSegments.empty()){

    SegmentList *pResult = stackSegments.top();
    stackSegments.pop();
    IA20_LOG(false, "reused segment at: "<<(void*)pResult);
    return new(pResult) SegmentList();
  }
  //TODO provider class, temporary solution to get segmentsize rounded addressing ;)
  SegmentList *pResult = reinterpret_cast<SegmentList*>(new uint8_t[2*CSegmentSize]);
  IA20_LOG(false, "New memory at: "<<(void*)pResult<<", to "<<(void*)((uint8_t*)pResult+2*CSegmentSize));
  pResult = GetSegmentAddress((uint8_t*)pResult+CSegmentSize);

  IA20_LOG(false, "New segment at: "<<(void*)pResult);
  return new(pResult) SegmentList();
}
/*************************************************************************/
void MemoryManager::freeSegment(MemoryManager::SegmentList* pSegment){
  IA20_TRACER;
  IA20_LOG(false, "Free segment at: "<<(void*)pSegment);
  stackSegments.push(pSegment);
}
/*************************************************************************/
void *MemoryManager::allocate(void* pAddress, uint32_t iSize){
  IA20_TRACER;

  iSize = (iSize + 0x7) & ~0x07;

  SegmentList *pCursor = pAddress ? GetSegmentAddress(pAddress) : NULL;

  while(pCursor && sizeof(SegmentList) + pCursor->iFreeOffset + iSize > CSegmentSize){
    IA20_LOG(false, "Memory left: "<<pCursor->iFreeOffset);
    pCursor = pCursor->pNext;
  }

  if(!pCursor){
    pCursor = allocateSegment();
  }

  uint8_t* pResult = reinterpret_cast<uint8_t*>(pCursor) + sizeof(SegmentList) + pCursor->iFreeOffset;

  pCursor->iFreeOffset += iSize;

  IA20_LOG(false, "Allocated "<<iSize<<" bytes at: "<<(void*)pResult);
  return pResult;
}
/*************************************************************************/
void MemoryManager::free(void* pAddress){
  IA20_TRACER;

  SegmentList *pCursor = GetSegmentAddress(pAddress);

  while(pCursor->pPrev)
    pCursor = pCursor->pPrev;

  while(pCursor){
    SegmentList *pNext = pCursor->pNext;
    freeSegment(pCursor);
    pCursor = pNext;
  }
  IA20_LOG(false, "Stacked segements: "<<stackSegments.size()<<" last at: "<<(void*)pAddress);
}
/*************************************************************************/
}
}
}
