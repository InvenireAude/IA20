/*
 * File: IA20-CommonLib/src/commonlib/types/ProducerConsumerQueue.h
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

#ifndef _IA20_Types_SortedHeapWithDelete_H_
#define _IA20_Types_SortedHeapWithDelete_H_

#include <ia20/commonlib/types.h>
#include <ia20/commonlib/memory/memory.h>
#include <ia20/commonlib/exception/BadUsageException.h>
#include <ia20/commonlib/threads/Mutex.h>
#include <ia20/commonlib/threads/Condition.h>

#include "BufferQueue.h"

namespace IA20 {

template<class C, class TA = IA20_DFT_FACTORY<C> >
class ProducerConsumerQueue : protected BufferQueue<C, TA> {

public:
    ProducerConsumerQueue(C *pStart, size_t iLogSize):
      BufferQueue<C, TA> (pStart, iLogSize),
      mutex(true),
      cndConsumer(true),
      cndProducer(true){}

    ProducerConsumerQueue(size_t iLogSize):
      BufferQueue<C, TA> (iLogSize),
      mutex(true),
      cndConsumer(true),
      cndProducer(true){}

    static size_t ComputeMemoryRequirement(size_t iLogSize){
        return BufferQueue<C, TA>::ComputeMemoryRequirement(iLogSize);
    }

    void put(const C& aValue){

        Mutex::Locker locker(mutex);

        while(BufferQueue<C, TA>::full()){
            Thread::Cancellation ca(true);
            mutex.wait(cndProducer);
        }

        BufferQueue<C, TA>::put(aValue);
        cndConsumer.signal();
    }

    C get(){

        Mutex::Locker locker(mutex);

        while(BufferQueue<C, TA>::empty()){
            Thread::Cancellation ca(true);
            mutex.wait(cndConsumer);
        }

        C aValue = BufferQueue<C, TA>::get();
        cndProducer.signal();

        return aValue;
    }

    size_t size(){
        Mutex::Locker locker(mutex);
        return BufferQueue<C, TA>::iNumItems;
    }

protected:
    Mutex     mutex;
    Condition cndProducer;
    Condition cndConsumer;
};

}

#endif /* _IA20_Types_SortedHeapWithDelete_H_ */
