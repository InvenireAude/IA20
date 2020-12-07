/*
 * File: IA20-CommonLib/src/commonlib/threads/ThreadLockException.h
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
#ifndef THREADLOCKEXCEPTION_H_
#define THREADLOCKEXCEPTION_H_

/*
 *
 */
#include "../exception/Exception.h"

namespace IA20 {

/** The class. */
 class ThreadLockException: public IA20::Exception {
public:
	ThreadLockException(const std::string& strInfo, int iErrCode = 0)throw();
	virtual ~ThreadLockException()throw();

	virtual const char* getName();
protected:
	ThreadLockException()throw();
};

/** The class. */
 class ThreadLockTimeoutException: public IA20::ThreadLockException {
public:
	ThreadLockTimeoutException()throw();
	virtual const char* getName();
};

}

#endif /* THREADLOCKEXCEPTION_H_ */
