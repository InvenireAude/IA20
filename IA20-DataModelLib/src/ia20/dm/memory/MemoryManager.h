/*
 * File: MemoryManager.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_DM_Memory_MemoryManager_H_
#define _IA20_DM_Memory_MemoryManager_H_

#include <ia20/commonlib/commonlib.h>

#include <stack>

using namespace IA20;

namespace IA20 {
namespace DM {
namespace Memory {

//TODO segment Provider with virtual allocation and free.

/*************************************************************************/
/** The MemoryManager class.
 *
 */
class MemoryManager {
public:

	virtual ~MemoryManager() throw();
	MemoryManager();

  typedef uint32_t SizeType;
  static  SizeType CSegmentSize;

  typedef uint64_t AddressIntegerType;
  static  AddressIntegerType CSegmentMask;

  template<class C>
    static C* AllocateLocally(void* pAddress){
      IA20_CHECK_IF_NULL(TheThreadLocal);
      return reinterpret_cast<C*>(TheThreadLocal->allocate(pAddress, sizeof(C)));
  };

  static void* AllocateLocally(void* pAddress, SizeType iSize){
       IA20_CHECK_IF_NULL(TheThreadLocal);
      return TheThreadLocal->allocate(pAddress, iSize);
  };

  void *allocate(void* pAddress, uint32_t iSize);
  void free(void* pAddress);

  static void AssureCreated(bool bRecycle = false){

    if(TheThreadLocal){
      delete TheThreadLocal;
      TheThreadLocal = NULL;
    }

    if(!TheThreadLocal)
      TheThreadLocal = new MemoryManager();
  }

protected:

  struct SegmentList {

    SegmentList *pPrev;
    SegmentList *pNext;
    uint32_t     iFreeOffset;

    inline SegmentList(SegmentList *pPrev=0, SegmentList *pNext = 0):
      pPrev(pPrev),
      pNext(pNext){};

  };

  SegmentList* allocateSegment();
  void freeSegment(SegmentList* pSegment);

  typedef std::stack<SegmentList*> SegmentsStack;
  SegmentsStack                    stackSegments;


  static inline SegmentList* GetSegmentAddress(void* pAddress){
      AddressIntegerType pValue = ((AddressIntegerType)pAddress) & CSegmentMask;
      return static_cast<SegmentList*>((void*)pValue);
  };

  static thread_local MemoryManager* TheThreadLocal;
};
/*************************************************************************/


  }
}
}

#endif /* _IA20_DM_Memory_MemoryManager_H_ */
