/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/MemoryManager.h
 *
 * Copyright (C) 2015, Albert Krzymowski
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
/* IA20_COPYRIGHT */

/* ChangeLog:
 *
 */

#ifndef _IA20_MemoryManager_H_
#define _IA20_MemoryManager_H_


#include <iostream>
#include <map>
#include <list>

#include "../ma/Allocator.h"
#include "../ma/Mallocator.h"

#include "../../threads/Mutex.h"
#include "../../sys/Signal.h"

namespace IA20{


/*************************************************************************/
/** The MemoryManager class.
 * One can dump the remaining allocated areas with printToStream() function.
 */

class MemoryManager :
	public Allocator
  //public SYS::Signal::UserSignalCallback
  {

public:
	MemoryManager(bool bAllowUserSignal = false, const char *sName = CDefaultName);
	virtual ~MemoryManager()throw();

	void addEntry(const char*   sFile,
			      const char*   sFun,
			      int           iLine,
			      unsigned long lPtr,
				  unsigned long iNumBytes);

	bool removeEntry(unsigned long lPtr);

	/** dumps the entries to the output stream */

	virtual void printToStream(std::ostream& os);

	void printToStream(std::ostream& os, bool bNewOnly, bool bStatsOnly = false);

	/** clears the "new" flag - use after allocating globals for example. */
	void clearNewFlag();

	inline static MemoryManager *GetInstance(){

		if(pInstance == NULL){
			pAllocator = pInstance = new (malloc(sizeof(MemoryManager))) MemoryManager(true, CMainName);
    }
		return pInstance;
	}

	void static SetDefaultAllocator(Allocator* pAllcator){
		MemoryManager::pAllocator=pAllcator;
	}

	 long  getNumerOfNew()const;

 	 virtual void* allocate(size_t n);
	 virtual void  free(const void* p);
	 virtual bool check(const void* p);
   virtual void trim();

	inline static Allocator *GetAllocator(){
			GetInstance();
			return pAllocator;
	}

  virtual void handleUserSignal();

  static const char* CDefaultName;
  static const char* CMainName;


 private:

	 struct Entry{
		const char *sFun;
		const char *sFile;
#ifdef __GLIBC__
    void *back_trace_array[10];
    int  back_trace_array_size;
    void printBackTrace(std::ostream& os) const;
#endif
		int        iLine;
		bool       bNewFlag;
		int        iTID;
		unsigned long lPtr;
		unsigned long iNumBytes;
	};

	typedef BareMap<size_t, Entry> EntryMap;

	static MemoryManager   *pInstance;
	static Allocator       *pAllocator;

	EntryMap 				hmEntries;

	unsigned long iTotalEntries;
	unsigned long iCurEntries;
	unsigned long iNewEntries;
	unsigned long iInstances;
	unsigned long iNumBytes;

	Mutex theLock;

	unsigned char *pMemory;
	int           iFree;
  const char    *sName;
	TimeSamplesResults  tsrMutexWaits;
  TimeSamplesResults trsMemoryTrim;

  void clearNewFlagNoLock();
};

template<class T >
 class MemoryManagerFactory : public AllocatorFactory<T, &::IA20::MemoryManager::GetAllocator> {};

};


#endif /* _IA20_MemoryManager_H_ */
