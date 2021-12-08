/*
 * File: IA20-CommonLib/src/commonlib/logger/LoggerFileLock.h
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
#ifndef _IA20_SYS_FS_LoggerFileLock_H_
#define _IA20_SYS_FS_LoggerFileLock_H_


#include <pthread.h>

namespace IA20 {
namespace LOGGER {

/*************************************************************************/
/** The LoggerFileLock class.
 *
 */
class LoggerFileLock {
public:

	LoggerFileLock();
	virtual ~LoggerFileLock() throw();

	virtual void lock();
	virtual void unlock();

	class Locker{
		public:
		Locker(LoggerFileLock& lock):lock(lock){lock.lock();}
		~Locker(){ lock.unlock(); }
		LoggerFileLock& lock;
	};

protected:

	int fd;
	struct Lock;

	Lock *tryShared();
	Lock *pMyLock;
	Lock *pLockShared;

};

/*************************************************************************/
}
}

#endif /* _IA20_SYS_FS_LoggerFileLock_H_ */
