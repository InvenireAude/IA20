/*
 * File: IA20-CommonLib/src/commonlib/threads/Condition.cpp
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
#include "Condition.h"
#include "Mutex.h"

#include "../logger/logger.h"
#include "../memory/memory.h"
#include "../tools/TypeTools.h"

#include "string.h"

namespace IA20 {

/*************************************************************************/
Condition::Condition(bool bShared){

	IA20_TRACER;

	int iResult = -1;

	pthread_condattr_t cond_attr;

	if ((iResult = pthread_condattr_init(&cond_attr)) != 0)
		IA20_THROW(SystemException("pthread_condattr_init", iResult));

	if (bShared) {
		if ((iResult = pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED)) != 0)
			IA20_THROW(SystemException("PTHREAD_PROCESS_SHARED", iResult));
	}

	if ((iResult = pthread_condattr_setclock(&cond_attr, CLOCK_MONOTONIC)) != 0)
				IA20_THROW(SystemException("CLOCK_MONOTONIC", iResult));

	if((iResult = pthread_cond_init(&theCond, &cond_attr)) != 0){
		IA20_THROW(SystemException("Error memory condition creation",iResult));
	}

}
/*************************************************************************/
Condition::~Condition() throw(){
	IA20_TRACER;

	int iResult = -1;

#ifdef _linux
	if (theCond.__data.__nwaiters > 1){
		IA20_LOG(LogLevel::INSTANCE.isError(), "GLIBC skip pthread_cond_destroy after other waiting process killed !");
		return;
	}
#endif

	if((iResult = pthread_cond_destroy(&theCond)) != 0){
		IA20_LOG(LogLevel::INSTANCE.isSystem(), "Error condition destruction, res="<<iResult<<".");
	}

}
/*************************************************************************/
void Condition::wait(Mutex& mutex){
	IA20_TRACER;

	int iResult = -1;

	if( (iResult = pthread_cond_wait(&theCond, &mutex.theMutex) ) != 0){
		IA20_LOG(LogLevel::INSTANCE.isError(), "Error on cond wait res="<<iResult<<".");
		IA20_THROW(IA20::ThreadLockException("pthread_cond_wait",iResult));
	}
}
/*************************************************************************/
bool Condition::wait(Mutex& mutex, unsigned int iTimeoutMS) {
	IA20_TRACER;

	int iResult = -1;

	struct timespec ts;
	::clock_gettime(CLOCK_MONOTONIC, &ts);

	//TODO (L) burak w liczeniu czasu ...
	ts.tv_sec += iTimeoutMS / 1000 + ts.tv_nsec/1000000000 ;
	ts.tv_nsec = (ts.tv_nsec  + (iTimeoutMS%1000) *1000000) % 1000000000;

	if( (iResult = pthread_cond_timedwait(&theCond, &mutex.theMutex, &ts) ) != 0){

		if(iResult == ETIMEDOUT){
			IA20_LOG(LogLevel::INSTANCE.isDetailedInfo(), "Current: "<<TypeTools::GetTimeMS());
			return false;
		}

		IA20_LOG(LogLevel::INSTANCE.isError(), "Error on cond wait res="<<iResult<<":"<<strerror(iResult));
		IA20_THROW(IA20::ThreadLockException("pthread_cond_wait",iResult));
	}

	return true;
}
/*************************************************************************/

void Condition::signal() {
	IA20_TRACER;

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	if( (iResult = pthread_cond_signal(&theCond)) != 0){
		IA20_LOG(LogLevel::INSTANCE.isError(), "Error on cond signal res="<<iResult<<".");
		IA20_THROW(IA20::ThreadLockException("pthread_cond_signal",iResult));
	}
#endif

}

/*************************************************************************/
void Condition::broadcast() {
	IA20_TRACER;

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	if((iResult = pthread_cond_broadcast(&theCond)) != 0){
		IA20_LOG(LogLevel::INSTANCE.isError(), "Error on cond broadcast res="<<iResult<<".");
		IA20_THROW(IA20::ThreadLockException("pthread_cond_broadcast",iResult));
	}
#endif

}
/*************************************************************************/

}
