/*
 * File: FixedObjectsTable.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Memory_FixedObjectsTable_H_
#define _IA20_IOT_Memory_FixedObjectsTable_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/iot/logger/LogLevel.h>

namespace IA20 {
namespace IOT {
namespace Memory {

/*************************************************************************/
/** The FixedObjectsTable class.
 *
 */
//TODO array of entry[] segments, index % SegmentSize

template<class C>
class FixedObjectsTable {
public:

	~FixedObjectsTable() throw(){
		// hopefully there is nothing allocated (TODO sanity checks ?)
		if(tEntries)
			delete reinterpret_cast<uint8_t*>(tEntries);
		if(tIsValid)
			delete reinterpret_cast<uint8_t*>(tIsValid);
	};

protected:

	static const int32_t CEmpty = -1;
	int32_t iNextFreeIdx;
	int32_t iSize;

public:

	FixedObjectsTable(int32_t iSize):
		iNumEntries(0),
		iSize(iSize),
		tEntries(nullptr){
		//TODO memory provider and some check and error reporting.
		tEntries = reinterpret_cast<C*>(new uint8_t[sizeof(C) * iSize]);
		tIsValid = reinterpret_cast<uint8_t*>(new uint8_t[iSize]); //TODO bits

		for(uint32_t i = 0; i<iSize; i++){
			*reinterpret_cast<int32_t*>(tEntries + i) =  i == iSize - 1 ? CEmpty : i + 1;
			tIsValid[i] = false;
		};

		iNextFreeIdx = 0;

	}

	C* allocate(){

		if(iNextFreeIdx == CEmpty)
			throw ItemNotFoundException("no memory - TODO better exception");

		C* pResult = tEntries + iNextFreeIdx;
		tIsValid[iNextFreeIdx] = true;

		iNextFreeIdx = *reinterpret_cast<int32_t*>(pResult);
		iNumEntries++;

		return pResult;
	}

	bool isValid(int32_t iIdx){
		if(iIdx < 0 || iIdx >= iSize)
			return false;
		return tIsValid[iIdx];
	}

	void free(int32_t iIdx){

		if(iIdx < 0 || iIdx >= iSize || !tIsValid[iIdx])
			return;

		*reinterpret_cast<int32_t*>(tEntries + iIdx) = iNextFreeIdx;
		tIsValid[iIdx] = false;
		iNextFreeIdx = iIdx;
		iNumEntries--;
	}


	size_t pointerToIdx(C* p){
		int32_t iIdx = p - tEntries;
		if(iIdx < 0 || iIdx >= iSize){
			return ~0L;
		}
		return iIdx;
	}

	C* idxToPointer(int32_t iIdx){
		if(iIdx < 0 || iIdx >= iSize || !tIsValid[iIdx])
			return NULL;
		IA20_LOG(IOT::LogLevel::INSTANCE.bIsMemory, "IdxToPonter: "<<iIdx<<" "<<(tEntries + iIdx));
		return tEntries + iIdx;
	}



	template<typename... Args>
	inline C* allocate(Args... args){
		return new(allocate())C(args...);
	}



	C* tEntries;
	uint8_t* tIsValid;
	uint32_t iNumEntries;

};

/*************************************************************************/
}
}
}

#endif /* _IA20_IOT_Memory_FixedObjectsTable_H_ */
