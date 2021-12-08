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

thread_local MemoryManager* MemoryManager::TheThreadLocal;
/*************************************************************************/

/*************************************************************************/
MemoryManager::MemoryManager(){
	IA20_TRACER;
}
/*************************************************************************/
MemoryManager::~MemoryManager() throw(){
	IA20_TRACER;
}
/*************************************************************************/
MemoryManager::SegmentList* MemoryManager::allocateSegment(){
  IA20_TRACER;

  if(!stackSegments.empty()){

    SegmentList *pResult = stackSegments.top();
    stackSegments.pop();

    return pResult;
  }
  return new(new uint8_t[CSegmentSize]) SegmentList(); //TODO provider class
}
/*************************************************************************/
void MemoryManager::freeSegment(MemoryManager::SegmentList* pSegment){
  IA20_TRACER;
  stackSegments.push(pSegment);
}
/*************************************************************************/
void *MemoryManager::allocate(void* pAddress, uint32_t iSize){
  IA20_TRACER;

  SegmentList *pCursor = pAddress ? GetSegmentAddress(pAddress) : NULL;

  while(!pCursor && sizeof(SegmentList) + pCursor->iFreeOffset + iSize > CSegmentSize){
    pCursor = pCursor->pNext;
  }

  if(!pCursor){
    pCursor = allocateSegment();
  }

  uint8_t* pResult = reinterpret_cast<uint8_t*>(pCursor) + sizeof(SegmentList) + pCursor->iFreeOffset;

  pCursor->iFreeOffset -= iSize;

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
    stackSegments.push(pCursor);
    pCursor = pNext;
  }

}
/*************************************************************************/
}
}
}
