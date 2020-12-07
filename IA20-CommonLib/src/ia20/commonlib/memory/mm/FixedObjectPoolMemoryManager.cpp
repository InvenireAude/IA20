/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/FixedObjectPoolMemoryManager.cpp
 *
 * Copyright (C) 2020, Albert Krzymowski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "FixedObjectPoolMemoryManager.h"
#include <ia20/commonlib/exception/InternalException.h>

#include <new>
#include <string.h>
#include <strings.h>

namespace IA20 {

/*************************************************************************/
size_t FixedObjectPoolMemoryManager::ComputeMemoryRequirement(size_t iObjectSize, size_t iNumObjects){
  if(iObjectSize % sizeof(EntryIndex) != 0){
    IA20_THROW(InternalException("FixedObjectPoolMemoryManager object size error: ")<<iObjectSize<<" ? "<<sizeof(EntryIndex));
  }
  return sizeof(Info) + iObjectSize * iNumObjects;
}
/*************************************************************************/
FixedObjectPoolMemoryManager::FixedObjectPoolMemoryManager(
    void *pMemory, size_t iObjectSize, size_t iNumObjects):
  bFreeMe(false){
	IA20_TRACER;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"Call for FOPMM: "<<iNumObjects<<" at: "<<pMemory<<", size="<<iObjectSize<<" at="<<(void*)this);

  if(iObjectSize % sizeof(EntryIndex) != 0){
    IA20_THROW(InternalException("FixedObjectPoolMemoryManager object size error: ")<<iObjectSize<<" ? "<<sizeof(EntryIndex));
  }

  const size_t iTabSize = ComputeMemoryRequirement(iObjectSize, iNumObjects);

  if(!pMemory){

    pMemory = new char[iTabSize];

    if(!pMemory){
      IA20_LOG(LogLevel::INSTANCE.bIsError,"FixedObjectPoolMemoryManager cannot allocate memory.");
      throw std::bad_alloc();
    }

    bFreeMe = true;
  }

  this->pMemory = pMemory;
  new (pMemory) Info();

  refObjectSize() = iObjectSize;
  refNumObjects() = iNumObjects;
  refTop() = 0;

  for(EntryIndex iIdx = 0; iIdx < iNumObjects - 1; iIdx++){
    indexEntry(iIdx) =  iIdx + 1;
  }
  indexEntry(iNumObjects - 1) = CUnusedIndex;

  bCheckPointersOnReturn = false;

	IA20_LOG(LogLevel::INSTANCE.isInfo(),"FOPMM is ready !");

}
/*************************************************************************/
FixedObjectPoolMemoryManager::FixedObjectPoolMemoryManager(void *pMemory):
  pMemory(pMemory),
  bFreeMe(false){
     bCheckPointersOnReturn = false;
  };
/*************************************************************************/
void* FixedObjectPoolMemoryManager::allocate(size_t n){

  if(n != refObjectSize()){
    IA20_LOG(LogLevel::INSTANCE.bIsError,"(FOPMM) Bad memory allocation request, got: "<<n<<" bytes, expected: "<<refObjectSize()<<".");
    PrintTrace(std::cerr);
    throw std::bad_alloc();
  }

  if(refTop() == CUnusedIndex){
    IA20_LOG(LogLevel::INSTANCE.bIsError,"FixedObjectPoolMemoryManager is out of memory.");
     PrintTrace(std::cerr);
    throw std::bad_alloc();
  }

	Mutex::Locker locker(refMutex(), tsrMutexWaits);
	AutoTimeSample sample(tsrAllocations,LogLevel::INSTANCE.isProfile());

  void *pResult = &(indexEntry(refTop()));
  refTop() = indexEntry(refTop());
	return pResult;
}
/*************************************************************************/
 bool FixedObjectPoolMemoryManager::isPointerSane(const void *p) const{
		bool bResult = p >= getStart() &&
		   p < getStart() + refObjectSize() * refNumObjects() &&
           ((unsigned char*)p - getStart()) % refObjectSize() == 0;

		if(!bResult){
			IA20_LOG(LogLevel::INSTANCE.bIsError,"FixedObjectPoolMemoryManager pointer error: "
				<<p<<" "
				<<((unsigned char*)p - getStart())
				<<", refObjectSize(): "<<refObjectSize()<<", iNumObjects: "<<refNumObjects());
        PrintTrace(std::cerr);
		}

		return bResult;
  	}
/*************************************************************************/
void  FixedObjectPoolMemoryManager::free(const void* p){

  if(!isPointerSane(p)){
   throw std::bad_alloc();
  }

	EntryIndex iReturned = ((unsigned char*)p - getStart())/refObjectSize();

  Mutex::Locker locker(refMutex(), tsrMutexWaits);

  indexEntry(iReturned) = refTop();
  refTop() = iReturned;
}
/*************************************************************************/
bool  FixedObjectPoolMemoryManager::check(const void* p){
  IA20_LOG(LogLevel::INSTANCE.isInfo(),"check:"<<bCheckPointersOnReturn);
  if(!isPointerSane(p))
    return false;

  EntryIndex iReturned = ((unsigned char*)p - getStart())/refObjectSize();

	Mutex::Locker locker(refMutex(), tsrMutexWaits);

  EntryIndex iCurrent = refTop();

  while(iCurrent != CUnusedIndex){

    if(p == (void*)&(indexEntry(iCurrent))){
      IA20_LOG(LogLevel::INSTANCE.bIsError,
        "FixedObjectPoolMemoryManager::check() failed for: "<<iCurrent<<"\t"<<(void*)&(indexEntry(iCurrent))<<"\t"<<p);
      return false;
    }

    iCurrent = indexEntry(iCurrent);
  }
	return true;
}
/*************************************************************************/
FixedObjectPoolMemoryManager::~FixedObjectPoolMemoryManager()throw(){
	IA20_TRACER;
}

/*************************************************************************/
void FixedObjectPoolMemoryManager::printToStream(std::ostream& os){

	Mutex::Locker locker(refMutex());

	os<<"FixedObjectPoolMemoryManager, entries="<<refNumObjects()<<", size="<<refObjectSize()<<" bytes."<<std::endl;

	os<<"  Waits:        "<<tsrMutexWaits<<std::endl;
	os<<"  Allocations:  "<<tsrAllocations<<std::endl;
	os<<std::endl;
}
/*************************************************************************/
void FixedObjectPoolMemoryManager::dump(std::ostream& os){

  os<<"Free List"<<std::endl;
  Mutex::Locker locker(refMutex());

  EntryIndex iCurrent = refTop();

  while(iCurrent != CUnusedIndex){
    os<<iCurrent<<"\t"<<(void*)&(indexEntry(iCurrent))<<std::endl;
    iCurrent = indexEntry(iCurrent);
  }
  os<<"End of dump."<<std::endl;
}
/*************************************************************************/
void FixedObjectPoolMemoryManager::trim(){
}
/*************************************************************************/
}
