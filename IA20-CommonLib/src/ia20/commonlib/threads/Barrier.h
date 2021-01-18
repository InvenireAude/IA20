/*
 * File: IA20-CommonLib/src/commonlib/threads/Barrier.h
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
#ifndef _IA20_THREAD_Barrier_H_
#define _IA20_THREAD_Barrier_H_

/*
 *
 */

#include "ThreadLockException.h"
#include <ia20/commonlib/time/TimeSamplesRegister.h>

#include "Mutex.h"
#include "Condition.h"

namespace IA20 {

class Barrier  {
public:
	Barrier(unsigned int iNumThreads, unsigned int iTimeoutMS, bool bShared=false);
	~Barrier()throw();

	void synchronize();


protected:

  volatile bool         bFlip;
  volatile unsigned int iValue;
  unsigned int iNumThreads;
  unsigned int iTimeoutMS;

	Mutex     mutex;
  Condition cnd;
};

}

#endif /* _IA20_THREAD_Barrier_H_ */
