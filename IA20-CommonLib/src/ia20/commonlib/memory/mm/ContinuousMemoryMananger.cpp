/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/ContinuousMemoryMananger.cpp
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
#include "ContinuousMemoryMananger.h"
#include <ia20/commonlib/exception/InternalException.h>

#include <ia20/commonlib/logger/logger.h>

#include <new>
#include <string.h>
#include <strings.h>
#include <iostream>

namespace IA20 {

/*************************************************************************/
size_t ContinuousMemoryMananger::ComputeMemoryRequirement(size_t iMemoryPoolSize){
  return sizeof(Info) + iMemoryPoolSize;
}
/*************************************************************************/
ContinuousMemoryMananger::ContinuousMemoryMananger(
    void *pMemory, size_t iMemoryPoolSize):
  bFreeMe(false){
	IA20_TRACER;

  if(!pMemory){

    pMemory = new char[ComputeMemoryRequirement(iMemoryPoolSize)];

    if(!pMemory){
      IA20_LOG(LogLevel::INSTANCE.bIsError,"ContinuousMemoryMananger cannot allocate memory.");
      throw std::bad_alloc();
    }

    bFreeMe = true;
  }

  this->pMemory = pMemory;
  refFreeOffset() = 0;
  refSize() = iMemoryPoolSize;
	IA20_LOG(LogLevel::INSTANCE.isInfo(),"CMM is ready !");

}
/*************************************************************************/
ContinuousMemoryMananger::ContinuousMemoryMananger(void *pMemory):
  pMemory(pMemory),
  bFreeMe(false){}
/*************************************************************************/
void* ContinuousMemoryMananger::allocate(size_t n){

  if(getFree() < n){
    IA20_LOG(LogLevel::INSTANCE.bIsError,"ContinuousMemoryMananger is out of memory. "<<"n= "<<n<<", size="<<this->getFree());
    throw std::bad_alloc();
  }

	Mutex::Locker locker(refMutex(), tsrMutexWaits);
	AutoTimeSample sample(tsrAllocations,LogLevel::INSTANCE.isProfile());

  void *pResult = getAddressAtStart();

  lstEntries.push_back(LogEntry(pResult, n));
  refFreeOffset() += n;
	return pResult;
}
/*************************************************************************/
inline bool ContinuousMemoryMananger::isPointerSane(const void *p) const{
		bool bResult = p >= getStart() &&
		   p < getAddressAtStart();

		if(!bResult){
			IA20_LOG(LogLevel::INSTANCE.bIsError,"ContinuousMemoryMananger pointer error: "
				<<p<<" "
				<<((unsigned char*)p - (unsigned char*)getStart())
				<<", offset: "<<refFreeOffset()<<", refSize: "<<refSize());
		}

		return bResult;
  	}
/*************************************************************************/
void  ContinuousMemoryMananger::free(const void* p){

  if(!isPointerSane(p)){
   throw std::bad_alloc();
  }

}
/*************************************************************************/
bool  ContinuousMemoryMananger::check(const void* p){

  if(!isPointerSane(p))
    return false;

	return true;
}
/*************************************************************************/
ContinuousMemoryMananger::~ContinuousMemoryMananger()throw(){
	IA20_TRACER;
}

/*************************************************************************/
void ContinuousMemoryMananger::printToStream(std::ostream& os){

	Mutex::Locker locker(refMutex());

	os<<"ContinuousMemoryMananger, offset: "<<refFreeOffset()<<", size: "<<refSize()<<std::endl;

	os<<"  Waits:        "<<tsrMutexWaits<<std::endl;
	os<<"  Allocations:  "<<tsrAllocations<<std::endl;

  os<<std::endl;
}
/*************************************************************************/
void ContinuousMemoryMananger::dump(std::ostream& os){

  Mutex::Locker locker(refMutex());

	os<<"ContinuousMemoryMananger, offset: "<<refFreeOffset()<<", size: "<<refSize()<<std::endl;

  os<<"  Waits:        "<<tsrMutexWaits<<std::endl;
	os<<"  Allocations:  "<<tsrAllocations<<std::endl;

  for(EntriesList::const_iterator it = lstEntries.begin(); it != lstEntries.end(); it++){
    os<<"\t"<<(*it).pAddress<<"\t"<<(*it).iSize<<std::endl;
  }

  os<<std::endl;
}
/*************************************************************************/
void ContinuousMemoryMananger::trim(){
}
/*************************************************************************/
}
