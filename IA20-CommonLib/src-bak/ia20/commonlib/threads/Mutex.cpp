/*
 * File: IA20-CommonLib/src/commonlib/threads/Mutex.cpp
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

#include "Mutex.h"

#include <ia20/commonlib/logger/logger.h>
#include <ia20/commonlib/memory/memory.h>
#include <string.h>
#include "Condition.h"
#include "ThreadLockException.h"

namespace IA20 {

/*************************************************************************/

Mutex::Mutex(bool bShared, bool bRobust){

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	pthread_mutexattr_t mutex_attr;

	if ((iResult = pthread_mutexattr_init(&mutex_attr)) != 0)
		IA20_THROW(SystemException("pthreas_mutexattr_init", iResult));

	if (bShared) {
		if ((iResult = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED)) != 0)
			IA20_THROW(SystemException("PTHREAD_PROCESS_SHARED", iResult));
	}
#ifdef __USE_XOPEN2K
	if (bRobust) {
		if ((iResult = pthread_mutexattr_setrobust(&mutex_attr,	PTHREAD_MUTEX_ROBUST)) != 0)
			IA20_THROW(SystemException("PTHREAD_MUTEX_ROBUST", iResult));
	}
#endif

	if ((iResult = pthread_mutex_init(&theMutex, &mutex_attr)) != 0) {
		IA20_THROW(SystemException("PTHREAD_MUTEX_ROBUST", iResult));
	}

#endif

}

/*************************************************************************/

Mutex::~Mutex() throw() {

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	if((iResult = pthread_mutex_destroy(&theMutex)) != 0){
		IA20_LOG(LogLevel::INSTANCE.isSystem(), "Error mutex destruction, res="<<iResult<<".");
	}
#endif

}

//TODO (M) throw exceptions here ?
/*************************************************************************/

void Mutex::lock() {

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	if((iResult = pthread_mutex_lock( &theMutex )) != 0){

#ifdef __USE_XOPEN2K
		if(iResult==EOWNERDEAD)
			iResult=pthread_mutex_consistent( &theMutex );
#endif

		if(iResult!=0)
			IA20_LOG(LogLevel::INSTANCE.isSystem(), "Error on memory mutex lock, res="<<iResult<<":"<<strerror(iResult));
	}

#endif

}
/*************************************************************************/
bool Mutex::trylock() {

	int iResult = -1;

	if((iResult = pthread_mutex_trylock( &theMutex )) != 0){

		if(iResult==EBUSY)
			return false;

#ifdef __USE_XOPEN2K
		if(iResult==EOWNERDEAD)
			iResult=pthread_mutex_consistent( &theMutex );
#endif

		if(iResult!=0){
			IA20_LOG(LogLevel::INSTANCE.isSystem(), "Error on memory mutex lock, res="<<iResult<<":"<<strerror(iResult));
		}
	}

	return true;
}
/*************************************************************************/
void Mutex::unlock() {

#ifdef IA20_FEATURE_THREAD_SAFE
	int iResult = -1;

	if((iResult = pthread_mutex_unlock( &theMutex )) != 0){
		IA20_LOG(LogLevel::INSTANCE.isSystem(), "Error on memory mutex lock, res="<<iResult<<"."<<errno);
	}
#endif
}
/*************************************************************************/
Mutex::Unlocker::Unlocker(Mutex& refLock) throw():
 refLock(refLock){
}
/*************************************************************************/
Mutex::Unlocker::~Unlocker() throw(){
	refLock.unlock();
}
/*************************************************************************/
Mutex::Locker::Locker(Mutex& refLock) throw():
 Unlocker(refLock){
	refLock.lock();
}
/*************************************************************************/
Mutex::Locker::Locker(Mutex& refLock,TimeSamplesResults& tsr) throw():
 Unlocker(refLock){

	TimeSample ts(::IA20::LogLevel::INSTANCE.isProfile());
	refLock.lock();
	tsr.addSample(ts);
}
/*************************************************************************/
void Mutex::wait(Condition& cond){
	cond.wait(*this);
}
/*************************************************************************/
bool Mutex::wait(Condition& cond, unsigned int iTimeoutMS){
	return cond.wait(*this,iTimeoutMS);
}
/*************************************************************************/
}
