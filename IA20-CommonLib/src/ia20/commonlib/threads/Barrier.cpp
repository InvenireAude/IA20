/*
 * File: IA20-CommonLib/src/commonlib/threads/Barrier.cpp
 *
 * Copyright (C) 2021, Albert Krzymowski
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
#include "Barrier.h"

#include "../logger/logger.h"

namespace IA20 {

/*************************************************************************/
Barrier::Barrier(unsigned int iNumThreads, unsigned int iTimeoutMS, bool bShared):
  mutex(bShared),
  cnd(bShared),
  iValue(0),
  iNumThreads(iNumThreads),
  iTimeoutMS(iTimeoutMS),
  bFlip(false){
	IA20_TRACER;
}
/*************************************************************************/
Barrier::~Barrier() throw(){
	IA20_TRACER;


}
/*************************************************************************/
void Barrier::synchronize(){
	IA20_TRACER;

  Mutex::Locker locker(mutex);

  iValue++;

  if(iValue == iNumThreads){
    iValue = 0;
    cnd.broadcast();
  }else{
    mutex.wait(cnd, iTimeoutMS);
  }

}
/*************************************************************************/
}
