/*
 * File: IA20-CommonLib/src/commonlib/memory/mm/MemoryManager.cpp
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
/* ChangeLog:
 *
 */

#include "MemoryManager.h"
#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/logger/StackTrace.h>
#include <ia20/commonlib/logger/StackTraceContainer.h>
#include <ia20/commonlib/memory/memory.h>
#include <ia20/commonlib/tools/MiscTools.h>

#include <stdlib.h>
#include <cstring>

#ifdef __GLIBC__
#include <execinfo.h>
#include <malloc.h>
#endif

namespace IA20 {

MemoryManager* MemoryManager::pInstance = NULL;
Allocator*     MemoryManager::pAllocator = NULL;

const char*    MemoryManager::CDefaultName = "Default";
const char*    MemoryManager::CMainName = "Main";

/*************************************************************************/

void MemoryManager::Entry::printBackTrace(std::ostream& os) const{

  char **strings;
  size_t i;

  strings = backtrace_symbols (back_trace_array, back_trace_array_size);

  for (i = 0; i < back_trace_array_size; i++)
     os<<strings[i]<<std::endl;

  ::free (strings);
}

/*************************************************************************/
MemoryManager::MemoryManager(bool bAllowUserSignal, const char* sName):
   //SYS::Signal::UserSignalCallback(bAllowUserSignal),
	iNewEntries(0),
	pMemory(0),
	iFree(0),
	iTotalEntries(0),
	iCurEntries(0),
	iInstances(0),
  sName(sName){

	//iFree=10000000;
	//pMemory = new unsigned char[iFree];

	//this->bCheckPointersOnReturn=false;
}
/*************************************************************************/
MemoryManager::~MemoryManager() throw () {
}
/*************************************************************************/
void MemoryManager::clearNewFlagNoLock() {

	for (EntryMap::iterator iter = hmEntries.begin(); iter != hmEntries.end(); iter++) {
		iter->second.bNewFlag = false;
	}
	iNewEntries = 0;
}
/*************************************************************************/
void MemoryManager::clearNewFlag() {

	IA20_LOG(LogLevel::INSTANCE.isMemory(),"Clearing New Flag.");
	Mutex::Locker locker(theLock);

  clearNewFlagNoLock();
}
/*************************************************************************/
void MemoryManager::addEntry(const char* sFile, const char* sFun, int iLine, unsigned long lPtr, unsigned long iNumBytes) {

	Entry entry;

	entry.sFile = sFile;
	entry.sFun = sFun;
	entry.iLine = iLine;
	entry.bNewFlag = true;
	entry.lPtr = lPtr;
	entry.iNumBytes = iNumBytes;
#ifdef __GLIBC__
  void *back_trace_array[12];
  entry.back_trace_array_size = backtrace(back_trace_array,12) - 2;
  memcpy(entry.back_trace_array, back_trace_array + 2, entry.back_trace_array_size*sizeof(void*));
#endif

	bool bCheck = false;
	{
		Mutex::Locker locker(theLock);
		if (!(bCheck = hmEntries.count(lPtr) > 0)) {
			hmEntries[lPtr] = entry;
			iTotalEntries++;
			iCurEntries++;
			iNewEntries++;
			this->iNumBytes+=iNumBytes;
		}
	}

	if (bCheck) {
		IA20_LOG(LogLevel::INSTANCE.isError(),"Duplicated entry: "<<((void*)lPtr));
#ifdef __GLIBC__
      entry.printBackTrace(std::cerr);
#endif
	}

	IA20_LOG(IA20::LogLevel::INSTANCE.isMemory(), "N["<<(void*)lPtr<<"]:"<<iNumBytes<<","<<sFile<<","<<sFun<<"("<<iLine<<"),s="<<hmEntries.size()
			<<":"<<iCurEntries<<":"<<iTotalEntries);

  // if(IA20::LogLevel::INSTANCE.isMemory()){
  //   entry.printBackTrace(std::cerr);
  // }

}

/*************************************************************************/
bool MemoryManager::removeEntry(unsigned long lPtr) {

	IA20_LOG(IA20::LogLevel::INSTANCE.isMemory(), "R["<<(void*)lPtr<<"]");

	int iCount = 0;
	{
		Mutex::Locker locker(theLock);
		iCount = hmEntries.count(lPtr);
		if (iCount > 0) {
			Entry& entry(hmEntries[lPtr]);

			if (entry.bNewFlag)
				iNewEntries--;

			this->iNumBytes-=entry.iNumBytes;

			hmEntries.erase(lPtr);
			iCurEntries--;
		}
	}


	if (iCount == 0) {
		IA20_LOG(LogLevel::INSTANCE.isError(),"Missing entry [remove]: "<<((void*)lPtr));
		IA20_LOG(LogLevel::INSTANCE.isStackTrace(),"Stack: ");
		if (LogLevel::INSTANCE.isStackTrace()) {
      #ifdef __GLIBC__
	      IA20::PrintTrace(std::cerr);
      #else
	      IA20_MY_STACK().printStack(std::cerr);
      #endif
		}

		if(getenv("IA20_MEM_DUMP_ON_MISSING_ENTRY") != NULL){
			std::cerr<<"core dump as requested "<<std::endl;
			abort();
		}
	}

	return iCount != 0;
}
/*************************************************************************/
long MemoryManager::getNumerOfNew() const {
	IA20_TRACER;
	return iNewEntries;
}
/*************************************************************************/
void MemoryManager::printToStream(std::ostream& os) {
	printToStream(os,false,false);
}
/*************************************************************************/
void MemoryManager::printToStream(std::ostream& os, bool bNewOnly, bool bStatsOnly) {

	Mutex::Locker locker(theLock);

	os << " >>>  MEMORY USAGE: " <<sName<< std::endl;
	os << " >>>  Total:     " << iTotalEntries << std::endl;
	os << " >>>  Bytes:     " << iNumBytes << std::endl;
	os << " >>>  Current:   " << iCurEntries << "," << hmEntries.size() << std::endl;
	os << " >>>  New:       " << iNewEntries << std::endl;

	if(!bStatsOnly){

		EntryMap::iterator iter;


		for (iter = hmEntries.begin(); iter != hmEntries.end(); iter++) {

			Entry& entry = iter->second;

			if (!bNewOnly || entry.bNewFlag) {
				os << (entry.sFile) << ":" << (entry.sFun) << "(" << entry.iLine << "): ";
				os << "\t [" << ((void*) entry.lPtr) << "]";
				os << "(" << ((unsigned int) entry.iTID) << ")";
				if (entry.bNewFlag)
					os << " N";
				os << std::endl;
			}

#ifdef __GLIBC__
      entry.printBackTrace(os);
#endif
		}/* for */

		os << std::endl;

	}/* !bStatsOnly */

#ifdef __GLIBC__


    struct Stats{
      long  iNumBytes;
      long  iNumEntries;
      Entry* pEntry;
    };

    typedef std::map<long, Stats, std::less<long>, Mallocator<std::pair<const long, Stats> > > EntryStatMap;
    EntryStatMap hmStats;

		 for (EntryMap::iterator iter = hmEntries.begin(); iter != hmEntries.end(); iter++){
       long iHash = 0;
		 	Entry& entry = iter->second;
	    if (!bNewOnly || entry.bNewFlag) {
       for(int i=0; i<entry.back_trace_array_size; i++){
        iHash = 17 * iHash + 13 * (long)entry.back_trace_array[i];
       }

       if(hmStats.find(iHash) == hmStats.end()){
          hmStats[iHash].iNumBytes = entry.iNumBytes;
          hmStats[iHash].iNumEntries = 1;
          hmStats[iHash].pEntry = &entry;
       }else{
          hmStats[iHash].iNumBytes += entry.iNumBytes;
          hmStats[iHash].iNumEntries++;
       }
      }
    }

		for (EntryStatMap::iterator iter = hmStats.begin(); iter != hmStats.end(); iter++){
			Stats& stat = iter->second;
      os<<"\nBytes: "<<stat.iNumBytes<<", entries: "<<stat.iNumEntries<<std::endl;

      os<<" First 64 bytes hex: ";
      for(int i=0; i < stat.pEntry->iNumBytes && i < 64; i++){
        os<<MiscTools::CharToHex(((char*)stat.pEntry->lPtr)[i]);
      }
      os<<std::endl;
      os<<" First 64 bytes raw: ";
      for(int i=0; i < stat.pEntry->iNumBytes && i < 64; i++){
        os<<(String(" ") + ((char*)stat.pEntry->lPtr)[i]);
      }
      os<<std::endl;
      stat.pEntry->printBackTrace(os);
    };


#endif
		os << std::endl;


  clearNewFlagNoLock();
	os<<"Waits:  "<<tsrMutexWaits<<std::endl;

#ifdef __GLIBC__

  if(sName == CMainName){

    struct mallinfo info = mallinfo();
    os<<"System glibc malloc() stats:"<<std::endl;
    os<<" arena:  "<<info.arena<<std::endl;
    os<<" ordblks:  "<<info.ordblks<<std::endl;
    os<<" smblks:   "<<info.smblks<<std::endl;
    os<<" hblks:    "<<info.hblks<<std::endl;
    os<<" hblkhd:   "<<info.hblkhd<<std::endl;
    os<<" usmblks:  "<<info.usmblks<<std::endl;
    os<<" fsmblks:  "<<info.fsmblks<<std::endl;
    os<<" uordblks: "<<info.uordblks<<std::endl;
    os<<" fordblks: "<<info.fordblks<<std::endl;
    os<<" keepcost: "<<info.keepcost<<std::endl;
    os<<std::endl;

    // malloc_info(0,stderr);
    // malloc_trim(1024);
  }
#endif
}

/*************************************************************************/
void* MemoryManager::allocate(size_t iNumBytes) {

    void* p = malloc(iNumBytes);

	/*if(iFree < iNumBytes){
		std::cerr<<"OUT OF MEMORY!!!"<<std::endl;
		throw MemoryException(errno);
	}

	void *p=pMemory;
	iFree-=iNumBytes;
	pMemory+=iNumBytes;
    */
    if(p == NULL)
       throw MemoryException(errno);

	if (::IA20::LogLevel::INSTANCE.isMemoryTrace()) {

		const StackTrace& stack =	IA20_MY_STACK();

		if (!stack.empty()) {
			const StackTrace::Entry& entry = stack.top();
			addEntry(entry.sFile, entry.sFun, entry.iLine, (unsigned long) p, iNumBytes);
		} else {
  addEntry("noline", "nofun", 1, (unsigned long) p, iNumBytes);
		}

	}

	return p;

}
/*************************************************************************/
bool MemoryManager::check(const void* p) {

	if (!::IA20::LogLevel::INSTANCE.isMemoryTrace()) {
		IA20_LOG(IA20::LogLevel::INSTANCE.isMemory(), "Check is not working in FAST_AND_FURIOUS mode["<<(void*)p<<"]");
		return true;
	}

	unsigned long lPtr=(unsigned long)p;

	IA20_LOG(IA20::LogLevel::INSTANCE.isMemory(), "C["<<(void*)lPtr<<"]");

	int iCount = 0;
	{
		Mutex::Locker locker(theLock,tsrMutexWaits);
		iCount = hmEntries.count(lPtr);
	}

	if (iCount == 0) {
		IA20_LOG(LogLevel::INSTANCE.isError(),"Missing entry[check]: "<<((void*)lPtr));
		IA20_LOG(LogLevel::INSTANCE.isStackTrace(),"Stack: ");
		if (LogLevel::INSTANCE.isStackTrace()) {
		  #ifdef __GLIBC__
	      IA20::PrintTrace(std::cerr);
      #else
	      IA20_MY_STACK().printStack(std::cerr);
      #endif
		}

		if(getenv("IA20_MEM_DUMP_ON_MISSING_ENTRY") != NULL){
			std::cerr<<"core dump as requested "<<std::endl;
			abort();
		}
	}

	return iCount != 0;
}
/*************************************************************************/
void MemoryManager::free(const void* p) {

	if (::IA20::LogLevel::INSTANCE.isMemoryTrace() && !removeEntry((unsigned long) p)){
		IA20_LOG(LogLevel::INSTANCE.isError(),"Missing entry [free]: "<<((void*)p));
		  #ifdef __GLIBC__
	      IA20::PrintTrace(std::cerr);
      #else
	      IA20_MY_STACK().printStack(std::cerr);
      #endif

		if(getenv("IA20_MEM_DUMP_ON_MISSING_ENTRY") != NULL){
			std::cerr<<"core dump as requested "<<std::endl;
			abort();
		}

	}else{
		::free( const_cast<void*>(p));
	}
}
/*************************************************************************/
void MemoryManager::trim(){
#ifdef __GLIBC__
  Mutex::Locker locker(theLock);
  TimeSample ts(true);
  malloc_trim(1024);
  trsMemoryTrim.addSample(ts);
  IA20_LOG(LogLevel::INSTANCE.isProfile(),"Memory trim: "<<trsMemoryTrim);
#endif
}
/*************************************************************************/
void MemoryManager::handleUserSignal(){
    printToStream(std::cerr,true,true);
}
/*************************************************************************/
}/* namespace IA20 */
