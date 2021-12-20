/*
 * File: TasksRing.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SPIN_TasksRing_H_
#define _IA20_IOT_Tools_SPIN_TasksRing_H_

#include <ia20/commonlib/commonlib.h>
#include <ia20/commonlib/sys/Signal.h>

#include "../TasksRing.h"


#include <vector>

#define MPLOCKED        "lock ; "

struct rte_spinlock_t{
	volatile uint16_t locked; /**< lock status 0 = unlocked, 1 = locked */
    rte_spinlock_t():locked(0){};
};

static inline void
rte_spinlock_lock2(rte_spinlock_t *sl)
{
	uint16_t lock_val = 1;
  uint16_t exp = 0;
	asm volatile (
			"1:\n"
      MPLOCKED
			"cmpxchgw %[lock_val], %[dst];"
			"jz 2f;"
			"pause;"
			"jmp 1b;"
			"2:\n"
			:  /* output */
			  [dst] "=m" (sl->locked)
			: [lock_val] "r" (lock_val),      /* input */
			  "a" (exp),
			  "m" (sl->locked)
			: "memory");
}

static inline void
rte_spinlock_lock(rte_spinlock_t *sl)
{
	uint16_t lock_val = 1;
  uint16_t exp = 0;
  uint8_t res = 0;
  uint8_t cnt = 0;

 //while(!res){
	asm volatile (
		"1:\n"
      "pause;"
    "incl %[cnt];"
      MPLOCKED
			"cmpxchgw %[lock_val], %[dst];"
			"sete %[res];"
      "cmpb $0x0, %[res];"
      "jz 1b;"
			: [res] "=a" (res),     /* output */
			  [dst] "=m" (sl->locked),
        [cnt] "=m" (cnt)
			: [lock_val] "r" (lock_val),      /* input */
			  [exp] "a" (exp),
			  "m" (sl->locked)
			: "memory");
  if(!res)
    std::cout<<"test"<<(int)cnt<<"\n";
 //}
}

static inline void
rte_spinlock_unlock (rte_spinlock_t *sl)
{
	uint16_t unlock_val = 0;
	asm volatile (
               MPLOCKED
			"xchgw %[locked], %[ulv]\n"
			: [locked] "=m" (sl->locked), [ulv] "=q" (unlock_val)
			: "[ulv]" (unlock_val)
			: "memory");
}



class SpinLock{
    rte_spinlock_t sp ;
public:
    static volatile int iCounter;
    void lock() {
       rte_spinlock_lock(&sp);
       if(!sp.locked){
         std::cerr<<(void*)this<<"failed:"<<iCounter<<std::endl;
       }
    }
    void unlock() {
      if(!sp.locked){
         std::cerr<<(void*)this<<"failed2:"<<iCounter<<std::endl;
       }
       rte_spinlock_unlock(&sp);
    }
};

volatile int SpinLock::iCounter = 0;

static inline int
rte_atomic16_cmpset(volatile uint16_t *dst, uint16_t exp, uint16_t src)
{
	uint8_t res;

	asm volatile(
             MPLOCKED
			"cmpxchgw %[src], %[dst];"
			"sete %[res];"
			: [res] "=a" (res),     /* output */
			  [dst] "=m" (*dst)
			: [src] "r" (src),      /* input */
			  "a" (exp),
			  "m" (*dst)
			: "memory");            /* no-clobber list */
	return res;
}

static inline int rte_atomic16_test_and_set(volatile uint16_t *v)
{
	return rte_atomic16_cmpset(v, 0, 1);
}

static inline uint16_t
rte_atomic16_exchange(volatile uint16_t *dst, uint16_t val)
{
	asm volatile(
			MPLOCKED
			"xchgw %0, %1;"
			: "=r" (val), "=m" (*dst)
			: "0" (val),  "m" (*dst)
			: "memory");         /* no-clobber list */
	return val;
}

inline int power_pause(){
	asm volatile(".byte 0xF3, 0x90;");
	return 0;
}

class SpinLock1{
    volatile uint16_t locked = 0 ;
public:
    void lock() {
        struct timespec tp;
        int iCount = 0;
        while (!rte_atomic16_test_and_set(&locked)) {
            if(++iCount > 100){
                power_pause();
                iCount = 1000;
                // clock_gettime(CLOCK_MONOTONIC,&tp);
            }
         }
         if(!locked){
          std::cerr<<"failed"<<std::endl;
       }

    }
    void unlock() {
        rte_atomic16_exchange(&locked,0);
    }
};

class SpinLock2 {
    std::atomic_flag locked = ATOMIC_FLAG_INIT ;
public:
    void lock() {
        int iCount = 0;
        while (locked.test_and_set(std::memory_order_acquire)) {
            if(++iCount > 2){
                iCount = 0;
                //sched_yield();
            }; }
    }
    void unlock() {
        locked.clear(std::memory_order_release);
    }
};

namespace IA20 {
namespace IOT {
namespace Tools {
namespace SPIN {

/*************************************************************************/
/** The TasksRing class.
 *
 */
template<class D>
class TasksRing : public IA20::IOT::Tools::TasksRing<D> {

SpinLock sp;
public:

	virtual ~TasksRing() throw(){
  };

/*************************************************************************/
TasksRing(unsigned int iSize):
  iSize(iSize),
  iLength(0),
  iHead(0){
	IA20_TRACER;
}
/*************************************************************************/
virtual void enque(D* pValue){
	IA20_TRACER;

  while(!IA20::SYS::Signal::GetInstance()->isStopping()){
    Thread::Cancellation::Test();
    sp.lock();
    if(iSize != iLength + 1){
      vValues[ (iHead + iLength++) % iSize ] = pValue;
//      IA20_LOG(true, "Enque done: "<<(void*)this<<"\t"<<iHead<<", "<<iLength<<"]\t = "<<(void*)pValue);
      sp.unlock();
      return;
    }
    sp.unlock();
  }

}
/*************************************************************************/
virtual D *deque(){
	IA20_TRACER;

  while(!IA20::SYS::Signal::GetInstance()->isStopping()){
    Thread::Cancellation::Test();
    sp.lock();
    sp.iCounter++;
    if(iLength > 0){

      D *pValue = vValues[ iHead ];

      if(iHead < iSize - 1){
        iHead++;
      }else{
        iHead = 0;
      }
      iLength--;
//      IA20_LOG(true, "Deque done: "<<(void*)this<<"["<<iHead<<","<<iLength<<"]\t = "<<(void*)pValue);
      sp.unlock();
      return pValue;
    }
    sp.unlock();
  }

  IA20_THROW(ItemNotFoundException("Deque timeout"));
}

/*************************************************************************/
  static std::unique_ptr< typename Tools::TasksRing<D>::Interface> CreateInterface(unsigned int iSize){

     std::unique_ptr<typename Tools::TasksRing<D> > ptr1(new TasksRing(iSize));
     std::unique_ptr<typename Tools::TasksRing<D> > ptr2(new TasksRing(iSize));

    return std::unique_ptr< typename Tools::TasksRing<D>::Interface>(
        new typename Tools::TasksRing<D>::Interface(std::move(ptr1),std::move(ptr2)));

  };

/*************************************************************************/
protected:


  D* volatile vValues[1024];

  volatile unsigned int iHead;
  volatile unsigned int iLength;
  volatile unsigned int iSize;

};
/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SPIN_TasksRing_H_ */

