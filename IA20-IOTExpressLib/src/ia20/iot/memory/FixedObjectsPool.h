/*
 * File: FixedObjectsPool.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Memory_FixedObjectsPool_H_
#define _IA20_IOT_Memory_FixedObjectsPool_H_

#include <ia20/commonlib/commonlib.h>

namespace IA20 {
namespace IOT {
namespace Memory {

/*************************************************************************/
/** The FixedObjectsPool class.
 *
 */
//TODO array of entry[] segments, index % SegmentSize

template<class C, size_t CSize>
class FixedObjectsPool {
public:

	~FixedObjectsPool() throw(){
		// hopefully there is nothing allocated (TODO sanity checks ?)
		if(tEntries)
			delete reinterpret_cast<uint8_t*>(tEntries);
	};
	
protected:

	static const int32_t CEmpty = -1;
	int32_t iNextFreeIdx;

public:

	FixedObjectsPool():
		iNumEntries(0),
		tEntries(nullptr){
		//TODO memory provider and some check and error reporting.
		tEntries = reinterpret_cast<C*>(new uint8_t[sizeof(C) * CSize]);
		for(uint32_t i = 0; i<CSize; i++){
			*reinterpret_cast<int32_t*>(tEntries + i) = iNextFreeIdx = i == 0 ? CEmpty : i - 1;
		};

		iNextFreeIdx = CSize-1;

	}

	C* allocate(){
		
		if(iNextFreeIdx == CEmpty)
			throw ItemNotFoundException("no memory - TODO better exception");

		C* pResult = tEntries + iNextFreeIdx;
		iNextFreeIdx = *reinterpret_cast<int32_t*>(pResult);
		iNumEntries++;
		return pResult;
	}

	void free(C *p){
		int32_t iIdx = p - tEntries;
		*reinterpret_cast<int32_t*>(tEntries + iIdx) = iNextFreeIdx;
		iNextFreeIdx = iIdx;
		iNumEntries--;
	}


	static size_t PointerToIdx(C* p){
		int32_t iIdx = p - TheInstance.tEntries;
		if(iIdx < 0 || iIdx >= CSize){
			return ~0L;
		}
		return iIdx;
	}

	static C* IdxToPonter(int32_t iIdx){	
		if(iIdx < 0 || iIdx >= CSize)
			return NULL;	
		return TheInstance.tEntries + iIdx;
	}

	static FixedObjectsPool TheInstance;
	
	inline static void Free(C* p){
		TheInstance.free(p);
	}
	
	inline static C* Allocate(){
		return TheInstance.allocate();
	}

	template<typename... Args>
	inline static C* New(Args... args){
		return new(TheInstance.allocate())C(args...);
	}

	inline static C* New(){
		return new(TheInstance.allocate())C();
	}

	struct Deleter {
  		void operator()(C* c) { 
			  c->~C();
			  Free(c); 
			}
	};

	typedef std::unique_ptr<C, Deleter > unique_ptr;

	C* tEntries;
	uint32_t iNumEntries;
};

template<class C, size_t CSize>
FixedObjectsPool<C,CSize> FixedObjectsPool<C,CSize>::TheInstance;
/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Memory_FixedObjectsPool_H_ */
