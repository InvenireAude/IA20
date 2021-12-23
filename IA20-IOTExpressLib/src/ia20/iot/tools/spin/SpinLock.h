/*
 * File: TasksRing.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_SPIN_SpinLock_H_
#define _IA20_IOT_Tools_SPIN_SpinLock_H_

#include <ia20/commonlib/commonlib.h>

#include "../asm/asm.h"


namespace IA20 {
namespace IOT {
namespace Tools {
namespace SPIN {

/*************************************************************************/
//TODO some options with templates, system alternatives.

class SpinLock{
    _ia20_spinlock_t sp ;
public:
   
   inline void lock() {
       _ia20_spinlock_lock(&sp);
    }

   inline void unlock() {
       _ia20_spinlock_unlock(&sp);
    }
};

class Locker{
  
  public:
   inline Locker(SpinLock& sp):sp(sp){
      sp.lock();
    };

   inline ~Locker(){
      sp.unlock();
    }

  protected:
    SpinLock& sp;
};

// static inline int
// _ia20_atomic16_cmpset(volatile uint16_t *dst, uint16_t exp, uint16_t src)
// {
// 	uint8_t res;

// 	asm volatile(
//              MPLOCKED
// 			"cmpxchgw %[src], %[dst];"
// 			"sete %[res];"
// 			: [res] "=a" (res),     /* output */
// 			  [dst] "=m" (*dst)
// 			: [src] "r" (src),      /* input */
// 			  "a" (exp),
// 			  "m" (*dst)
// 			: "memory");            /* no-clobber list */
// 	return res;
// }

// static inline int _ia20_atomic16_test_and_set(volatile uint16_t *v)
// {
// 	return _ia20_atomic16_cmpset(v, 0, 1);
// }

// static inline uint16_t
// _ia20_atomic16_exchange(volatile uint16_t *dst, uint16_t val)
// {
// 	asm volatile(
// 			MPLOCKED
// 			"xchgw %0, %1;"
// 			: "=r" (val), "=m" (*dst)
// 			: "0" (val),  "m" (*dst)
// 			: "memory");         /* no-clobber list */
// 	return val;
// }

// inline int power_pause(){
// 	asm volatile(".byte 0xF3, 0x90;");
// 	return 0;
// }

// class SpinLock1{
//     volatile uint16_t locked = 0 ;
// public:
//     void lock() {
//         struct timespec tp;
//         int iCount = 0;
//         while (!_ia20_atomic16_test_and_set(&locked)) {
//             if(++iCount > 100){
//                 power_pause();
//                 iCount = 1000;
//                 // clock_gettime(CLOCK_MONOTONIC,&tp);
//             }
//          }
//          if(!locked){
//           std::cerr<<"failed"<<std::endl;
//        }

//     }
//     void unlock() {
//         _ia20_atomic16_exchange(&locked,0);
//     }
// };

// class SpinLock2 {
//     std::atomic_flag locked = ATOMIC_FLAG_INIT ;
// public:
//     void lock() {
//         int iCount = 0;
//         while (locked.test_and_set(std::memory_order_acquire)) {
//             if(++iCount > 2){
//                 iCount = 0;
//                 //sched_yield();
//             }; }
//     }
//     void unlock() {
//         locked.clear(std::memory_order_release);
//     }
// };

/*************************************************************************/
}
}
}
}

#endif /* _IA20_IOT_Tools_SPIN_SpinLock_H_ */

