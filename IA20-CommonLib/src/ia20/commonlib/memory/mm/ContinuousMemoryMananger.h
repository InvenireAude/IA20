/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/ContinuousMemoryMananger.h
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
#ifndef _IA20_ContinuousMemoryMananger_H_
#define _IA20_ContinuousMemoryMananger_H_

#include "../ma/Allocator.h"
#include "MemoryManager.h"

#include<list>

#include <ia20/commonlib/threads/Mutex.h>

namespace IA20 {

/*************************************************************************/
/** The ContinuousMemoryMananger class.
 *
 */
class ContinuousMemoryMananger : public Allocator {
public:

	virtual ~ContinuousMemoryMananger()throw();

	virtual void* allocate(size_t n);
	virtual void  free(const void* p);
	virtual bool  check(const void* p);
    virtual void  trim();

	virtual void printToStream(std::ostream& os);

	void dump(std::ostream& os);
	static size_t ComputeMemoryRequirement(size_t iMemoryPoolSize);

protected:

	ContinuousMemoryMananger(void *pMemory, size_t iMemoryPoolSize);
	ContinuousMemoryMananger(void *pMemory);

	void*  pMemory;

	struct Info {
		size_t           iFreeOffset;
    	size_t           iSize;
		Mutex mutex;
	};

	inline size_t& refFreeOffset(){return ((Info*)pMemory)->iFreeOffset; };
    inline size_t& refSize(){return ((Info*)pMemory)->iSize; };

	inline const size_t& refFreeOffset()const {return ((Info*)pMemory)->iFreeOffset; };
    inline const size_t& refSize()const {return ((Info*)pMemory)->iSize; };

	inline unsigned char*    getStart(){ return ((unsigned char*)pMemory) + sizeof(Info); }
  	inline const unsigned char*  getStart()const{ return ((unsigned char*)pMemory) + sizeof(Info); }
  	inline Mutex&            refMutex(){ return ((Info*)pMemory)->mutex; };

	inline size_t getFree()const { return refSize() - refFreeOffset(); };

	inline void* getAddressAtStart(){ return getStart() + refFreeOffset(); }
	inline const void* getAddressAtStart()const{ return getStart() + refFreeOffset(); }

  	bool             bFreeMe;

	TimeSamplesResults  tsrMutexWaits;
	TimeSamplesResults  tsrAllocations;

	inline bool isPointerSane(const void *p) const;

	struct LogEntry{

		LogEntry(void* pAddress, size_t iSize):
			pAddress(pAddress), iSize(iSize){};

		void* pAddress;
		size_t iSize;
	};

	typedef std::list<LogEntry> EntriesList;

	EntriesList lstEntries;

};

/*************************************************************************/
}

#endif /* _IA20_ContinuousMemoryMananger_H_ */
