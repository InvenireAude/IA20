/*
 * File: IA20-CommonLib/src/commonlib/threads/Mutex.h
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
#ifndef _IA20_THREAD_Mutex_H_
#define _IA20_THREAD_Mutex_H_

#include "../config.h"
#include <pthread.h>

#include <ia20/commonlib/time/TimeSamplesRegister.h>


/*
 *
 */
namespace IA20 {

class Condition;

/** The class. */
 class Mutex {
public:


  class Unlocker{
  public:
	  Unlocker(Mutex& refLock)throw();
	  ~Unlocker()throw();

	 protected:
	 	Mutex& refLock;
  };

  class Locker : public Unlocker{
	 public:
	  Locker(Mutex& refLock)throw();
	  Locker(Mutex& refLock, TimeSamplesResults& tsr)throw();

	  void wait(Condition& cond) ;
	  bool wait(Condition& cond, unsigned int iTimeoutMS);

	};


	Mutex(bool bShared=false, bool bRobust=true);
	~Mutex()throw();

	bool trylock();

	void lock();
	void unlock();

    void wait(Condition& cond);
	bool wait(Condition& cond, unsigned int iTimeoutMS);


protected:
	pthread_mutex_t    theMutex;
	friend class Condition;
};

}

#endif /* THREADSAFELOCK_H_ */
