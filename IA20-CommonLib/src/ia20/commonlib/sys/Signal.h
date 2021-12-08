/*
 * File: IA20-CommonLib/src/commonlib/sys/Signal.h
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
#ifndef _IA20_SYS_Signal_H_
#define _IA20_SYS_Signal_H_

#include "../memory/InstanceFeature.h"

#include <set>

namespace IA20 {
class Thread;

namespace SYS {

/*************************************************************************/
/** The Signal class.
 *
 */
class Signal : public InstanceFeature<Signal>{
public:

	virtual ~Signal() throw();

	bool isStopping()const;

	class ThreadRegistration{
	public:
		ThreadRegistration();
		~ThreadRegistration();
	};

	static void SignalHandler(int iSignal = 0);
	static void SignalHandlerStopOnly(int iSignal = 0);
    static void User1SignalHandler(int iSignal = 0);
    static void User2SignalHandler(int iSignal = 0);

	static Signal* GetInstance();

  class UserSignalCallback {
    public:
      UserSignalCallback(bool bActive);
      virtual ~UserSignalCallback();
      virtual void handleUserSignal() = 0;
    protected:
      bool bActive;
  };

	static void StopApplication(bool bCancelThreads = true);

	Signal();

protected:

	typedef std::set<Thread*> ThreadSet;
	ThreadSet                 setThreadsToStop;

	typedef std::set<UserSignalCallback*> UserSignalCallbackSet;
	UserSignalCallbackSet                 setUserSignalCallback;

	void addThread(Thread* pThread);
	void removeThread(Thread* pThread);

  void addUserSignalCallback(UserSignalCallback *pUserSignalCallback);
  void removeUserSignalCallback(UserSignalCallback *pUserSignalCallback);

	void stopThreads();
  void handleUserSignal();

	static bool bStopping;
};

/*************************************************************************/
}
}

#endif /* _IA20_SYS_Signal_H_ */
