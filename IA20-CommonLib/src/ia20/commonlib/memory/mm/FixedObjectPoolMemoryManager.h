/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/FixedObjectPoolMemoryManager.h
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
#ifndef _IA20_FixedObjectPoolMemoryManager_H_
#define _IA20_FixedObjectPoolMemoryManager_H_

#include "../ma/Allocator.h"

#include <ia20/commonlib/exception/ItemNotFoundException.h>
#include <ia20/commonlib/threads/Mutex.h>
#include <ia20/commonlib/logger/logger.h>

//TODO alignment std::max_align_t

namespace IA20 {

/*************************************************************************/
/** The FixedObjectPoolMemoryManager class.
 *
 */
class FixedObjectPoolMemoryManager : public Allocator {
public:

	virtual ~FixedObjectPoolMemoryManager()throw();

	virtual void* allocate(size_t n);
	virtual void  free(const void* p);
	virtual bool  check(const void* p);
    virtual void  trim();

	virtual void printToStream(std::ostream& os);

	void dump(std::ostream& os);

	static size_t ComputeMemoryRequirement(size_t iObjectSize, size_t iNumObjects);

	typedef uint32_t EntryIndex;
    static  const EntryIndex CUnusedIndex = 0xffffffff;

	template <class C>
		C* pointerFromIdx(EntryIndex iIdx){
			return reinterpret_cast<C*>(getStart() + (size_t)iIdx * refObjectSize());
		};

	template <class C>
		EntryIndex idxFromPointer(C* p){

			 if(!isPointerSane(p)){
   				IA20_THROW(ItemNotFoundException("FOMM, Invalid conversion from pointer to index."));
			 }
			return ((unsigned char*)p - getStart())/refObjectSize();
		};

protected:

	FixedObjectPoolMemoryManager(void *pMemory, size_t iObjectSize, size_t iNumObjects);
    FixedObjectPoolMemoryManager(void *pMemory);


	void*            pMemory;
  	bool             bFreeMe;

	struct Info {
	    EntryIndex       iTop;
    	size_t           iObjectSize;
  		size_t           iNumObjects;
		Mutex mutex;
		Info():
		 iTop(0),
		 iObjectSize(0),
		 iNumObjects(0),
		 mutex(true){}
	};

	inline EntryIndex&       refTop(){ return ((Info*)pMemory)->iTop; };
	inline unsigned char*    getStart(){ return ((unsigned char*)pMemory) + sizeof(Info); }
    inline size_t&           refObjectSize(){ return ((Info*)pMemory)->iObjectSize; };
  	inline size_t&           refNumObjects(){ return ((Info*)pMemory)->iNumObjects; };
	inline Mutex&            refMutex(){ return ((Info*)pMemory)->mutex; };

	inline const EntryIndex&       refTop()const{ return ((Info*)pMemory)->iTop; };
	inline const unsigned char*    getStart()const{ return ((unsigned char*)pMemory) + sizeof(Info); }
    inline const size_t&           refObjectSize()const{ return ((Info*)pMemory)->iObjectSize; };
  	inline const size_t&           refNumObjects()const{ return ((Info*)pMemory)->iNumObjects; };

	TimeSamplesResults  tsrMutexWaits;
	TimeSamplesResults  tsrAllocations;

	inline EntryIndex& indexEntry(size_t iIdx){
		return *(EntryIndex*)(getStart() + (size_t)iIdx * refObjectSize());
	}

	bool isPointerSane(const void *p) const;
};

/*************************************************************************/
}

#endif /* _IA20_FixedObjectPoolMemoryManager_H_ */
