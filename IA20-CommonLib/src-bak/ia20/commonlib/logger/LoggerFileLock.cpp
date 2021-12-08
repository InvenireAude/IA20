/*
 * File: IA20-CommonLib/src/commonlib/logger/LoggerFileLock.cpp
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
#include "LoggerFileLock.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include <sys/mman.h>

/*************************************************************************/
/* The logger lock.
 * The mutex code must have be repeated since we must ensure
 *  that no logging is done inside those functions.
 */

namespace IA20 {
namespace LOGGER {
/*************************************************************************/
struct LoggerFileLock::Lock{
	 Lock(bool bShared);
	 pthread_mutex_t mutex;

	 void lock();
	 void unlock();

	 static Lock TheInstance;
};
/*************************************************************************/
LoggerFileLock::Lock LoggerFileLock::Lock::TheInstance(false);
/*************************************************************************/
LoggerFileLock::Lock::Lock(bool bShared){

	pthread_mutexattr_t mutex_attr;
	int iResult;

	if ((iResult = pthread_mutexattr_init(&mutex_attr)) != 0){
		std::cerr<<"\nLoggerFileLock::Lock::unlock failed:"<<strerror(iResult)<<std::endl;
		return;
	}

	if (bShared) {
		if ((iResult = pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED)) != 0){
			std::cerr<<"\nLoggerFileLock::Lock::pthread_mutexattr_setpshared failed:"<<strerror(iResult)<<std::endl;
			return;
		}
	}

#ifdef __USE_XOPEN2K
	if ((iResult = pthread_mutexattr_setrobust(&mutex_attr,	PTHREAD_MUTEX_ROBUST)) != 0){
		std::cerr<<"\nLoggerFileLock::Lock::pthread_mutexattr_setrobust failed:"<<strerror(iResult)<<std::endl;
		return;
	}
#endif


	if ((iResult = pthread_mutex_init(&mutex, &mutex_attr)) != 0){
		std::cerr<<"\nLoggerFileLock::Lock::unlock failed:"<<strerror(iResult)<<std::endl;
		return;
	}

}
/*************************************************************************/

void LoggerFileLock::Lock::lock() {

	int iResult = -1;

	if((iResult = pthread_mutex_lock( &mutex )) != 0){
		if(iResult==EOWNERDEAD){
#ifdef __USE_XOPEN2K
			pthread_mutex_consistent( &mutex );
#endif
		}else{
			std::cerr<<"\nLoggerFileLock::Lock::lock failed:"<<strerror(iResult)<<std::endl;
		}
	}
}
/*************************************************************************/
void  LoggerFileLock::Lock::unlock() {

	int iResult = -1;

	if((iResult = pthread_mutex_unlock( &mutex )) != 0){
		std::cerr<<"\nLoggerFileLock::Lock::unlock failed:"<<strerror(iResult)<<std::endl;
	}
}
/*************************************************************************/
	LoggerFileLock::LoggerFileLock():
		pLockShared(0),
		pMyLock(0){


		pMyLock=tryShared();
		if(!pMyLock)
			pMyLock=&LoggerFileLock::Lock::TheInstance;

}
/*************************************************************************/
LoggerFileLock::Lock* LoggerFileLock::tryShared(){

	if(!getenv("IA20_LOGGER_LOCK"))
		return 0;

	bool bInitialize=true;

	if(getenv("IA20_LOGGER_LOCK"))
		fd = open(getenv("IA20_LOGGER_LOCK"), O_RDWR | O_CREAT | O_EXCL, 0666);

	if(fd != -1){

	}else if(fd == -1 && errno == EEXIST){

		fd = open(getenv("IA20_LOGGER_LOCK"), O_RDWR, 0666);

		if(fd == -1){
      std::cerr<<"\nLoggerFileLock::tryShared failed:"<<strerror(errno)<<std::endl;
			return 0;
		}

		bInitialize=false;

	}else {
		std::cerr<<"\nLoggerFileLock::tryShared failed:"<<strerror(errno)<<std::endl;
		return 0;
	}

	if(bInitialize)
		ftruncate(fd,sizeof(Lock));

	pLockShared=(Lock*)mmap(0,sizeof(Lock), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(!pLockShared){
		perror("mmap in logfilelock.");
		return 0;
	}

	if(bInitialize)
		new(pLockShared)Lock(true);

	return pLockShared;
}
/*************************************************************************/
LoggerFileLock::~LoggerFileLock() throw(){

	if(pLockShared)
		munmap(pLockShared,sizeof(Lock));

	if(fd >=0)
		close(fd);
}
/*************************************************************************/
void LoggerFileLock::lock(){
	pMyLock->lock();

}
/*************************************************************************/
void LoggerFileLock::unlock(){
	pMyLock->unlock();
}
/*************************************************************************/
}
}

