/*
 * File: IA20-CommonLib/src/commonlib/threads/ThreadSpecific.h
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
#ifndef _IA20_Threads_ThreadSpecific_h_
#define _IA20_Threads_ThreadSpecific_h_

#include <pthread.h>
#include <stdio.h>

namespace IA20 {
template<class T>
class ThreadSpecific {
public:

	public:

	ThreadSpecific():key(0){
			static const pthread_once_t _once_control = PTHREAD_ONCE_INIT;
			once_control = _once_control;
		};

	struct Pointer{
		inline T* operator->()const{ return ThreadSpecific::GetPointer(); }
		inline operator T*()const{ return ThreadSpecific::GetPointer(); }
		inline T& operator*()const{ return *ThreadSpecific::GetPointer(); }
		inline T* operator=(T* p){ ThreadSpecific::SetPointer(p); return p;}
	};

	static T* GetPointer(){
		InitializeIfNeeded();
		return (T*) pthread_getspecific(INSTANCE.key);
	}

	static void SetPointer(T* p){
		InitializeIfNeeded();
		pthread_setspecific(INSTANCE.key,p);
	}

	protected:

	static void InitializeIfNeeded(){
		if(pthread_once(&(INSTANCE.once_control), ThreadSpecific::Init)!=0)
			perror("pthread_once");
	}

	static void Init(){
		if(pthread_key_create(&(INSTANCE.key), 0)!=0)
			perror("pthread_key_create");
	}

	pthread_key_t key;
	pthread_once_t once_control;

	static ThreadSpecific INSTANCE;
};

template<class T>
ThreadSpecific<T> ThreadSpecific<T>::INSTANCE;

/******************************************************************************************/

}

#endif /* _IA20_Threads_ThreadSpecific_h_ */
