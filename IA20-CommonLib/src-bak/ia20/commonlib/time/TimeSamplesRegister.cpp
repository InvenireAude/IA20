/*
 * File: IA20-CommonLib/src/commonlib/time/TimeSamplesRegister.cpp
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
#include <ia20/commonlib/exception/SystemException.h>

#include "TimeSamplesRegister.h"


namespace IA20{
/*************************************************************************/
void TimeSample::start(){
	if(clock_gettime(CLOCK_MONOTONIC,&tsStart)==-1)
		throw SystemException("TimeSamplesRegister clock_gettime");
};

/*************************************************************************/
long TimeSample::getSample()const{

		if(!tsStart.tv_sec)
			return 0L;

		struct timespec tsStop;

		if(clock_gettime(CLOCK_MONOTONIC,&tsStop)==-1)
			throw SystemException("TimeSamplesRegister clock_gettime");

		long iTimeElapsed=tsStop.tv_sec-tsStart.tv_sec;
		iTimeElapsed*=1000000;

		iTimeElapsed+=(tsStop.tv_nsec-tsStart.tv_nsec)/1000;

		return iTimeElapsed;
	}
/*************************************************************************/
}
