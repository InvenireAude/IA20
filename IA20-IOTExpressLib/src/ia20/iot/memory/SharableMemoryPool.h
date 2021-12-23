/*
 * File: SharableMemoryPool.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Memory_SharableMemoryPool_H_
#define _IA20_IOT_Memory_SharableMemoryPool_H_

#include <ia20/commonlib/commonlib.h>

#include <stack>
#include <functional>

#include "../tools/spin/SpinLock.h"


using namespace IA20;

namespace IA20 {
namespace IOT {
namespace Memory {

/*************************************************************************/
/** The SharableMemoryPool class.
 *
 */
class SharableMemoryPool {
public:

	virtual ~SharableMemoryPool() throw();
	SharableMemoryPool();

  typedef uint32_t SizeType;
  static  SizeType CSegmentSize;
  static  SizeType CSegmentDataSize;

  typedef int32_t OffsetType;
  static  uint64_t CSegmentMask;

  
  template<class C>
     C* allocate(void* pAddress){
      return reinterpret_cast<C*>(allocate(pAddress, sizeof(C)));
  };

  class Deleter {
    public:
      Deleter(SharableMemoryPool* pPool):
        pPool(pPool){};

      template<class T>
        void operator()(T *p){                
          pPool->free(p);
      }
   private:
    SharableMemoryPool* pPool;
  };

  template<typename T>
  using unique_ptr = std::unique_ptr<T, Deleter>;

  inline Deleter& getDeleter(){
    return mDeleter;
  }

  void *allocate(void* pAddress, uint32_t iSize);
  void *allocate(void* pAddress, uint32_t iMinSize, uint32_t iMaxSize, uint32_t& iBytesAllocated);

  void free(void* pAddress);

 struct Stats {
    uint64_t iMaxSegments;
    uint64_t iNumAllocations;

    void reset(){
      iMaxSegments = 0;
      iNumAllocations = 0;
    }
    
    friend
      std::ostream& operator<<(std::ostream& os, const Stats& s);
  };

protected:

  Deleter mDeleter;

  struct SegmentList {

    OffsetType   iOffsetNext;
    SizeType     iUsed;

    inline SegmentList(OffsetType pOffsetNext = 0):
      iOffsetNext(pOffsetNext),
      iUsed(0){};

    inline SegmentList* getNext()const{
      return const_cast<SegmentList*>(this + iOffsetNext);
    }

    inline void setNext(const SegmentList* pNext){
      iOffsetNext = pNext - this;
    }

    inline bool isSingle()const{
      return !iOffsetNext;
    }

  };

  SegmentList* allocateSegment();
  void freeSegment(SegmentList* pSegment);

  SegmentList *pFreeSegments;

  static inline SegmentList* GetSegmentAddress(void* pAddress){      
      uint64_t pValue = ((uint64_t)pAddress) & CSegmentMask;      
      return static_cast<SegmentList*>((void*)pValue);
  };

  Tools::SPIN::SpinLock mLock;

  Stats stats;
};
/*************************************************************************/


  }
}
}

#endif /* _IA20_IOT_Memory_SharableMemoryPool_H_ */
