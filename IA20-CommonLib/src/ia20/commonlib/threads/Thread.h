/*
 * File: IA20-CommonLib/src/commonlib/threads/Thread.h
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
#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#include "ThreadSpecific.h"

namespace IA20 {
class Runnable;

/** The class. */
 class Thread {
public:
	virtual ~Thread();

	void start();
	void join();
	void stop();

	class Cancellation{
	public:
		Cancellation(bool bEnabled);
		~Cancellation();

		static void Test();
	protected:
		int     state;
	};

	typedef ThreadSpecific<Thread> Specific;


protected:

	static void* Starter(void *pArg);

	Thread(Runnable *pRunnable);

	Runnable *pRunnable;

	pthread_t theThread;

	bool bIsStarted;
	bool bCanceled;

};

}

#endif /* THREAD_H_ */
