/*
 * File: TasksRing.h
 *
 * Copyright (C) 2021, Albert Krzymowski
 *

 */


#ifndef _IA20_IOT_Tools_ASM_x86_H_
#define _IA20_IOT_Tools_ASM_x86_H_

#define MPLOCKED        "lock ; "
#include <inttypes.h>

struct _ia20_spinlock_t{
	volatile uint16_t locked; 
    _ia20_spinlock_t():locked(0){};
};


static inline void
_ia20_spinlock_lock(_ia20_spinlock_t *sl){

	uint16_t lock_val = 1;
    uint16_t exp = 0;
    uint8_t res = 0;
   // uint8_t cnt = 0;

 //while(!res){
	asm volatile (
	"1:\n"
    "pause;"
    //"incl %[cnt];"
      MPLOCKED
	"cmpxchgw %[lock_val], %[dst];"
	"sete %[res];"
    "cmpb $0x0, %[res];"
    "jz 1b;"
			: [res] "=a" (res),     /* output */
			  [dst] "=m" (sl->locked)
     //   [cnt] "=m" (cnt)
			: [lock_val] "r" (lock_val),      /* input */
			  [exp] "a" (exp),
			  "m" (sl->locked)
			: "memory");
  //if(!res)
    // /std::cout<<"test"<<(int)cnt<<"\n";
 //}
}

static inline void
_ia20_spinlock_unlock (_ia20_spinlock_t *sl)
{
	uint16_t unlock_val = 0;
	asm volatile (
    MPLOCKED
			"xchgw %[locked], %[ulv]\n"
			: [locked] "=m" (sl->locked), [ulv] "=q" (unlock_val)
			: "[ulv]" (unlock_val)
			: "memory");
}



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

#endif /* _IA20_IOT_Tools_ASM_x86_H_ */

